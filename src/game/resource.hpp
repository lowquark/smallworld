#ifndef GAME_RESOURCE_HPP
#define GAME_RESOURCE_HPP

#include <cstdint>

namespace game
{

namespace resource
{

enum Type { NONE = 0, TEXTURE, SPRITE, MESH, SOUND };

typedef uint32_t Id;

// A texture is an image data resource
struct Texture
{
  uint8_t * data;
  unsigned int width;
  unsigned int height;

  Texture();
};

// A sprite is a collection of frames, and a reference to texture
struct Sprite
{
  Id texture_id;
  uint16_t tiles_w, tiles_h;

  Sprite();
};

// A mesh is a collection of vertices/faces, and a reference to texture
struct Mesh
{
  Id texture_id;

  Mesh();
};

// A sound is a compressed set of samples
struct Sound
{
  Sound();
};

}

}

#endif
