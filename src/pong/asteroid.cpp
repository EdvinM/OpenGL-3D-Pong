#include <glm/gtc/random.hpp>
#include "asteroid.h"
#include "projectile.h"
#include "explosion.h"
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

Asteroid::Asteroid() {
  // Set random scale speed and rotation
  scale *= linearRand(1.0f, 3.0f);
  speed = {linearRand(8.0f, 10.0f), linearRand(-8.0f, -10.0f), 0.0f};
  rotation = ballRand(PI);
  rotMomentum = ballRand(PI);

  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("asteroid.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("asteroid.obj");
}

bool Asteroid::update(Scene &scene, float dt) {
  // Count time alive
  age += dt;

  // Animate position according to time
  position += speed * dt;

  // Rotate the object
  rotation += rotMomentum * dt;

  // Delete when alive longer than 10s or out of visibility
  if (position.y < -12)
      speed.y *= (-1);

  if(position.y > 12)
      speed.y *= (-1);

    if (position.x < -12)
        speed.x *= (-1);

    if(position.x > 12)
        speed.x *= (-1);

  // Collide with scene
  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with asteroids and projectiles, ignore other objects
    auto asteroid = dynamic_cast<Asteroid*>(obj.get());
    auto projectile = dynamic_cast<Projectile*>(obj.get());
    auto player = dynamic_cast<Player*>(obj.get());
    if (!asteroid && !projectile && !player) continue;

    if (distance(position, player->position) <= player->scale.x) {
      float x_deviation_value = (player->scale.x - (distance(position, player->position))) + 0.01;
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

void Asteroid::explode(Scene &scene, vec3 explosionPosition, vec3 explosionScale, int pieces) {
  // Generate explosion
  auto explosion = make_unique<Explosion>();
  explosion->position = explosionPosition;
  explosion->scale = explosionScale;
  explosion->speed = speed / 2.0f;
  scene.objects.push_back(move(explosion));

  // Generate smaller asteroids
  for (int i = 0; i < pieces; i++) {
    auto asteroid = make_unique<Asteroid>();
    asteroid->speed = speed + vec3(linearRand(-3.0f, 3.0f), linearRand(0.0f, -5.0f), 0.0f);;
    asteroid->position = position;
    asteroid->rotMomentum = rotMomentum;
    float factor = (float) pieces / 2.0f;
    asteroid->scale = scale / factor;
    scene.objects.push_back(move(asteroid));
  }
}

void Asteroid::render(Scene &scene) {
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

void Asteroid::onClick(Scene &scene) {
  cout << "Asteroid clicked!" << endl;
  explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
  age = 10000;
}

