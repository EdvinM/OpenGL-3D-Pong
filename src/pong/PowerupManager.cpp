//
// Created by Edvin Mako on 27/11/2018.
//

//
// Created by Edvin Mako on 27/11/2018.
//
#include <glm/gtc/random.hpp>
#include "PowerupManager.h"

#include "Powerups/Magnify.h"
#include "Powerups/QuakePU.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> PowerupManager::mesh;
unique_ptr<Texture> PowerupManager::texture;
unique_ptr<Shader> PowerupManager::shader;

map<std::string, int> PowerupManager::material_map;
vector<tinyobj::material_t> PowerupManager::material;

PowerupManager::PowerupManager() {

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("soccerball.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("magnifier.obj");

    //Load mtl files
    ifstream mtl("magnifier.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    this->spawned = false;
}

bool PowerupManager::update(Scene &scene, float dt) {

    if(!this->spawned) {

        vec3 spawnPosition = {linearRand(-10.0f, 10.0f), linearRand(-10.0f, 10.0f), -0.05f};

//        auto magnifier = make_unique<Magnify>();
//        magnifier->position = spawnPosition;
//        magnifier->scale *= 0.5f;
//        scene.objects.push_back(move(magnifier));

        auto magnifier = make_unique<QuakePU>();
        magnifier->position = spawnPosition;
        scene.objects.push_back(move(magnifier));

        this->spawned = true;
    }

}

void PowerupManager::render(Scene &scene) {
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