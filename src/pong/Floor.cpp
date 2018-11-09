//
// Created by Edvin Mako on 09/11/2018.
//

#include "Floor.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Floor::mesh;
unique_ptr<Texture> Floor::texture;
unique_ptr<Shader> Floor::shader;

Floor::Floor() {
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("sphere.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("floor.obj");
}

bool Floor::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Floor::render(Scene &scene) {
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