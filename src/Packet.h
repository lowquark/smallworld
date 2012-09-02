#ifndef PACKET_H
#define PACKET_H

#include <DL/Geom.h>

#include <enet/enet.h>
#include <cstring>

namespace packet
{

//Channels
enum Channel {OTHER_UPDATES_CHANNEL = 0, WORLD_UPDATES_CHANNEL = 1, PLAYER_UPDATES_CHANNEL = 2, AGENT_UPDATES_CHANNEL = 3};
extern const int CHANNEL_NUM;

//Maximum lengths
const int maxNameLength = 30;

struct Base
{
	int id;

	Base(int _id);
};

struct PacketRequest : public Base
{
	static const int PACKETID = 0;

	int requestId;

	PacketRequest(int _requestId = -1);
};

struct JoinRequest : public Base
{
	static const int PACKETID = 1;

	char name[maxNameLength + 1];
	//TODO Sprite data

	JoinRequest(const std::string & _name);
};
struct JoinResponse : public Base
{
	static const int PACKETID = 2;
	static const int MESSAGE_LENGTH = 200;

	int playerId;
	int agentId;
	char message[MESSAGE_LENGTH + 1];

	JoinResponse(const std::string & _message = "");

	void setMessage(const std::string & _message);
	void setAsOk(int _playerId, int _agentId);
	void setAsBad(const std::string & _reason);
	bool isOk();
};

struct GameInfo : public Base
{
	static const int PACKETID = 12;

	char name[maxNameLength + 1];

	GameInfo();
};
struct TextMessage : public Base
{
	static const int PACKETID = 13;

	char sender[maxNameLength + 1];
	char message[300];

	TextMessage();
};

}

template <typename T>
void broadcastPacket(ENetHost * _host, T * _packet, int _channel = packet::OTHER_UPDATES_CHANNEL)
{
	int flags = 0;

	switch(_channel)
	{
		case packet::OTHER_UPDATES_CHANNEL: flags = ENET_PACKET_FLAG_RELIABLE;
		case packet::WORLD_UPDATES_CHANNEL: flags = ENET_PACKET_FLAG_RELIABLE;
		case packet::PLAYER_UPDATES_CHANNEL: flags = ENET_PACKET_FLAG_RELIABLE;
		case packet::AGENT_UPDATES_CHANNEL: flags = ENET_PACKET_FLAG_UNSEQUENCED;
	}

	ENetPacket * packet = enet_packet_create(_packet, sizeof(T), flags);

	enet_host_broadcast(_host, _channel, packet);
}
template <typename T>
void sendPacket(ENetPeer * _peer, T * _packet, int _channel = packet::OTHER_UPDATES_CHANNEL)
{
	int flags = 0;

	switch(_channel)
	{
		case packet::OTHER_UPDATES_CHANNEL: flags = ENET_PACKET_FLAG_RELIABLE;
		case packet::WORLD_UPDATES_CHANNEL: flags = ENET_PACKET_FLAG_RELIABLE;
		case packet::PLAYER_UPDATES_CHANNEL: flags = ENET_PACKET_FLAG_RELIABLE;
		case packet::AGENT_UPDATES_CHANNEL: flags = ENET_PACKET_FLAG_UNSEQUENCED;
	}

	ENetPacket * packet = enet_packet_create(_packet, sizeof(T), flags);

	enet_peer_send(_peer, _channel, packet);
}

#endif
