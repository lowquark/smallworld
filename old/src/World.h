#ifndef WORLD_H
#define WORLD_H

#include "Agent.h"
#include "Terrain.h"

#include <vector>
#include <iostream>

class World
{
	std::vector<Agent *> m_agentList;

	Terrain m_terrain;

  public:

	World();
	~World();

	void tick();

	void updateAgent(const packet::Agent * _agentPacket);

	Agent * addAgent();												//Adds an agent
	Agent * addAgent(int _id);										//Adds an agent at a particular index
	Agent * getAgent(int _id) const;
	int getAgentId(const Agent * _agent) const;						//Returns the index of a given agent.  Returns -1 if not found.
	const std::vector<Agent *> & getAgentList() const;
	void clearAgents();

	const Terrain & getTerrain() const;
};

#endif
