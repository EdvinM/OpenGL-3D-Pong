//
// Created by Edvin Mako on 2018-11-29.
//

#include <glm/gtc/random.hpp>
#include "RightWinnerScene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> RightWinnerScene::mesh;
unique_ptr<Shader> RightWinnerScene::shader;

map<std::string, int> RightWinnerScene::material_map;
vector<tinyobj::material_t> RightWinnerScene::material;

RightWinnerScene::RightWinnerScene() {
//    // Initialize static resources if needed
//    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
//    if (!mesh) mesh = make_unique<Mesh>("right_player.obj");
//
//    //Load mtl files
//    ifstream mtl("right_player.mtl", std::ifstream::binary);
//    tinyobj::LoadMtl(this->material_map, this->material, mtl);
}

bool RightWinnerScene::update(Scene &scene, float dt) {

    rotation.x = PI / 2;

    generateModelMatrix();
    return true;
}

void RightWinnerScene::render(Scene &scene) {
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

    mesh->render();
}
