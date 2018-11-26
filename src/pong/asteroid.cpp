#include <glm/gtc/random.hpp>
#include "asteroid.h"
#include "player.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Asteroid::mesh;
unique_ptr<Texture> Asteroid::texture;
unique_ptr<Shader> Asteroid::shader;

map<std::string, int> Asteroid::material_map;
vector<tinyobj::material_t> Asteroid::material;

Asteroid::Asteroid() {
  // Set random scale speed and rotation
  speed = {linearRand(8.0f, 10.0f), linearRand(8.0f, 10.0f), 0.0f};
  scale *= 0.02f;

  //Needed to give some randomness for our ball for the moving position
  if(rand() % 2 == 0) {
    speed.x *= -1;
  }

  if(rand() % 2 == 1) {
    speed.y *= -1;
  }

  rotation = ballRand(PI);
  rotMomentum = ballRand(PI);

  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("soccerball_specular.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("game_ball.obj");

  //Load mtl files
  ifstream mtl("game_ball.mtl", std::ifstream::binary);
  tinyobj::LoadMtl(this->material_map, this->material, mtl);
}

bool Asteroid::update(Scene &scene, float dt) {
  // Count time alive
  age += dt;

  // Animate position according to time
  position += speed * dt;

  // Rotate the object
  rotation += rotMomentum * dt;

  float x_deviation_value = 0;

  //Check for collision with screen boundaries
  if (position.y <= -(Scene::WIDTH / 100.0) + 2.56f) {
    x_deviation_value = static_cast<float>((Scene::WIDTH / 100.0) - 2.56f + position.y + 0.01);

    if(speed.y < 0)
      x_deviation_value *= -1;

    speed.y *= (-1);
    position.y += x_deviation_value;
  }

  if(position.y >= (Scene::WIDTH / 100.0) - 2.56f) {
    x_deviation_value = static_cast<float>((Scene::WIDTH / 100.0) - 2.56f - position.y + 0.01);

    if(speed.y < 0)
      x_deviation_value *= -1;

    speed.y *= (-1);
    position.y += x_deviation_value;
  }

  // Collide with scene
  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with asteroids and projectiles, ignore other objects
    auto asteroid = dynamic_cast<Asteroid*>(obj.get());
    auto player = dynamic_cast<Player*>(obj.get());
    if (!asteroid && !player) continue;

    if((position.x >= (Scene::WIDTH / 100.0) && player->pos == 1) || (position.x <= -(Scene::WIDTH / 100.0) && player->pos == -1)) {
      if(player->lifes.size() > 0) {

        //Remove player life
        player->lifes.pop_back();

        //Destroy the ball
        return false;
      }
    }

    if (distance(position, player->position) <= (player->scale.y * 2)) {
      x_deviation_value = ((player->scale.y * 2) - (distance(position, player->position))) + 0.01;
      if(speed.x > 0)
        x_deviation_value *= -1;

      speed.x *= (-1);
      position.x += x_deviation_value;
    }
  }

  // Generate modelMatrix from position, rotation and scale
  generateModelMatrix();

  return true;
}

void Asteroid::render(Scene &scene) {

  vec3 ambient = vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]);
  vec4 diffuse = vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f);
  vec3 specular = vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]);
  float shininess = material.data()->shininess * 128;

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

void Asteroid::onClick(Scene &scene) {
  cout << "Asteroid clicked!" << endl;
  age = 10000;
}

