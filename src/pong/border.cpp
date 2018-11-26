//
// Created by Edvin Mako on 09/11/2018.
//

#include "border.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> border::mesh;
unique_ptr<Texture> border::texture;
unique_ptr<Shader> border::shader;

border::border() {

    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("sphere.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("uvmappedcube.obj");
}

border::border(int border_position) : border() {
    this->border_position = border_position;
}

bool border::update(Scene &scene, float dt) {

    generateModelMatrix();
    return true;
}

void border::render(Scene &scene) {
    shader->use();
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);
    shader->setUniform("AmbientLightColor", scene.ambientLightColor);
    shader->setUniform("CameraPosition", scene.camera->position);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);

    shader->setUniform("Texture", *texture);
    mesh->render();
}