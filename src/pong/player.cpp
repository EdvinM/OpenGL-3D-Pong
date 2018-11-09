#include <glm/gtc/random.hpp>
#include "player.h"
#include "scene.h"
#include "asteroid.h"
#include "border.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// shared resources
unique_ptr<Mesh> Player::mesh;
unique_ptr<Texture> Player::texture;
unique_ptr<Shader> Player::shader;

Player::Player() {
  // Scale the default model
  scale.y *= 3.0f;

  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("corsair.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("paddle.obj");
}

Player::Player(int control_up, int control_down, int id) : Player() {
  this->control_up    = control_up;
  this->control_down  = control_down;
}

bool Player::update(Scene &scene, float dt) {
  // Fire delay increment
  fireDelay += dt;

  // Keyboard controls
  if(scene.keyboard[this->control_up] && (position.y * 100) <= 1280 - (texture->image.height / 3)) {
    position.y += 10 * dt;
  } else if(scene.keyboard[this->control_down] && (position.y * 100) > -1280 + (texture->image.height / 3)) {
    position.y -= 10 * dt;
  }

  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    auto Border = dynamic_cast<border*>(obj.get());

    if(!Border) continue;

    //cout << << endl;
    if(distance((float)((Border->position.x + 1.0f) * Border->scale.x), position.y) <= scale.y) {
      cout << "position= " << position.y << endl;
    }
  }

  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene) {
  shader->use();

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Player::onClick(Scene &scene) {
  cout << "Player has been clicked!" << endl;
}
