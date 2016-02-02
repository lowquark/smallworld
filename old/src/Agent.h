#ifndef AGENT_H
#define AGENT_H

#include "Packet.h"
#include "Terrain.h"

#include <DL/Geom.h>

#include <iostream>

class Agent;

namespace packet
{
	struct Agent : public Base
	{
		static const int PACKETID = 14;

		int agentId;
		char name[maxNameLength + 1];

		float mass;
		float maxSpeed;

		float health;
		float maxHealth;

		dl::Point3D position;
		dl::Vector3D velocity;
		dl::Vector2D direction;

		Agent(const ::Agent * _agent);
		void update(::Agent * _agent) const;
	};
}

class AgentController
{
  protected:

	Agent * m_agent;

  public:

	AgentController();

	virtual void tick() const = 0;

	Agent * getAgent();
	void setAgent(Agent * _agent);
};

class Agent
{
	friend struct packet::Agent;

  protected:

	int m_id;

	//Properties
	std::string m_name;

	float m_mass;				//Mass in kg
	float m_maxSpeed;			//The maximum horizontal speed of the Agent

	bool m_onGround;

	float m_health;
	float m_maxHealth;

	//Moving
	dl::Point3D m_position;
	dl::Vector3D m_velocity;	//The current velocity of the Agent
	dl::Vector2D m_direction;	//A normal vector representing the horizontal direction last *pushed* in

	const AgentController * m_controller;

  public:

	Agent(int _id = -1);

	static float dt;				//1/60th of a second
	static dl::Vector3D gravity;	//Gravity

	void tick(const Terrain & _terrain);

	void push(const dl::Vector3D _force);	//Accelerates based on the force(N) and weight(kg)
	void dampen(float _force);				//Applies a force in the opposite direction of horizontal motion
	void jump(float _impulse);				//Applies an impulse in the z direction if on ground

	void heal();							//Heals to maximum health
	void heal(float _amount);				//Heals damage
	void hurt(float _damage);				//Deals damage
	void kill();							//Guaranteed kill

	bool isDead() const;					//Returns whether dead or not


	int getId() const;

	const std::string & getName() const;
	void setName(const std::string & _name);

	float getMass() const;
	void setMass(float _mass);

	float getMaxSpeed() const;
	void setMaxSpeed(float _maxSpeed);


	float getHealth() const;
	float getHealthPerc() const;
	void setHealth(float _health);

	float getMaxHealth() const;
	void setMaxHealth(float _maxHealth);


	const dl::Point3D & getPosition() const;
	void setPosition(const dl::Point3D & _position);

	const dl::Vector3D & getVelocity() const;
	void setVelocity(const dl::Vector3D & _velocity);

	const dl::Vector2D & getDirection() const;

	const AgentController * getController() const;
	void setController(const AgentController * _controller);
};

#endif
