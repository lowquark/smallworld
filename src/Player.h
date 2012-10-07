#ifndef PLAYER_H
#define PLAYER_H

#include "Agent.h"
#include "Packet.h"
#include "World.h"

#include <iostream>
#include <list>

namespace packet {struct Player;}

class Player : public AgentController
{
	friend struct packet::Player;

  public:

	enum Key {KEY_INVALID = -1, KEY_UP = 0, KEY_LEFT = 1, KEY_DOWN = 2, KEY_RIGHT = 3, KEY_JUMP = 4};
	static const int KEYNUM = 5;

  private:

	int m_id;
	std::string m_name;

	//Agent * m_followedAgent;

	bool m_keyStates[KEYNUM]; // True for down, false for up

  public:

	Player(int _id = -1);

	void tick() const override;

	int getId() const;

	const std::string & getName() const;
	void setName(const std::string & _name);

	bool isKeyDown(Key _key) const;
	bool isKeyUp(Key _key) const;
	void setKeyDown(Key _key);
	void setKeyUp(Key _key);
};

namespace packet
{

struct Player : public Base
{
	static const int PACKETID = 10;

	int playerId;
	char name[maxNameLength + 1];

	//int followedAgentId;

	bool keyStates[::Player::KEYNUM]; // True for down, false for up

	Player(const ::Player * _player);
	void update(::Player * _player, const World & _world) const;
};

}

#endif
