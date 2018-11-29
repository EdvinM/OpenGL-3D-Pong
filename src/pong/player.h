#pragma once
#include <memory>
#include <vector>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "life.h"
#include "StaticScenes/TextBackground.h"
#include "StaticScenes/RightWinnerScene.h"
#include "StaticScenes/LeftWinnerScene.h"

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Asteroid objects in the scene
 */
class Player final : public Object {
private:
  // Static resources (Shared between instances)
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  //Things for mtl
  static std::vector<tinyobj::material_t> material;

    static std::map<std::string, int> material_map;

  int control_up;
    bool can_move_up;

    int control_down;
    bool can_move_down;

    bool spawnLifes;

public:

    static std::unique_ptr<ppgso::Texture> texture;

    int pos;
    float acceleration{0.0f};
    bool mutex;

public:
    // All objects to be rendered in scene
    std::vector<std::unique_ptr<Life>> lifes;
    /*!
   * Create a new player
   */
  Player();
    Player(Scene &scene, int control_up, int control_down, int position);

  /*!
   * Update player position considering keyboard inputs
   * @param scene Scene to update
   * @param dt Time delta
   * @return true to delete the object
   */
  bool update(Scene &scene, float dt) override;

  /*!
   * Render player
   * @param scene Scene to render in
   */
  void render(Scene &scene) override;


  /*!
   * Player click event
   * @param scene
   */
  void initEndGameScene(Scene &scene, int side);
};

