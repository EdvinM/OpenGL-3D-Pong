//
// Created by Edvin Mako on 27/11/2018.
//

#include <glm/gtc/random.hpp>
#include <cmath>
#include "Splitter.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Splitter::mesh;
unique_ptr<Texture> Splitter::texture;
unique_ptr<Shader> Splitter::shader;

map<std::string, int> Splitter::material_map;
vector<tinyobj::material_t> Splitter::material;

Splitter::Splitter() {
    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("soccerball.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("puck.obj");

    //Load mtl files
    ifstream mtl("puck.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    this->spawned = false;
    this->age = 0.0f;
}

Splitter::Splitter(vec3 position) : Splitter() {
    this->position = position;

    int random = rand() % 4 + 1;
    for(int i = 0; i < random; i++) {
        auto rotatingBall = make_unique<RotatingBall>();

        //Blbne pre 180 stupnov
        if(sin(90 * i * PI / 180) >= -0.1 && sin(90 * i * PI / 180) <= 0.1)
            rotatingBall->position.y = 1.5f * cos(90 * i * PI / 180);
        else
            rotatingBall->position.x = 1.5f * sin(90 * i * PI / 180);

        this->rotatingBalls.push_back(move(rotatingBall));
    }
};

bool Splitter::update(Scene &scene, float dt) {

    age += dt;

    this->rotation = {0.0f, age, 0.0f};

    if(age > 2.5f) {
        return false;
    }

    if(!spawned) {
        this->spawned = true;

        for (auto& obj : this->rotatingBalls) {
            scene.objects.push_back(move(obj));
        }
    }

    generateModelMatrix();
    return true;
}

void Splitter::render(Scene &scene) {
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
