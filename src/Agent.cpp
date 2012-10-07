#include "Agent.h"

using namespace std;


packet::Agent::Agent(const ::Agent * _agent) : packet::Base(PACKETID)
{
	agentId = _agent->m_id;

	strncpy(name, _agent->m_name.c_str(), packet::maxNameLength);
	name[packet::maxNameLength] = '\0';

	mass = _agent->m_mass;
	maxSpeed = _agent->m_maxSpeed;

	health = _agent->m_health;
	maxHealth = _agent->m_maxHealth;

	position = _agent->m_position;
	velocity = _agent->m_velocity;
	direction = _agent->m_direction;
}
void packet::Agent::update(::Agent * _agent) const
{
	//cout << "packet::Agent::update(): _agent->getId(): " << _agent->getId() << endl;
	_agent->m_name = name;

	_agent->m_mass = mass;
	_agent->m_maxSpeed = maxSpeed;

	_agent->m_health = health;
	_agent->m_maxHealth = maxHealth;

	dl::Vector3D diff = position - _agent->m_position;
	if(diff.magnitude() > .25)
		_agent->m_position = position;

	_agent->m_velocity = velocity;
	_agent->m_direction = direction;
}


AgentController::AgentController() : m_agent(0)
{
}

Agent * AgentController::getAgent()
{
	return m_agent;
}
void AgentController::setAgent(Agent * _agent)
{
	m_agent = _agent;

	_agent->setController(this);
}


Agent::Agent(int _id) : m_id(_id),
						m_name("Unnamed Agent"),
						m_mass(200),
						m_maxSpeed(4),
						m_onGround(false),
						m_health(100),
						m_maxHealth(100),
						m_controller(0)
{
}

float Agent::dt((float)1/60);
dl::Vector3D Agent::gravity(0, 0, (float)-26);

void Agent::tick(const Terrain & _terrain)
{
	if(m_controller)
		m_controller->tick();
	else
		dampen(5000); //Default controller action

	m_velocity += gravity*dt;

	dl::Vector2D horizVelocity = m_velocity;

	//Regulate speed to the maximum
	if(horizVelocity.magnitude() > m_maxSpeed)
	{
		horizVelocity = horizVelocity.normalize()*m_maxSpeed;
		m_velocity = dl::Vector3D(horizVelocity.i, horizVelocity.j, m_velocity.k);
	}

	m_position += m_velocity*dt;

	/*
	if(m_position.x > 10) {m_position.x = 10;}
	if(m_position.x < -10) {m_position.x = -10;}
	if(m_position.y > 10) {m_position.y = 10;}
	if(m_position.y < -10) {m_position.y = -10;}
	*/

	float groundHeight = -100;

	if(Tile * tile = _terrain.getTile(floor(m_position.x), floor(m_position.y)))
	{
		groundHeight = tile->getHeight();
	}

	if(m_position.z < groundHeight)
	{
		m_position.z = groundHeight;
		m_velocity.k = 0;

		m_onGround = true;
	}
	else
	{
		m_onGround = false;
	}
}

void Agent::push(const dl::Vector3D _force)
{
	//Add to the velocity
	m_velocity += (_force/m_mass)*dt;

	dl::Vector2D horizVelocity = m_velocity;

	//Only update the direction if we're moving a certain speed
	if(horizVelocity.magnitude() > 1)
		m_direction = horizVelocity.normal();
}
void Agent::dampen(float _force)
{
	dl::Vector2D horizVelocity = m_velocity;

	//Cuttoff speed
	if(horizVelocity.magnitude() < .2)
	{
		m_velocity.i = 0;
		m_velocity.j = 0;
	}
	else
	{
		dl::Vector2D force = horizVelocity.normal()*-_force;
		push(force); //Push in the opposite direction of motion
	}
}
void Agent::jump(float _impulse)
{
	if(m_onGround)
	{
		//Add to the velocity
		m_velocity.k += _impulse/m_mass;
	}
}

void Agent::heal()
{
	m_health = m_maxHealth;
}
void Agent::heal(float _amount)
{
	m_health += _amount;

	//Make sure the health doesn't exceed the maximum
	if(m_health > m_maxHealth)
	{
		m_health = m_maxHealth;
	}
}
void Agent::hurt(float _damage)
{
	m_health -= _damage;
}
void Agent::kill()
{
	m_health = 0;
}

bool Agent::isDead() const
{
	return m_health <= 0;
}


int Agent::getId() const
{
	return m_id;
}

const std::string & Agent::getName() const
{
	return m_name;
}
void Agent::setName(const std::string & _name)
{
	m_name = _name;
}

float Agent::getMass() const
{
	return m_mass;
}
void Agent::setMass(float _mass)
{
	m_mass = _mass;
}

float Agent::getMaxSpeed() const
{
	return m_maxSpeed;
}
void Agent::setMaxSpeed(float _maxSpeed)
{
	m_maxSpeed = _maxSpeed;
}

float Agent::getHealth() const
{
	return m_health;
}
float Agent::getHealthPerc() const
{
	return m_health/m_maxHealth;
}
void Agent::setHealth(float _health)
{
	m_health = _health;
}

float Agent::getMaxHealth() const
{
	return m_maxHealth;
}
void Agent::setMaxHealth(float _maxHealth)
{
	m_maxHealth = _maxHealth;
}


const dl::Point3D & Agent::getPosition() const
{
	return m_position;
}
void Agent::setPosition(const dl::Point3D & _position)
{
	m_position = _position;
}

const dl::Vector3D & Agent::getVelocity() const
{
	return m_velocity;
}
void Agent::setVelocity(const dl::Vector3D & _velocity)
{
	m_velocity = _velocity;
}

const dl::Vector2D & Agent::getDirection() const
{
	return m_direction;
}

const AgentController * Agent::getController() const
{
	return m_controller;
}
void Agent::setController(const AgentController * _controller)
{
	m_controller = _controller;
}
