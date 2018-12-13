//
// Created by Edvin Mako on 27/11/2018.
//

#include <glm/gtc/random.hpp>
#include "RotatingBall.h"
#include "Splitter.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> RotatingBall::mesh;
unique_ptr<Texture> RotatingBall::texture;
unique_ptr<Shader> RotatingBall::shader;

map<std::string, int> RotatingBall::material_map;
vector<tinyobj::material_t> RotatingBall::material;

RotatingBall::RotatingBall() {

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("soccerball.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("soccerball.obj");

    //Load mtl files
    ifstream mtl("soccerball.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    this->scale *= 0.15f;
    this->age = 0.0f;
    this->duration = 5.0f;
}

bool RotatingBall::update(Scene &scene, float dt) {

    bool ballOnScreen = false;
    for (auto &obj : scene.objects) {
        if (obj.get() == this) continue;

        auto Ball = dynamic_cast<Splitter*>(obj.get());

        if(!Ball) continue;

        this->modelMatrix = Ball->modelMatrix * glm::translate(mat4(1.0f), position) * glm::scale(mat4(1.0f), scale);
        ballOnScreen = true;
    }

    //Delete rotating objects if the main object was also deleted
    if(!ballOnScreen) {
        return false;
    }

    return true;
}

void RotatingBall::render(Scene &scene) {
    shader->use();
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);
    shader->setUniform("LightDirection2", scene.lightDirection2);
    shader->setUniform("LightColor2", scene.lightColor2);
    shader->setUniform("AmbientLightColor", scene.ambientLightColor);
    shader->setUniform("CameraPosition", scene.camera->position);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);

    shader->setUniform("Ambient", vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]));
    shader->setUniform("Diffuse", vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f));
    shader->setUniform("Specular", vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]));
    shader->setUniform("Shininess", (material.data()->shininess * 128));

    shader->setUniform("Texture", *texture);
    mesh->render();
}