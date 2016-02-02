
#include <game/Zone.hpp>

#include <stdexcept>

namespace game
{

GameState::GameState()
{
}

void GameState::FullUpdate(View * view) const
{
  printf("Full update!\n");
}
void GameState::SignalUpdates(View * view) const
{
  printf("Some updates!\n");
}
void GameState::ClearUpdates()
{
}

void GameState::SetAgentMesh(Id agent_id, const Agent::Mesh & mesh)
{
}
void GameState::SetAgentBody(Id agent_id, const Agent::Body & body)
{
}

void View::SetAgentMesh(Id eid, const Agent::Mesh & mesh) {}
void View::SetAgentBody(Id eid, const Agent::Body & body) {}
void View::DestroyAgent(Id eid) {}
void View::EnterZone() {}
void View::LeaveZone() {}

Zone::Zone(const std::string & script)
{
  //agents.push_back(new Agent());
}
Zone::~Zone()
{
  for(auto & player : players)
  {
    OnPlayerLeave(player->id);
    player->view->LeaveZone();
  }
}

/*
Agent * Zone::GetAgent(Id agent_id) const
{
  try {
    return agents.at(agent_id);
  } catch(std::out_of_range & e) {
    return 0;
  }
}
Agent * Zone::CreateAgent()
{
  for(int i = 0 ; i < agents.size() ; i ++)
  {
    if(!agents[i])
    {
      agents[i] = new Agent();
      return agents[i];
    }
  }
  agents.push_back(new Agent());
  return agents.back();
}
Agent * Zone::CreateAgent(Id * agent_id)
{
  for(int i = 0 ; i < agents.size() ; i ++)
  {
    if(!agents[i])
    {
      agents[i] = new Agent();
      *agent_id = i;
      return agents[i];
    }
  }
  agents.push_back(new Agent());
  *agent_id = agents.size() - 1;
  return agents.back();
}
void Zone::DestroyAgent(Id agent_id)
{
  try {
    delete agents.at(agent_id);
    agents.at(agent_id) = 0;
  } catch(std::out_of_range & e) {
  }
}
*/

void Zone::AddPlayer(Player * player)
{
  if(!player->spectator)
    OnPlayerEnter(player->id);

  game_state.FullUpdate(player->view);
  player->view->EnterZone();
  players.push_back(player);
}
void Zone::RemovePlayer(Player * player)
{
  for(auto it = players.begin() ; 
      it != players.end() ;
      it ++)
  {
    if(*it == player)
    {
      OnPlayerLeave(player->id);
      player->view->LeaveZone();
      players.erase(it);
      return;
    }
  }
}
void Zone::PlayerCommand(Player * player, const Command & cmd)
{
  OnPlayerCommand(player->id, cmd);
}

bool Zone::PollTransfer(Transfer * transfer)
{
  if(transfers.size())
  {
    *transfer = transfers.front();
    transfers.pop_front();
    return true;
  }
  return false;
}

void Zone::Tick()
{
  Transfer t;
  t.message = "WOOAHH";
  transfers.push_back(t);

  for(auto & player : players)
  {
    game_state.SignalUpdates(player->view);
  }
  game_state.ClearUpdates();
}

void Zone::OnPlayerEnter(Id player_id)
{
  printf("(zone : %p)->PlayerEnter(id : %u)\n", this, player_id);
}
void Zone::OnPlayerLeave(Id player_id)
{
  printf("(zone : %p)->PlayerLeave(id : %u)\n", this, player_id);
}
void Zone::OnPlayerCommand(Id player_id, const Command & cmd)
{
  printf("(zone : %p)->PlayerCommand(id : %u, ...)\n", this, player_id);
}

}

