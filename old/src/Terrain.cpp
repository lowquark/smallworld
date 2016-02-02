#include "Terrain.h"

using namespace std;

Tile::Tile() : m_id(0), m_height(0)
{
}
Tile::Tile(int _id, float _height) : m_id(_id), m_height(_height)
{
}

int Tile::getId() const
{
	return m_id;
}
void Tile::setId(int _id)
{
	m_id = _id;
}

float Tile::getHeight() const
{
	return m_height;
}
void Tile::setHeight(float _height)
{
	m_height = _height;
}


Chunk * Chunk::findChunk(int & _x, int & _y) const
{
	if(_x >= SIZE)
	{
		_x -= SIZE;
		return m_east->findChunk(_x, _y);
	}
	if(_x < 0)
	{
		_x += SIZE;
		return m_west;
	}
	if(_y >= SIZE)
	{
		_y -= SIZE;
		return m_north;
	}
	if(_y < 0)
	{
		_y += SIZE;
		return m_south;
	}

	return (Chunk *)this;
}

Chunk::Chunk() : m_tileMap(0), m_north(0), m_east(0), m_south(0), m_west(0)
{
	m_tileMap = new Tile * [SIZE];
	for(int i = 0;i < SIZE;i ++)
	{
		m_tileMap[i] = new Tile[SIZE];

		for(int j = 0;j < SIZE;j ++)
		{
			m_tileMap[i][j].setHeight((float)j/10 + (float)i/10);
		}
	}
}
Chunk::~Chunk()
{
	for(int i = 0;i < SIZE;i ++)
	{
		delete m_tileMap[i];
	}

	delete m_tileMap;
}

bool Chunk::validTile(int _x, int _y) const
{
	if(_x >= 0 && _x < SIZE &&
	   _y >= 0 && _y < SIZE)
	{
		return true;
	}

	return false;
}

const Tile Chunk::getTile(int _x, int _y) const
{
	if(validTile(_x, _y))
	{
		return m_tileMap[_x][_y];
	}

	return Tile(0, 0);
}
void Chunk::setTile(int _x, int _y, const Tile & _tile)
{
	if(validTile(_x, _y))
	{
		m_tileMap[_x][_y] = _tile;
	}
}

Chunk * Chunk::getNorthNeighbor() const
{
	return m_north;
}
Chunk * Chunk::getEastNeighbor() const
{
	return m_east;
}
Chunk * Chunk::getSouthNeighbor() const
{
	return m_south;
}
Chunk * Chunk::getWestNeighbor() const
{
	return m_west;
}

void Chunk::setNorthNeighbor(Chunk * _north)
{
	m_north = _north;
}
void Chunk::setEastNeighbor(Chunk * _east)
{
	m_east = _east;
}
void Chunk::setSouthNeighbor(Chunk * _south)
{
	m_south = _south;
}
void Chunk::setWestNeighbor(Chunk * _west)
{
	m_west = _west;
}



Terrain::Terrain(int _width, int _length) : m_width(0), m_length(0), m_chunkMap(0)
{
	setSize(_width, _length);
}
Terrain::~Terrain()
{
	clear();
}

void Terrain::clear()
{
	if(m_chunkMap)
	{
		for(int i = 0;i < m_width;i ++)
		{
			for(int j = 0;j < m_length;j ++)
			{
				if(m_chunkMap[i][j])
				{
					delete m_chunkMap[i][j];
				}
			}

			delete m_chunkMap[i];
		}

		delete m_chunkMap;

		m_width = 0;
		m_length = 0;
	}
}

bool Terrain::validChunk(int _x, int _y) const
{
	if(_x >= 0 && _x < m_width &&
	   _y >= 0 && _y < m_length)
	{
		return true;
	}

	return false;
}

const Tile Terrain::getTile(int _x, int _y) const
{
	int chunkX = _x / Chunk::SIZE;
	int chunkY = _y / Chunk::SIZE;

	if(validChunk(chunkX, chunkY))
	{
		return m_chunkMap[chunkX][chunkY]->getTile(_x % Chunk::SIZE, _y % Chunk::SIZE);
	}

	return Tile(0, 0);
}
void Terrain::setTile(int _x, int _y, const Tile & _tile)
{
	if(validChunk(_x, _y))
	{
		m_chunkMap[_x][_y]->setTile(_x % Chunk::SIZE, _y % Chunk::SIZE, _tile);
	}
}
Chunk * Terrain::getChunk(int _x, int _y) const
{
	if(validChunk(_x, _y))
	{
		return m_chunkMap[_x][_y];
	}

	return 0;
}

int Terrain::getWidth() const
{
	return m_width;
}
int Terrain::getLength() const
{
	return m_length;
}
void Terrain::setSize(int _width, int _length)
{
	clear();

	m_chunkMap = new Chunk * * [_width];
	for(int i = 0;i < _width;i ++)
	{
		m_chunkMap[i] = new Chunk * [_length];
		for(int j = 0;j < _length;j ++)
		{
			m_chunkMap[i][j] = new Chunk();
		}
	}

	m_width = _width;
	m_length = _length;
}
