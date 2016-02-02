#ifndef GAME_ZONE_HPP
#define GAME_ZONE_HPP

#include <string>
#include <vector>
#include <deque>
#include <map>

#include <game/resource.hpp>
#include <game/Player.hpp>

namespace game
{

typedef uint32_t Id;

struct Agent
{
  struct Mesh
  {
    resource::Id id;
    uint32_t animation;
  };
  struct Body
  {
    float w, l, h;
    float x, y, z;
    float dx, dy, dz;
  };

  std::string name;
  Mesh mesh;
  Body body;
};

class View
{
  public:
  virtual void SetAgentMesh(Id eid, const Agent::Mesh & mesh);
  virtual void SetAgentBody(Id eid, const Agent::Body & body);
  virtual void DestroyAgent(Id eid);
  virtual void EnterZone();
  virtual void LeaveZone();
};

class GameState
{
  std::map<Id, Agent> agents;

  public:
  GameState();

  void FullUpdate(View * view) const;
  void SignalUpdates(View * view) const;
  void ClearUpdates();

  void SetAgentMesh(Id agent_id, const Agent::Mesh & mesh);
  void SetAgentBody(Id agent_id, const Agent::Body & body);
};

struct Command
{
  enum { INPUT } type;
  union {
    struct {
      float dx, dy;
    } input;
  };
};

struct Player
{
  Id id;
  std::string name;
  View * view;
  bool spectator;
};

// A Zone manages agents and their environment
//
// May be thought of as an I/O model, where player input goes in,
// and updates come out.
class Zone
{
  GameState game_state;

  public:
  Zone(const std::string & script);
  ~Zone();

  void AddPlayer(Player * player);
  void RemovePlayer(Player * player);
  void PlayerCommand(Player * player, const Command & cmd);

  struct Transfer
  {
    std::string message;
  };
  bool PollTransfer(Transfer * transfer);

  void Tick();

  private:
  std::vector<Player *> players;
  std::deque<Transfer> transfers;

  virtual void OnPlayerEnter(Id player_id);
  virtual void OnPlayerLeave(Id player_id);
  virtual void OnPlayerCommand(Id player_id, const Command & cmd);
};

}

#endif
