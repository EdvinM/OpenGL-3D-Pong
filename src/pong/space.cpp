#include "space.h"
#include "scene.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

Space::Space() {

  scale *= 10.0f;
  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(texture_vert_glsl, texture_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("stars.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("quad.obj");
}

bool Space::update(Scene &scene, float dt) {
  // Offset for UV mapping, creates illusion of scrolling
  //textureOffset.y -= dt/5;

  generateModelMatrix();
  return true;
}

void Space::render(Scene &scene) {
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

// shared resources
unique_ptr<Mesh> Space::mesh;
unique_ptr<Shader> Space::shader;
unique_ptr<Texture> Space::texture;
