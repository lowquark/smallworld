#include "Game.h"
#include "GameRenderer.h"
#include "Packet.h"

#include <iostream>
#include <cstdlib>
#include <enet/enet.h>

#include <DL/Geom.h>
#include <DL/RenderLoop.h>

using namespace std;


Game game;
Player * ourPlayer = 0;

enum StartupMode {OFFLINE_TEST, CONNECT_TO_SERVER};
StartupMode mode = OFFLINE_TEST;

//Rendering stuff
dl::RenderLoop renderLoop(60, true);

int width = 1000, height = 550;
bool fullscreen = false;
GameRenderer gameRenderer(game);

//Connection stuff
ENetHost * host = 0;
ENetPeer * server = 0;
string ip = "";
int port = 7007;
int NET_TIMEOUT = 10000;

bool connectedToServer()
{
	return host && server;
}

bool connectToServer(const std::string & _ipStr, int _port)
{
	cout << "Attempting to connect to " << _ipStr << " over port " << _port << "... " << flush;

	host = enet_host_create(0, 1, packet::CHANNEL_NUM, 0, 0);

	if(host)
	{
		ENetAddress address;
		address.port = _port;
		enet_address_set_host(&address, _ipStr.c_str());

		server = enet_host_connect(host, &address, packet::CHANNEL_NUM, 0);

		ENetEvent event;
		if(enet_host_service(host, &event, NET_TIMEOUT) && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			cout << "Success!" << endl;

			enet_host_flush(host); //Make sure the server knows we connected

			return true;
		}
		else
		{
			enet_peer_reset(server); //Clean up the peer object
			server = 0;

			enet_host_destroy(host); //Clean up the host object
			host = 0;

			cout << "Connection timed out." << endl;
		}
	}
	else
	{
		cout << "Failed to create host object" << endl;
	}

	return false;
}
void disconnectFromServer()
{
	enet_peer_disconnect(server, 0); //Disconnect

	enet_host_flush(host); //Make sure the server knows we disconnected
}

//Waits for a particular packet
template <typename T>
bool waitForPacket(T * _packet, int _timeout)
{
	ENetEvent event;
	if(enet_host_service(host, &event, NET_TIMEOUT) && event.type == ENET_EVENT_TYPE_RECEIVE)
	{
		T * packet = (T *)event.packet->data;

		if(packet->id == T::PACKETID)
		{
			memcpy(_packet, event.packet->data, sizeof(T));

			return true;
		}
	}

	return false;
}

//Sends a packet request
void requestPacket(int _id)
{
	packet::PacketRequest request;
	request.requestId = _id;

	sendPacket<packet::PacketRequest>(server, &request);
}

bool getServerInfo(packet::GameInfo * _info)
{
	cout << "Requesting server info... " << flush;
	requestPacket(packet::GameInfo::PACKETID);

	if(waitForPacket<packet::GameInfo>(_info, NET_TIMEOUT))
	{
		cout << "Done!" << endl;

		return true;
	}

	cout << "Attempt timed out..." << endl;

	return false;
}

void sendJoinRequest(const std::string & _name)
{
	packet::JoinRequest request(_name);

	sendPacket<packet::JoinRequest>(server, &request);
}

bool joinServer(const std::string & _name)
{
	cout << "Attempting to join server... " << flush;

	//Send a JoinRequest packet
	sendJoinRequest(_name);

	packet::JoinResponse response;
	if(waitForPacket<packet::JoinResponse>(&response, NET_TIMEOUT))
	{
		//We're good to join the game
		if(response.isOk())
		{
			cout << "Success!" << endl;

			if(strlen(response.message) != 0)
				cout << "Message: " << response.message << endl;

			cout << "Assigned player id: " << response.playerId << endl;

			ourPlayer = game.addPlayer(_name, response.playerId);

			if(response.agentId != -1)
			{
				cout << "Assigned agent id: " << response.agentId << endl;

				Agent * ourAgent = game.addAgent(_name, response.agentId);

				ourPlayer->setAgent(ourAgent);

				gameRenderer.setFollowedAgent(ourAgent);
			}

			return true;
		}
		else
		{
			cout << "Denied." << endl;
			cout << "Reason: " << response.message << endl;
		}
	}
	else
	{
		cout << "Attempt timed out..." << endl;
	}

	return false;
}

Player::Key getPlayerKey(SDLKey _sym)
{
	switch(_sym)
	{
		case SDLK_w: return Player::Key::KEY_UP;
		case SDLK_a: return Player::Key::KEY_LEFT;
		case SDLK_s: return Player::Key::KEY_DOWN;
		case SDLK_d: return Player::Key::KEY_RIGHT;

		case SDLK_SPACE: return Player::Key::KEY_JUMP;

		default: return Player::Key::KEY_INVALID;
	}
}

void handleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
		{
			renderLoop.stop(); //Finish up
		}
		else if(event.type == SDL_KEYDOWN)
		{
			//Let the player know about our keypress
			ourPlayer->setKeyDown(getPlayerKey(event.key.keysym.sym));

			if(event.key.keysym.sym == SDLK_ESCAPE)
				renderLoop.stop(); //Finish up
		}
		else if(event.type == SDL_KEYUP)
		{
			ourPlayer->setKeyUp(getPlayerKey(event.key.keysym.sym));
		}
	}
}

void handlePlayerPacket(packet::Player * _packet)
{
	if(_packet->playerId != ourPlayer->getId())
	{
		game.updatePlayer(_packet);
	}
}
void handleAgentPacket(packet::Agent * _packet)
{
	game.updateAgent(_packet);
}

void handlePacket(packet::Base * _packet)
{
	if(_packet->id == packet::Player::PACKETID)
	{
		handlePlayerPacket((packet::Player *)_packet);
	}
	if(_packet->id == packet::Agent::PACKETID)
	{
		handleAgentPacket((packet::Agent *)_packet);
	}
}

void handleNetwork()
{
	ENetEvent event;
	while(enet_host_service(host, &event, 0))
	{
		if(event.type == ENET_EVENT_TYPE_RECEIVE)
		{
			handlePacket((packet::Base *)event.packet->data);

			enet_packet_destroy(event.packet);
		}
		else if(event.type == ENET_EVENT_TYPE_DISCONNECT)
		{
			cout << "The server disconnected." << endl;
			renderLoop.stop(); //Finish up
		}
	}
}

void loop()
{
	handleEvents();

	if(connectedToServer())
		handleNetwork();
}

void tick()
{
	game.tick();

	gameRenderer.tick();

	if(connectedToServer())
	{
		if(ourPlayer)
		{
			packet::Player packet(ourPlayer);

			sendPacket<packet::Player>(server, &packet, packet::PLAYER_UPDATES_CHANNEL);
		}
	}
}

void render()
{
	gameRenderer.render();
}

void printUsage()
{
	cout << "Usage: smallworld [OPTION]" << endl;
	cout << endl;
	cout << "  -c, --connect     <HOST>[:PORT]      Connect to server at HOST over port PORT" << endl;
	cout << endl;
	cout << "  -f, --fullscreen                     Run in fullscreen mode" << endl;
	cout << "  -r, --resolution  <WIDTH>x<HEIGHT>   Set the window resolution to WIDTH by HEIGHT" << endl;
	cout << endl;
	cout << "  -h, --help                           Display this help and exit" << endl;
}

bool parseOption(int _argc, char ** _argv)
{
	string option = _argv[0];
	if(option == "--connect" || option == "-c")
	{
		if(_argc > 1)
		{
			char hostStr[50];

			sscanf(_argv[1], "%50[a-zA-Z0-9.]:%d", hostStr, &port);

			ip = hostStr;

			if(ip == "localhost")
				ip = "127.0.0.1";

			mode = CONNECT_TO_SERVER;

			return true;
		}
	}
	else if(option == "--resolution" || option == "-r")
	{
		if(_argc > 1)
		{
			string resStr = _argv[1];
			int xPos = resStr.find_first_of("x");

			width = atoi(resStr.substr(0, xPos).c_str());
			height = atoi(resStr.substr(xPos + 1).c_str());

			return true;
		}
	}
	else if(option == "--fullscreen" || option == "-f")
	{
		fullscreen = true;

		return true;
	}
	else if(option == "--help" || option == "-h")
	{
		printUsage();

		exit(0);
	}

	return false;
}

bool parseOptions(int _argc, char ** _argv)
{
	char ** prevOption = 0;
	int argNum = 1;
	for(int i = 0;i < _argc;i ++)
	{
		if(_argv[i][0] == '-')
		{
			if(prevOption)
			{
				if(!parseOption(argNum, prevOption))
				{
					return false;
				}
			}

			prevOption = &_argv[i];
			argNum = 1;
		}
		else
		{
			argNum ++;
		}
	}

	if(prevOption)
	{
		if(!parseOption(argNum, prevOption))
		{
			return false;
		}
	}

	return true;
}

int main(int _argc, char ** _argv)
{
	cout << (float)0xFFFFFFFF << endl;
	//Parse the command line flags
	if(!parseOptions(_argc, _argv))
	{
		printUsage();
		return 1;
	}

	if(mode == OFFLINE_TEST)
	{
		ourPlayer = game.addPlayer("Dave");

		Agent * ourAgent = game.addAgent("Dave");

		ourPlayer->setAgent(ourAgent);

		gameRenderer.setFollowedAgent(ourAgent);
		gameRenderer.createWindow(width, height, fullscreen);

		renderLoop.run(loop, tick, render);
	}
	else if(mode == CONNECT_TO_SERVER)
	{
		if(enet_initialize()) //Initialize enet
		{
			cout << "Error initializing enet..." << endl;
			return 1;
		}

		if(connectToServer(ip, port))
		{
			packet::GameInfo gameInfo;
			if(getServerInfo(&gameInfo))
			{
				cout << "  Server name: " << gameInfo.name << endl;

				if(joinServer("Dave"))
				{
					gameRenderer.createWindow(width, height, fullscreen);

					renderLoop.run(loop, tick, render);
				}
			}
		}

		if(server)
		{
			disconnectFromServer();

			enet_host_destroy(host);
		}

		enet_deinitialize();
	}

	return 0;
}
