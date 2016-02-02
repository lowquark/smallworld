#include "Game.h"
#include "Packet.h"
#include "lua.h"

#include <enet/enet.h>
#include <iostream>
#include <sstream>
#include <vector>

#include <DL/Time.h>

using namespace std;

ENetHost * host = 0;

lua_State * luaState = luaL_newstate();
Game game;

std::string ipStr(enet_uint32 _ip)
{
	unsigned char a = ((unsigned char *)&_ip)[0];
	unsigned char b = ((unsigned char *)&_ip)[1];
	unsigned char c = ((unsigned char *)&_ip)[2];
	unsigned char d = ((unsigned char *)&_ip)[3];

	stringstream ss;

	ss << (int)a << "." << (int)b << "." << (int)c << "." << (int)d;

	return ss.str();
}

struct JoinInfo
{
	std::string name;

	std::string ip;
	int port;

	void pushLuaData(lua_State * _luaState) const
	{
		lua_newtable(_luaState);
			lua_pushstring(_luaState, name.c_str());   lua_setfield(_luaState, -2, "name");

			lua_pushstring(_luaState, ip.c_str());     lua_setfield(_luaState, -2, "ip");
			lua_pushinteger(_luaState, port);          lua_setfield(_luaState, -2, "port");
	}
};
bool lua_allowJoin(const JoinInfo * _info, std::string * _message)
{
	lua_getglobal(luaState, "server");
		lua_getfield(luaState, -1, "allowJoin");

		_info->pushLuaData(luaState);

		lua_call(luaState, 1, 2);

		bool allowJoin = lua_toboolean(luaState, -2);
		if(lua_isstring(luaState, -1))
		{
			*_message = lua_tostring(luaState, -1);
		}

		lua_pop(luaState, 2);

	lua_pop(luaState, 1);

	return allowJoin;
}

//Packet Sending
void sendGameInfo(const std::string & _name, ENetPeer * _peer)
{
	packet::GameInfo gameInfo;

	//Set up the values
	strcpy(gameInfo.name, _name.c_str());

	//Send the packet
	sendPacket<packet::GameInfo>(_peer, &gameInfo);
}

//Individual packet handling
void handlePacketRequest(packet::PacketRequest * _packet, ENetPeer * _peer)
{
	if(_packet->requestId == packet::GameInfo::PACKETID)
	{
		sendGameInfo(game.getName(), _peer);
	}
}
void handleJoinRequest(packet::JoinRequest * _packet, ENetPeer * _peer)
{
	JoinInfo info;
	info.name = _packet->name;
	info.ip = ipStr(_peer->address.host);
	info.port = _peer->address.port;

	packet::JoinResponse response;
	string message;
	Player * player = 0;

	//Check with lua first
	if(lua_allowJoin(&info, &message))
	{
		response.setMessage(message);

		//Try to add the player
		Player * player = game.addPlayer(info.name);

		if(player)
		{
			bool spectate = false;
			
			if(spectate)
			{
				response.setAsOk(player->getId(), -1);
			}
			else
			{
				Agent * agent = game.addAgent(info.name);
				response.setAsOk(player->getId(), agent->getId());

				player->setAgent(agent);
			}

			_peer->data = player;
		}
		else
		{
			response.setAsBad("Game is full");
		}
	}
	else
	{
		response.setAsBad(message);
	}

	//Send the packet
	sendPacket<packet::JoinResponse>(_peer, &response);

	if(response.isOk())
	{
		//Send all player data
	}
}
void handlePlayerPacket(packet::Player * _packet, ENetPeer * _peer)
{
	//TODO: Ensure the packet is from the specified player
	game.updatePlayer(_packet);

	//cout << "Agent position: " << game.getPlayer(_packet->playerId)->getFollowedAgent()->getPosition() << endl;

	broadcastPacket<packet::Player>(host, _packet, packet::PLAYER_UPDATES_CHANNEL);
}

void handlePacket(packet::Base * _packet, ENetPeer * _peer)
{
	if(_packet->id == packet::PacketRequest::PACKETID)
	{
		handlePacketRequest((packet::PacketRequest *)_packet, _peer);
	}
	else if(_packet->id == packet::JoinRequest::PACKETID)
	{
		handleJoinRequest((packet::JoinRequest *)_packet, _peer);
	}
	else if(_packet->id == packet::Player::PACKETID)
	{
		handlePlayerPacket((packet::Player *)_packet, _peer);
	}
}

void tick()
{
	game.tick(); //Tick the game!

	for(auto & agent : game.getWorld().getAgentList())
	{
		if(agent)
		{
			packet::Agent packet(agent);
			broadcastPacket<packet::Agent>(host, &packet, packet::AGENT_UPDATES_CHANNEL);
		}
	}
}

int main(int _argc, char ** _argv)
{
	luaL_openlibs(luaState);

	//Create the server global table
	lua_newtable(luaState); lua_setglobal(luaState, "server");

	//Execute the server file
	if(luaL_dofile(luaState, "res/local/server.lua"))
	{
		cout << "Error loading res/server.lua:" << endl << lua_tostring(luaState, -1) << endl;
		lua_pop(luaState, 1);
	}

	if(enet_initialize())
	{
		cout << "Error initializing enet..." << endl;
		return 1;
	}

	ENetAddress address;

	address.host = ENET_HOST_ANY;
	address.port = 7007;

	host = enet_host_create(&address, Game::MAX_PLAYERS, packet::CHANNEL_NUM, 0, 0);

	if(!host)
	{
		cout << "Error creating enet server..." << endl;
		return 1;
	}

	cout << "Successfully created smallworld server on port " << address.port << endl;
	cout << "Waiting for incoming connections..." << endl;

	double unprocessed = 0;
	double lastTime = dl::getSeconds();
	double secondsPerTick = (float)1/60;
	while(true)
	{
		ENetEvent event;
		if(enet_host_service(host, &event, 0) > 0)
		{
			if(event.type == ENET_EVENT_TYPE_CONNECT)
			{
				cout << "A client connected from " << ipStr(event.peer->address.host) << " on port " << event.peer->address.port << endl;
			}
			else if(event.type == ENET_EVENT_TYPE_RECEIVE)
			{
				handlePacket((packet::Base *)event.packet->data, event.peer);

				enet_packet_destroy(event.packet);
			}
			else if(event.type == ENET_EVENT_TYPE_DISCONNECT)
			{
				((Player *)event.peer->data)->getAgent()->setController(0);
				game.removePlayer((Player *)event.peer->data);

				cout << "A client disconnected from " << ipStr(event.peer->address.host) << " on port " << event.peer->address.port << endl;

				event.peer->data = 0;
			}
		}

		double now = dl::getSeconds();
		double elapsed = now - lastTime;

		lastTime = now;

		unprocessed += elapsed/secondsPerTick;

		while(unprocessed > 1)
		{
			tick();
			unprocessed -= 1;
		}
	}

	enet_host_destroy(host);

	enet_deinitialize();

	return 0;
}
