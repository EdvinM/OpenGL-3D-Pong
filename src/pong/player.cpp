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

Player::Player(Scene &scene, int control_up, int control_down, int position) : Player() {
  this->control_up    = control_up;
  this->control_down  = control_down;

  this->can_move_up   = true;
  this->can_move_down = true;

  this->pos           = position;

    auto life = make_unique<Life>();
    life->position.x = ((Scene::WIDTH) / 100.0f + 2.0f) * position;
    life->scale *= 0.05f;
    lifes.push_back(move(life));

    auto life2 = make_unique<Life>();
    life2->position.x = ((Scene::WIDTH) / 100.0f + 2.0f) * position;
    life2->position.y = 2.5f;
    life2->scale *= 0.05f;
    lifes.push_back(move(life2));

    auto life3 = make_unique<Life>();
    life3->position.x = ((Scene::WIDTH) / 100.0f + 2.0f) * position;
    life3->position.y = -2.5f;
    life3->scale *= 0.05f;
    lifes.push_back(move(life3));
}

bool Player::update(Scene &scene, float dt) {
  // Fire delay increment
  fireDelay += dt;

  //Updated rendered player lifes
    for (auto& obj : this->lifes) {
        obj->update(scene, dt);
    }

  // Keyboard controls
  if(scene.keyboard[this->control_up] && (position.y * 100) <= 1280 - (texture->image.height / 3) && this->can_move_up) {
    position.y += 10 * dt;
  } else if(scene.keyboard[this->control_down] && (position.y * 100) > -1280 + (texture->image.height / 3) && this->can_move_down) {
    position.y -= 10 * dt;
  }

  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    auto Border = dynamic_cast<border*>(obj.get());

    if(!Border) continue;

    if (distance(Border->position, position) < Border->scale.x) {
        //Stop upper movement for our paddle if it reached border down

        if (Border->border_position == 2) {
            this->can_move_down = false;
        }

        //Stop upper movement for our paddle if it reached upper border
        if (Border->border_position == 1) {
            this->can_move_up = false;
        }
    } else {

        if(abs(Border->scale.x - distance(Border->position, position)) >= 0.6f) {
            if (!this->can_move_up && Border->border_position == 1)
                this->can_move_up = true;

            if (!this->can_move_down && Border->border_position == 2)
                this->can_move_down = true;
        }
    }
  }

  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene) {

    //Render player lifes
    for (auto& obj : this->lifes)
        obj->render(scene);

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
