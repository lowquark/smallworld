#include "Game.h"

using namespace std;


Game::Game() : m_name("Unnamed Smallworld Server")
{
	for(int i = 0;i < MAX_PLAYERS;i ++)
	{
		m_playerList[i] = 0;
	}
}
Game::~Game()
{
}

void Game::tick()
{
	/*
	for(int i = 0;i < MAX_PLAYERS;i ++)
	{
		if(m_playerList[i])
		{
			m_playerList[i]->tick();
		}
	}
	*/

	m_world.tick();
}

void Game::updatePlayer(const packet::Player * _playerPacket)
{
	if(!m_playerList[_playerPacket->playerId])
	{
		addPlayer(_playerPacket->name, _playerPacket->playerId);
	}

	_playerPacket->update(m_playerList[_playerPacket->playerId], m_world);
}
void Game::updateAgent(const packet::Agent * _agentPacket)
{
	m_world.updateAgent(_agentPacket);
}

Player * Game::getPlayer(int _id)
{
	if(_id >= 0 && _id < MAX_PLAYERS)
	{
		return m_playerList[_id];
	}

	return 0;
}
Player * Game::addPlayer(const std::string & _name)
{
	for(int i = 0;i < MAX_PLAYERS;i ++)
	{
		if(Player * player = addPlayer(_name, i)) // Keep trying until it works
			return player;
	}

	return 0; // It's full
}
Player * Game::addPlayer(const std::string & _name, int _index)
{
	if(!m_playerList[_index])
	{
		m_playerList[_index] = new Player(_index);

		m_playerList[_index]->setName(_name);

		return m_playerList[_index];
	}

	return 0;
}
bool Game::removePlayer(const Player * _player)
{
	for(int i = 0;i < MAX_PLAYERS;i ++)
	{
		if(m_playerList[i] == _player)
		{
			delete m_playerList[i];
			m_playerList[i] = 0;

			return true;
		}
	}

	return false;
}
bool Game::isFull() const
{
	for(int i = 0;i < MAX_PLAYERS;i ++)
	{
		if(!m_playerList[i])
			return false;
	}

	return true;
}

Agent * Game::addAgent(const std::string & _name)
{
	Agent * ret = m_world.addAgent();

	ret->setName(_name);

	return ret;
}
Agent * Game::addAgent(const std::string & _name, int _index)
{
	Agent * ret = m_world.addAgent(_index);

	ret->setName(_name);

	return ret;
}

const string & Game::getName() const
{
	return m_name;
}
void Game::setName(string & _name)
{
	m_name = _name;
}

const World & Game::getWorld() const
{
	return m_world;
}
