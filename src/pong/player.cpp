#include <glm/gtc/random.hpp>
#include "player.h"
#include "scene.h"
#include "Ball.h"
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

map<std::string, int> Player::material_map;
vector<tinyobj::material_t> Player::material;

Player::Player() {
  mutex = false;

  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("hockeypaddle-2.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("hockeypaddle.obj");

    //Load mtl files
    ifstream mtl("hockeypaddle.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);
}

Player::Player(Scene &scene, int control_up, int control_down, int position) : Player() {
  this->control_up    = control_up;
  this->control_down  = control_down;

  this->can_move_up   = true;
  this->can_move_down = true;

  this->pos           = position;

  for(float movement = 0; movement <= 5.0f;) {
      auto life = make_unique<Life>(vec3({0, 0, 0}), vec3({0.5f,0.5f,0.5}), vec3({((Scene::WIDTH) / 100.0f + 1.3f) * position, ((Scene::WIDTH) / 100.0f) - movement, 0}));
      lifes.push_back(move(life));

      movement += 2.5f;
  }

    this->mutex = false;
  this->acceleration = 0.0f;
}

bool Player::update(Scene &scene, float dt) {

  //Updated rendered player lifes
    for (auto& obj : this->lifes) {
        obj->update(scene, dt);
    }

    if(this->lifes.size() == 0) {
        this->initEndGameScene(scene, this->pos);
        return false;
    }

  // Keyboard controls
  if(scene.keyboard[this->control_up] && this->can_move_up) {
      this->acceleration += dt * 1.25f;

    position.y += 10 * dt * this->acceleration;
  } else if(scene.keyboard[this->control_down] && this->can_move_down) {
      this->acceleration += dt * 1.25f;

      position.y -= 10 * dt * this->acceleration;
  }
  else {
      this->acceleration = 1.3f;
  }

  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    auto Border = dynamic_cast<border*>(obj.get());

    if(!Border) continue;

    if (distance(Border->position.y, position.y) <= (scale.y + 0.5f)) {
        //Stop upper movement for our paddle if it reached border down

        if (Border->border_position == 2) {
            this->can_move_down = false;
        }

        //Stop upper movement for our paddle if it reached upper border
        if (Border->border_position == 1) {
            this->can_move_up = false;
        }
    } else {

        if (abs(Border->scale.x - distance(Border->position, position)) >= 0.1f) {
            if (!this->can_move_up && Border->border_position == 1)
                this->can_move_up = true;

            if (!this->can_move_down && Border->border_position == 2)
                this->can_move_down = true;
        }
    }
  }

    updateKeyFrame();
  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene) {
    //Render player lifes
    for (auto& obj : this->lifes) {
        obj->render(scene);
    }

    shader->use();
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);
    shader->setUniform("LightDirection2", scene.lightDirection2);
    shader->setUniform("LightColor2", scene.lightColor2);
    shader->setUniform("AmbientLightColor", scene.ambientLightColor);
    shader->setUniform("CameraPosition", scene.camera->position);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);

    shader->setUniform("Ambient", vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]));
    shader->setUniform("Diffuse", vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f));
    shader->setUniform("Specular", vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]));
    shader->setUniform("Shininess", (material.data()->shininess * 128));

    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Player::initEndGameScene(Scene &scene, int side) {
    scene.camera->position.z = -17.82f;
    scene.camera->position.y = -0.060f;
    scene.camera->position.x = -77.220f;

    scene.camera->back.y = -2.71988f;
    scene.camera->back.z = -37.0402f;

    scene.camera->up.y = 1.0f;

    scene.objects.push_back(make_unique<TextBackground>());

    if(side != -1) {
        auto winnerScene = make_unique<RightWinnerScene>();
        winnerScene->position.x = -76.99f;
        winnerScene->position.y = 1.29f;
        winnerScene->position.z = -5.0f;

        scene.objects.push_back(move(winnerScene));
    }
    else {
        auto winnerScene = make_unique<LeftWinnerScene>();
        winnerScene->position.x = -76.99f;
        winnerScene->position.y = 1.29f;
        winnerScene->position.z = -5.0f;

        scene.objects.push_back(move(winnerScene));
    }

}
