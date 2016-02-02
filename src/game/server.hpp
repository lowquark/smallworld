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

// Adds a zone to the server, addressed by name
void AddZone(const std::string & name, Zone * zone, bool pri = false);

// Creates a player for the server, adds them to primary zone
void CreatePlayer(const std::string & name, View * view, bool spectate = false);

void Tick();

};

}

#endif
