#ifndef GAME_HPP
#define GAME_HPP

#include <string>

#include <game/Zone.hpp>

namespace game
{

resource::Id LoadTexture(const std::string & path);
resource::Id LoadMesh(const std::string & path);

namespace server
{

void Init();
void Open(uint16_t port);
void Deinit();

void AddZone(const std::string & name, Zone * zone, bool pri = false);

void CreatePlayer(const std::string & zone_name, View * view, bool spectate = false);

void Tick();

};

}

#endif
