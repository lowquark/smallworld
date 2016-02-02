
#include <game/server.hpp>

class MyView : public game::View
{
  void OnUpdateTerrain() {
    printf("OnUpdateTerrain()\n");
  }
  void OnUpdateMesh(game::Id eid, const game::Agent::Mesh & mesh) {
    printf("OnUpdateMesh()\n");
  }
  void OnDestroy(game::Id eid) {
    printf("OnDestroy()\n");
  }
  void OnEnterZone() {
    printf("OnEnterZone()\n");
  }
  void OnLeaveZone() {
    printf("OnLeaveZone()\n");
  }
};

MyView view;

int main(int argc, char ** argv)
{
  auto tileset_id = game::LoadTexture("res/tileset.png");
  auto hedge_mesh_id = game::LoadMesh("res/mesh/hedge.mesh");

  game::server::Init();

  game::Zone * z = new game::Zone("some_script.lua");
  game::server::AddZone("Lumbridge", z, true);

  game::server::CreatePlayer("noob", &view);

  game::server::Open(8888);

  game::server::Deinit();

  delete z;
  z = 0;
}

