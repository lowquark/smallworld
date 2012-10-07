#include "Terrain.h"

using namespace std;

Tile::Tile(int _id, float _height) : m_id(_id), m_height(_height)
{
}

int Tile::getId()
{
	return m_id;
}
void Tile::setId(int _id)
{
	m_id = _id;
}

float Tile::getHeight()
{
	return m_height;
}
void Tile::setHeight(float _height)
{
	m_height = _height;
}


Terrain::Terrain(int _width, int _length) : m_width(0), m_length(0), m_tileMap(0)
{
	setSize(_width, _length);
}
Terrain::~Terrain()
{
	clear();
}

void Terrain::setSize(int _width, int _length)
{
	clear();

	m_tileMap = new Tile * * [_width];
	for(int i = 0;i < _width;i ++)
	{
		m_tileMap[i] = new Tile * [_length];
		for(int j = 0;j < _length;j ++)
		{
			m_tileMap[i][j] = new Tile(i/4, /*(float)i/10 + (float)j/10*/0);
		}
	}

	m_width = _width;
	m_length = _length;
}
void Terrain::clear()
{
	if(m_tileMap)
	{
		for(int i = 0;i < m_width;i ++)
		{
			for(int j = 0;j < m_length;j ++)
			{
				if(m_tileMap[i][j])
				{
					delete m_tileMap[i][j];
				}
			}

			delete m_tileMap[i];
		}

		delete m_tileMap;

		m_width = 0;
		m_length = 0;
	}
}

bool Terrain::validTile(int _x, int _y) const
{
	if(_x >= 0 && _x < m_width &&
	   _y >= 0 && _y < m_length)
	{
		return true;
	}

	return 0;
}

Tile * Terrain::getTile(int _x, int _y) const
{
	if(validTile(_x, _y))
	{
		return m_tileMap[_x][_y];
	}

	return 0;
}

float Terrain::getTileHeight(int _x, int _y) const
{
	if(Tile * tile = getTile(_x, _y))
	{
		return tile->getHeight();
	}

	return 0;
}
void Terrain::setTileHeight(int _x, int _y, float _height)
{
	if(validTile(_x, _y))
	{
		m_tileMap[_x][_y]->setHeight(_height);
	}
}

int Terrain::getWidth() const
{
	return m_width;
}
int Terrain::getLength() const
{
	return m_length;
}
