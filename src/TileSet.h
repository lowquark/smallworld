#ifndef TILESET_H
#define TILESET_H

#include <DL/DEngine.h>
#include <DL/Geom.h>

#include <string>

class TileSet : public dl::Texture
{
  public:

	TileSet();
	TileSet(const std::string & _path);

	dl::FloatRect getTile(int _x, int _y) const;
	dl::FloatRect getTile(int _index) const;

	dl::FloatRect getTile(dl::IntRect _pixels) const;
};

#endif
