#include "World.h"

using namespace std;


World::World() : m_terrain(2, 1)
{
}
World::~World()
{
	clearAgents();
}

void World::tick()
{
	for(auto & agentA : m_agentList)
	{
		for(auto & agentB : m_agentList)
		{
			if(agentA != agentB)
			{
				dl::Vector3D aToB = dl::Vector3D(agentB->getPosition() - agentA->getPosition());

				float dist = aToB.magnitude();

				if(dist < .65 && dist != 0)
				{
					float force = 2000/dist;
					agentA->push(dl::Vector2D(aToB.normal()*-force));
					agentB->push(dl::Vector2D(aToB.normal()*force));
				}
			}
		}
	}

	for(auto & agent : m_agentList)
	{
		//if(agent)
			agent->tick(m_terrain);
	}
}

void World::updateAgent(const packet::Agent * _agentPacket)
{
	Agent * agent = getAgent(_agentPacket->agentId);

	if(!agent)
	{
		agent = addAgent(_agentPacket->agentId); //Will only add if it's not already there
	}
	else
	{
		_agentPacket->update(agent);
	}
}

Agent * World::addAgent()
{
	//Look for a hole in the array first
	for(auto agentIt = m_agentList.begin();agentIt != m_agentList.end();agentIt ++)
	{
		if(Agent * agent = addAgent(agentIt - m_agentList.begin()))
			return agent;
	}

	//If that failed, push it on the back
	return addAgent(m_agentList.size());
}
Agent * World::addAgent(int _id)
{
	//Make sure the array is big enough
	if(_id >= m_agentList.size())
		m_agentList.resize(_id + 1);

	//Only add if it's not already there
	if(!m_agentList[_id])
	{
		m_agentList[_id] = new Agent(_id); //Do the assigning

		//m_agentList[_id]->createLuaInstance();

		return m_agentList[_id];
	}

	return 0;
}
Agent * World::getAgent(int _id) const
{
	if(_id >= 0 && _id < m_agentList.size())
	{
		return m_agentList[_id];
	}

	return 0;
}
int World::getAgentId(const Agent * _agent) const
{
	//Search for it
	for(auto agentIt = m_agentList.begin();agentIt != m_agentList.end();agentIt ++)
	{
		if(_agent == *agentIt)
		{
			return agentIt - m_agentList.begin();
		}
	}

	//Not found
	return -1;
}
const std::vector<Agent *> & World::getAgentList() const
{
	return m_agentList;
}
void World::clearAgents()
{
	while(!m_agentList.empty())
	{
		if(m_agentList.back())
			delete m_agentList.back();

		m_agentList.pop_back();
	}
}

const Terrain & World::getTerrain() const
{
	return m_terrain;
}
