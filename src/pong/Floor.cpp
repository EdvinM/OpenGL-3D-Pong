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

    if (scene.keyboard[GLFW_KEY_0] && scene.keyboard[GLFW_KEY_K]){
        scene.camera->position.x -= 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_1]&& scene.keyboard[GLFW_KEY_K]){
        scene.camera->position.y -= 0.04f;
    }
    else if (scene.keyboard[GLFW_KEY_2]&& scene.keyboard[GLFW_KEY_K]){
        scene.camera->position.z -= 0.04f;
    }
    else if (scene.keyboard[GLFW_KEY_3]&& scene.keyboard[GLFW_KEY_K]){
        scene.camera->back.y -= 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_4]&& scene.keyboard[GLFW_KEY_K]){
        scene.camera->back.z -= 0.01f;
    }

    else if (scene.keyboard[GLFW_KEY_0]){
        scene.camera->position.x += 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_1]){
        scene.camera->position.y += 0.04f;
    }
    else if (scene.keyboard[GLFW_KEY_2]){
        scene.camera->position.z += 0.04f;
    }
    else if (scene.keyboard[GLFW_KEY_3]){
        scene.camera->back.y += 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_4]){
        scene.camera->back.z += 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_J]){
        scene.camera->up = {0,1,0};
        scene.camera->position = {0,0,0};
        scene.camera->back = {0,0,-1};
        scene.camera->position.z = -20.0f;
        scene.camera->position.y = -15.0f;
        scene.camera->back.y = -0.8f;
    }

    generateModelMatrix();
    return true;
}

void Floor::render(Scene &scene) {
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