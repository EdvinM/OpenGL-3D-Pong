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
  if (!texture) texture = make_unique<Texture>(image::loadBMP("corsair.bmp"));
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

    auto life = make_unique<Life>(vec3({0, 0, 0}), vec3({1* 0.05f,1* 0.05f,1* 0.05f}), vec3({((Scene::WIDTH) / 100.0f + 2.0f) * position, 0, 0}));
    lifes.push_back(move(life));

    auto life2 = make_unique<Life>(vec3({0, 0, 0}), vec3({1* 0.05f,1* 0.05f,1* 0.05f}), vec3({((Scene::WIDTH) / 100.0f + 2.0f) * position, 2.5f, 0}));
    lifes.push_back(move(life2));

    auto life3 = make_unique<Life>(vec3({0, 0, 0}), vec3({1* 0.05f,1* 0.05f,1* 0.05f}), vec3({((Scene::WIDTH) / 100.0f + 2.0f) * position, -2.5f, 0}));
    lifes.push_back(move(life3));

    this->mutex = false;
}

bool Player::update(Scene &scene, float dt) {
  // Fire delay increment
  fireDelay += dt;

  //Updated rendered player lifes
    for (auto& obj : this->lifes) {
        obj->update(scene, dt);
    }

  // Keyboard controls
  if(scene.keyboard[this->control_up] && this->can_move_up) {
    position.y += 10 * dt;
  } else if(scene.keyboard[this->control_down] && this->can_move_down) {
    position.y -= 10 * dt;
  }

  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    auto Border = dynamic_cast<border*>(obj.get());

    if(!Border) continue;

    if (distance(Border->position, position) <= 12.2) {
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

  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene) {

    vec3 ambient = vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]);
    vec4 diffuse = vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f);
    vec3 specular = vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]);
    float shininess = material.data()->shininess * 128;

    //Render player lifes
    for (auto& obj : this->lifes) {
        obj->render(scene);
    }

    shader->use();
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);
    shader->setUniform("AmbientLightColor", scene.ambientLightColor);
    shader->setUniform("CameraPosition", scene.camera->position);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);

    shader->setUniform("MaterialAmbient", {ambient.x, ambient.y, ambient.z});
    shader->setUniform("MaterialDiffuse", {diffuse.x, diffuse.y, diffuse.z, 1.0f});
    shader->setUniform("MaterialSpecular", {specular.x, specular.y, specular.z});
    shader->setUniform("MaterialShininess", shininess);

    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Player::onClick(Scene &scene) {
  cout << "Player has been clicked!" << endl;
}
