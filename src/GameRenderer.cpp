#include "GameRenderer.h"

using namespace std;


dl::Point3D GameRenderer::getFollowCameraPos(const Agent * _agent) const
{
	return _agent->getPosition() + dl::Vector3D(0, -5, 5);
}


void GameRenderer::renderAxis(float _length, dl::Point3D _pos) const
{
	glLineWidth(2);

	glTranslatef(_pos.x, _pos.y, _pos.z);

		//x axis
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);
			glVertex3f( 0.0, 0.0, 0.0);
			glVertex3f(_length, 0.0, 0.0);
		glEnd();

		//y axis
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
			glVertex3f(0.0,  0.0, 0.0);
			glVertex3f(0.0, _length, 0.0);
		glEnd();

		//z axis
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0,  0.0);
			glVertex3f(0.0, 0.0, _length);
		glEnd();

	glTranslatef(-_pos.x, -_pos.y, -_pos.z);
}
void GameRenderer::renderGrid(int _size) const
{
	glLineWidth(1);
	glColor3f(.4, .4, .4);

	glBegin(GL_LINES);

	for(int i = -_size;i <= _size;i ++)
	{
		glVertex3f(-_size, i, 0.0);
		glVertex3f( _size, i, 0.0);

		glVertex3f(i, -_size, 0.0);
		glVertex3f(i,  _size, 0.0);
	}

	glEnd();
}

void GameRenderer::renderTileLeftSide(const Tile * _tileA, const Tile * _tileB) const
{
	float heightDiff = _tileA->getHeight() - _tileB->getHeight();

	glColor3f(1.0, 1.0, 1.0);

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

		glVertex3f(0.0, 0.0,  0.0);
		glVertex3f(0.0, 0.0, -heightDiff);
		glVertex3f(0.0, 1.0, -heightDiff);
		glVertex3f(0.0, 1.0,  0.0);

	glEnd();

	glEnable(GL_TEXTURE_2D);
}
void GameRenderer::renderTileFrontSide(const Tile * _tileA, const Tile * _tileB) const
{
	float heightDiff = _tileA->getHeight() - _tileB->getHeight();

	glColor3f(1.0, 1.0, 1.0);

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

		glVertex3f(0.0, 0.0,  0.0);
		glVertex3f(0.0, 0.0, -heightDiff);
		glVertex3f(1.0, 0.0, -heightDiff);
		glVertex3f(1.0, 0.0,  0.0);

	glEnd();

	glEnable(GL_TEXTURE_2D);
}
void GameRenderer::renderTileRightSide(const Tile * _tileA, const Tile * _tileB) const
{
	float heightDiff = _tileA->getHeight() - _tileB->getHeight();

	glColor3f(1.0, 1.0, 1.0);

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

		glVertex3f(1.0, 0.0,  0.0);
		glVertex3f(1.0, 0.0, -heightDiff);
		glVertex3f(1.0, 1.0, -heightDiff);
		glVertex3f(1.0, 1.0,  0.0);

	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void GameRenderer::renderTile(float _x, float _y, const Tile * _tile, const Tile * _leftNeighbor, const Tile * _frontNeighbor, const Tile * _rightNeighbor) const
{
	dl::FloatRect rect(m_tileSet.getTile(_tile->getId()));

	glTranslatef(_x, _y, _tile->getHeight());

	glBegin(GL_QUADS);
		glTexCoord2f(rect.x,              rect.y              ); glVertex3f(0.0, 1.0, 0.0);
		glTexCoord2f(rect.x,              rect.y + rect.height); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(rect.x + rect.width, rect.y + rect.height); glVertex3f(1.0, 0.0, 0.0);
		glTexCoord2f(rect.x + rect.width, rect.y              ); glVertex3f(1.0, 1.0, 0.0);
	glEnd();

	if(_leftNeighbor && _tile->getHeight() > _leftNeighbor->getHeight())
		renderTileLeftSide(_tile, _leftNeighbor);

	if(_frontNeighbor && _tile->getHeight() > _frontNeighbor->getHeight())
		renderTileFrontSide(_tile, _frontNeighbor);

	if(_rightNeighbor && _tile->getHeight() > _rightNeighbor->getHeight())
		renderTileRightSide(_tile, _rightNeighbor);

	glTranslatef(-_x, -_y, -_tile->getHeight());
}

void GameRenderer::renderTerrain() const
{
	m_tileSet.bind();

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	for(int i = 0;i < m_game.getWorld().getTerrain().getWidth();i ++)
	{
		for(int j = 0;j < m_game.getWorld().getTerrain().getLength();j ++)
		{
			Tile * tile = m_game.getWorld().getTerrain().getTile(i, j);

			if(tile)
			{
				renderTile(i, j, tile, m_game.getWorld().getTerrain().getTile(i - 1, j    ),
									   m_game.getWorld().getTerrain().getTile(i,     j - 1),
									   m_game.getWorld().getTerrain().getTile(i + 1, j    ));
			}
		}
	}

	glDisable(GL_TEXTURE_2D);
}

void GameRenderer::renderAgents() const
{
	for(auto & agent : m_game.getWorld().getAgentList())
	{
		if(agent)
		{
			renderAgent(agent);
		}
	}
}
void GameRenderer::renderAgent(const Agent * _agent) const
{
	dl::Point3D pos = _agent->getPosition();

	//Draw the guy
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);

	m_agentTexture.bind();

	glTranslatef(pos.x, pos.y, pos.z);

		Sprite3D sprite;

		if((dl::Vector2D)_agent->getVelocity() != dl::Vector2D(0, 0))
			sprite.setStepping(m_stepping);

		if(_agent->getDirection().i > 0)
			if(_agent->getDirection().j > 0)
				sprite.setOrientation(Sprite3D::RIGHT_BACK);
			else
				sprite.setOrientation(Sprite3D::RIGHT_FRONT);
		else
			if(_agent->getDirection().j > 0)
				sprite.setOrientation(Sprite3D::LEFT_BACK);
			else
				sprite.setOrientation(Sprite3D::LEFT_FRONT);

		sprite.render();

	glTranslatef(-pos.x, -pos.y, -pos.z);

	glDisable(GL_TEXTURE_2D);
}

void GameRenderer::loadTextures()
{
	m_tileSet.load("res/local/spritesheet.png");
	m_tileSet.setTileSize(10);
	m_agentTexture.load("res/local/agent.png");
}


GameRenderer::GameRenderer(const Game & _game) : m_camera(dl::Point3D(0, -10, 10), dl::Point3D(0, 0, 0)), 
												 m_viewport(640, 480, 70, .01, 1000), 
												 m_game(_game), 
												 m_counter(0), 
												 m_stepping(false),
												 m_terrainDLID(0)
{
	SDL_Init(0);
}
GameRenderer::~GameRenderer()
{
	SDL_Quit();
}

void GameRenderer::createWindow(int _width, int _height, bool _fullscreen)
{
	//Make an SDL window supporting OpenGL
	SDL_SetVideoMode(_width, _height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL | (_fullscreen ? SDL_FULLSCREEN : 0));

	SDL_WM_SetCaption("smallworld", "sw");

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.7);

	m_viewport.width = _width;
	m_viewport.height = _height;

	loadTextures();
}

void GameRenderer::tick()
{
	//Follow the specified agent
	if(m_followedAgent)
	{
		m_camera.setTarget(getFollowCameraPos(m_followedAgent), m_followedAgent->getPosition()); //Point at the agent
	}

	m_camera.tick();

	m_counter ++;

	if(m_counter == 60)
		m_counter = 0;

	m_stepping = 10/(m_counter % 20 + 1);
}
void GameRenderer::render()
{
	float r = (float)0x87/0xFF;
	float g = (float)0xCE/0xFF;
	float b = (float)0xFA/0xFF;

	glClearColor(r, g, b, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_scene.render(m_camera, m_viewport);

	renderAgents();

	//renderAxis(10);
	//renderGrid(10);

	if(!m_terrainDLID)
	{
		m_terrainDLID = glGenLists(1);

		glNewList(m_terrainDLID, GL_COMPILE_AND_EXECUTE);
		renderTerrain();
		glEndList();
	}
	else
	{
		glCallList(m_terrainDLID);
	}

	SDL_GL_SwapBuffers();
}

const Agent * GameRenderer::getFollowedAgent() const
{
	return m_followedAgent;
}
void GameRenderer::setFollowedAgent(const Agent * _followedAgent)
{
	m_followedAgent = _followedAgent;
}
