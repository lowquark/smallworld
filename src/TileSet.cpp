#include "TileSet.h"

TileSet::TileSet() : Texture()
{
}
TileSet::TileSet(const std::string & _path) : Texture(_path)
{
}

dl::FloatRect TileSet::getTile(int _x, int _y) const
{
	int x = _x*m_tileSize;
	int y = _y*m_tileSize;

	return getTile(dl::IntRect(x, y, m_tileSize, m_tileSize));
}
dl::FloatRect TileSet::getTile(int _index) const
{
	//Tiles in the x direction
	int tilesX = getWidth()/m_tileSize;

	int x = _index % tilesX;
	int y = _index / tilesX;

	return getTile(x, y);
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

int TileSet::getTileSize()
{
	return m_tileSize;
}
void TileSet::setTileSize(int _tileSize)
{
	m_tileSize = _tileSize;
}
