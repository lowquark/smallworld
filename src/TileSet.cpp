#include "TileSet.h"

TileSet::TileSet() : Texture()
{
}
TileSet::TileSet(const std::string & _path) : Texture(_path)
{
}

dl::FloatRect TileSet::getTile(int _x, int _y) const
{
}
dl::FloatRect TileSet::getTile(int _index) const
{
}

dl::FloatRect TileSet::getTile(dl::IntRect _pixels) const
{
	dl::FloatRect ret;

	ret.x = (float)_pixels.x/getWidth() + .001;
	ret.y = (float)_pixels.y/getHeight() + .001;

	ret.width = (float)_pixels.width/getWidth() - .002;
	ret.height = (float)_pixels.height/getHeight() - .002;

	return ret;
}
