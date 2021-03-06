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
    if (!texture) texture = make_unique<Texture>(image::loadBMP("puck.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("puck.obj");

    //Load mtl files
    ifstream mtl("puck.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    this->spawned = false;

    this->age = 0.0f;
    this->duration = 5.0f;


    this->position = {linearRand(-((Scene::WIDTH) / 100.0f) + 2.5f, ((Scene::WIDTH) / 100.0f)) - 2.5f, ((Scene::WIDTH) / 100.0f) - 2.5f, 0.0f};

    int random = rand() % 4 + 1;
    for(int i = 0; i < random; i++) {
        auto rotatingBall = make_unique<RotatingBall>();

        //Blbne pre 180 stupnov
        if(sin(90 * i * PI / 180) >= -0.1 && sin(90 * i * PI / 180) <= 0.1)
            rotatingBall->position.y = 1.2f * cos(90 * i * PI / 180);
        else
            rotatingBall->position.x = 1.2f * sin(90 * i * PI / 180);

        this->rotatingBalls.push_back(move(rotatingBall));
    }

    addKeyFrame(600, this->rotation, this->scale, {this->position.x, -((Scene::WIDTH) / 100.0f) + 2.5f, this->position.z});
    addKeyFrame(600, this->rotation, this->scale, {this->position.x, ((Scene::WIDTH) / 100.0f) - 2.5f, this->position.z});
}

bool Splitter::update(Scene &scene, float dt) {

    age += dt;

    this->rotation = {0.0f, age, 0.0f};

    if(this->age > this->duration)
        return false;

    if(!spawned) {
        this->spawned = true;

        for (auto& obj : this->rotatingBalls) {
            scene.objects.push_back(move(obj));
        }
    }

    for (auto &obj : scene.objects) {
        if (obj.get() == this) continue;

        auto ball = dynamic_cast<Ball*>(obj.get());

        if (!ball) continue;

        if(distance(position, ball->position) <= (scale.x * 2.0)) {

            for(int i = 0; i < this->rotatingBalls.size(); i++) {
                //Add ball to the game
                auto n_ball = make_unique<Ball>();
                n_ball->position = ball->position;
                n_ball->position.x += linearRand(1.5f, 2.5f);
                n_ball->position.y += linearRand(1.5f, 2.5f);
                n_ball->scale = ball->scale;

                scene.objects.push_back(move(n_ball));
            }

            //Destroy splitter
            this->age += this->duration * 2;
            break;
        }

    }

    updateKeyFrame();
    generateModelMatrix();
    return true;
}

void Splitter::render(Scene &scene) {
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
