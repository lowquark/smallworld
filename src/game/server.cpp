
#include <game/server.hpp>

#include <vector>
#include <map>

#include <enet/enet.h>

namespace game
{

resource::Id LoadTexture(const std::string & path)
{
  return 0;
}
resource::Id LoadMesh(const std::string & path)
{
  return 0;
}

namespace server
{

std::map<std::string, Zone *> zones;
Zone * default_zone = 0;

std::vector<Player *> players;

ENetHost * host = 0;

void Init()
{
}
void Open(uint16_t port)
{
  enet_initialize();

  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = port;

  host = enet_host_create(&address, 32, 2, 0, 0);

  if(!host)
    printf("Failed to create enet host.\n");
}
void Deinit()
{
  zones.clear();

  for(auto & player : players) {
    delete player;
    player = 0;
  }

  enet_host_destroy(host);
  host = 0;
  enet_deinitialize();
}

void AddZone(const std::string & name, Zone * zone, bool pri)
{
  if(zone)
    zones[name] = zone;
  else
    zones.erase(name);

  if(pri)
    default_zone = zone;
}

void CreatePlayer(const std::string & name, View * view, bool spectate)
{
  Player * player = new Player;
  player->id = 0;
  player->name = name;
  player->view = view;
  player->spectator = spectate;
  default_zone->AddPlayer(player);
}

void Tick()
{
  if(host)
  {
    ENetEvent event;
    while(enet_host_service(host, &event, 0))
    {
      if(event.type == ENET_EVENT_TYPE_CONNECT)
      {
        printf("ENET_EVENT_TYPE_CONNECT\n");
      }
      else if(event.type == ENET_EVENT_TYPE_DISCONNECT)
      {
        printf("ENET_EVENT_TYPE_DISCONNECT\n");
      }
      else if(event.type == ENET_EVENT_TYPE_RECEIVE)
      {
        printf("ENET_EVENT_TYPE_RECEIVE\n");
        enet_packet_destroy(event.packet);
      }
    }
  }

  for(auto & kvpair : zones)
  {
    auto & zone = kvpair.second;
    zone->Tick();
  }
}

}

}

