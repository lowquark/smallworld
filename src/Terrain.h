#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>

class Tile
{
	int m_id;
	float m_height;

  public:

	Tile(int _id, float _height = 0);

	int getId() const;
	void setId(int _id);

	float getHeight() const;
	void setHeight(float _height);
};

class Terrain
{
	int m_width, m_length;

	Tile * ** m_tileMap;

  public:

	Terrain(int _width, int _length);
	~Terrain();

	void setSize(int _width, int _length);
	void clear();

	bool validTile(int _x, int _y) const;

	Tile * getTile(int _x, int _y) const;

	float getTileHeight(int _x, int _y) const;
	void setTileHeight(int _x, int _y, float _height);

	int getWidth() const;
	int getLength() const;
};

#endif
