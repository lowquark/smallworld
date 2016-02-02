#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>

class Tile
{
	int m_id;
	float m_height;

  public:

	Tile();
	Tile(int _id, float _height);

	int getId() const;
	void setId(int _id);

	float getHeight() const;
	void setHeight(float _height);
};

class Chunk
{
	Tile ** m_tileMap;

	Chunk * m_north, * m_east, * m_south, * m_west;

	Chunk * findChunk(int & _x, int & _y) const; //Returns the neighbor in the direction of the overflow

  public:

	static const int SIZE = 16;

	Chunk();
	~Chunk();

	bool validTile(int _x, int _y) const;

	const Tile getTile(int _x, int _y) const;
	void setTile(int _x, int _y, const Tile & _tile);

	Chunk * getNorthNeighbor() const;
	Chunk * getEastNeighbor() const;
	Chunk * getSouthNeighbor() const;
	Chunk * getWestNeighbor() const;

	void setNorthNeighbor(Chunk * _north);
	void setEastNeighbor(Chunk * _east);
	void setSouthNeighbor(Chunk * _south);
	void setWestNeighbor(Chunk * _west);
};

class Terrain
{
	int m_width, m_length; //Chunks in the x and y directions

	Chunk * ** m_chunkMap;

  public:

	Terrain(int _width, int _length);
	~Terrain();

	void clear();

	bool validTile(int _x, int _y) const;
	bool validChunk(int _x, int _y) const;

	const Tile getTile(int _x, int _y) const;
	void setTile(int _x, int _y, const Tile & _tile);
	Chunk * getChunk(int _x, int _y) const;

	int getWidth() const;
	int getLength() const;
	void setSize(int _width, int _length);
};

#endif
