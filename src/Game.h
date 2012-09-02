#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "Player.h"

#include <vector>
#include <sstream>

class Game
{
  public:

	Game();
	~Game();

	static const int MAX_PLAYERS = 16;

	void tick();

	void updatePlayer(const packet::Player * _playerPacket); //Updates the given player. If not found, creates a player.
	void updateAgent(const packet::Agent * _agentPacket); //Updates the given agent. If not found, creates an agent.

	Player * getPlayer(int _id);
	Player * addPlayer(const std::string & _name);
	Player * addPlayer(const std::string & _name, int _index);
	bool removePlayer(const Player * _player);
	bool isFull() const;

	Agent * addAgent(const std::string & _name);
	Agent * addAgent(const std::string & _name, int _index);

	const std::string & getName() const;
	void setName(std::string & _name);

	const World & getWorld() const;

  private:

	std::string m_name;

	World m_world;

	Player * m_playerList[MAX_PLAYERS];
};

#endif
