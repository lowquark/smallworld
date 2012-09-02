#ifndef RGAME_H
#define RGAME_H

#include "Game.h"
#include "Sprite3D.h"
#include "TileSet.h"

#include "ResourceManager.h"

#include <DL/DEngine.h>
#include <SDL/SDL.h>

#include <iostream>
#include <stdexcept>

class GameRenderer
{
	dl::Scene m_scene;
	dl::Camera m_camera;
	dl::Viewport m_viewport;

	const Game & m_game; //Game to render

	const Agent * m_followedAgent; //Agent followed by camera

	TileSet m_tileSet; //Master tileSet (subject to change)
	dl::Texture m_agentTexture; // ""

	int m_counter; //Counts every tick from 0 to 60 and loops
	bool m_stepping; //Whether to make the agents step or not


	dl::Point3D getFollowCameraPos(const Agent * _agent) const;


	void renderAxis(float _length, dl::Point3D _pos = dl::Point3D(0, 0, 0)) const;
	void renderGrid(int _size) const;

	void renderTerrain() const;

	void renderAgents() const;
	void renderAgent(const Agent * _agent) const;

	void loadTextures();

  public:

	GameRenderer(const Game & _game);
	~GameRenderer();

	void createWindow(int _width, int _height, bool _fullscreen);

	void tick();
	void render() const;

	void follow(const Agent * _agent, bool _smoothTransition = false);

	const Agent * getFollowedAgent() const;
	void setFollowedAgent(const Agent * _followedAgent);
};

#endif
