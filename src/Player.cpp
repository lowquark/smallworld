#include "Player.h"

using namespace std;


packet::Player::Player(const ::Player * _player) : packet::Base(PACKETID)
{
	playerId = _player->m_id;

	strncpy(name, _player->m_name.c_str(), packet::maxNameLength);
	name[packet::maxNameLength] = '\0';

	//followedAgentId = _player->m_followedAgent ? _player->m_followedAgent->getId() : -1;

	for(int i = 0;i < ::Player::KEYNUM;i ++)
	{
		keyStates[i] = _player->m_keyStates[i];
	}
}
void packet::Player::update(::Player * _player, const World & _world) const
{
	_player->m_name = name;

	/*
	if((_player->m_followedAgent && followedAgentId != _player->m_followedAgent->getId()) || (!_player->m_followedAgent && followedAgentId != -1))
	{
		_player->m_followedAgent = _world.getAgent(followedAgentId);
	}
	*/

	for(int i = 0;i < ::Player::KEYNUM;i ++)
	{
		_player->m_keyStates[i] = keyStates[i];
	}
}


Player::Player(int _id) : m_id(_id)
{
	for(int i = 0;i < KEYNUM;i ++)
	{
		m_keyStates[i] = false;
	}
}

void Player::tick(Agent * _agent) const
{
	bool move = false;

	if(isKeyDown(KEY_UP))
	{
		_agent->push(dl::Vector3D(0, 5000, 0));
		move = true;
	}
	if(isKeyDown(KEY_LEFT))
	{
		_agent->push(dl::Vector3D(-5000, 0, 0));
		move = true;
	}
	if(isKeyDown(KEY_DOWN))
	{
		_agent->push(dl::Vector3D(0, -5000, 0));
		move = true;
	}
	if(isKeyDown(KEY_RIGHT))
	{
		_agent->push(dl::Vector3D(5000, 0, 0));
		move = true;
	}

	if(!move)
		_agent->dampen(5000);

	if(isKeyDown(KEY_JUMP))
		_agent->jump(1300);
}

int Player::getId() const
{
	return m_id;
}

const std::string & Player::getName() const
{
	return m_name;
}
void Player::setName(const std::string & _name)
{
	m_name = _name;
}

bool Player::isKeyDown(Key _key) const
{
	if(_key == Key::KEY_INVALID)
		return false;

	return m_keyStates[(int)_key];
}
bool Player::isKeyUp(Key _key) const
{
	if(_key == Key::KEY_INVALID)
		return false;

	return !isKeyDown(_key);
}
void Player::setKeyDown(Key _key)
{
	if(_key != Key::KEY_INVALID)
		m_keyStates[(int)_key] = true;
}
void Player::setKeyUp(Key _key)
{
	if(_key != Key::KEY_INVALID)
		m_keyStates[(int)_key] = false;
}
