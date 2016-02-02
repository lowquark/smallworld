#ifndef TILESET_H
#define TILESET_H

#include <DL/DEngine/Texture.h>
#include <DL/Geom.h>

#include <string>

#include <iostream>

class TileSet : public dl::Texture
{
	int m_tileSize;

  public:

	TileSet();
	TileSet(const std::string & _path);

	dl::FloatRect getTile(int _x, int _y) const;
	dl::FloatRect getTile(int _index) const;

	dl::FloatRect getTile(dl::IntRect _pixels) const;

	int getTileSize();
	void setTileSize(int _tileSize);
};

#endif
