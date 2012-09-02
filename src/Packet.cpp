#include "Packet.h"

using namespace packet;
using namespace std;

const int packet::CHANNEL_NUM(4);

Base::Base(int _id) : id(_id)
{
}

PacketRequest::PacketRequest(int _requestId) : Base(PACKETID), requestId(_requestId)
{
}

JoinRequest::JoinRequest(const string & _name) : Base(PACKETID)
{
}

JoinResponse::JoinResponse(const std::string & _message) : Base(PACKETID), playerId(-1), agentId(-1)
{
	setMessage(_message);
}

void JoinResponse::setMessage(const string & _message)
{
	strncpy(message, _message.c_str(), MESSAGE_LENGTH);
	message[MESSAGE_LENGTH] = '\0'; // Terminating zero
}
void JoinResponse::setAsOk(int _playerId, int _agentId)
{
	playerId = _playerId;
	agentId = _agentId;
}
void JoinResponse::setAsBad(const string & _reason)
{
	playerId = -1;
	agentId = -1;
	setMessage(_reason);
}
bool JoinResponse::isOk()
{
	return playerId != -1;
}

GameInfo::GameInfo() : Base(PACKETID)
{
}
TextMessage::TextMessage() : Base(PACKETID)
{
}
