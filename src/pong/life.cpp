//
// Created by Edvin Mako on 10/11/2018.
//

#include "life.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Life::mesh;
unique_ptr<Texture> Life::texture;
unique_ptr<Shader> Life::shader;

Life::Life() {
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("heart_lp.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("heart_lp.obj");

    this->time = 0.001f;

    this->active = true;
}

bool Life::update(Scene &scene, float dt) {

    if(!this->active)
        return false;

    // Rotate the object
    rotation = vec3({0, time, 0});
    time += 0.01f;

    generateModelMatrix();
    return true;
}

void Life::render(Scene &scene) {
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