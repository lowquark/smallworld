
#include "resource.hpp"

namespace game
{

namespace resource
{

Texture::Texture()
  : data(0),
    width(0),
    height(0)
{
}
Sprite::Sprite()
  : texture_id(0),
    tiles_w(0),
    tiles_h(0)
{
}
Mesh::Mesh()
  : texture_id(0)
{
}
Sound::Sound()
{
}

}

}
