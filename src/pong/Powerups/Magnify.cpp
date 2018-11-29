//
// Created by Edvin Mako on 27/11/2018.
//

#include <glm/gtc/random.hpp>
#include "Magnify.h"
#include "../player.h"
#include "../Ball.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Magnify::mesh;
unique_ptr<Texture> Magnify::texture;
unique_ptr<Shader> Magnify::shader;

map<std::string, int> Magnify::material_map;
vector<tinyobj::material_t> Magnify::material;

Magnify::Magnify() {

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("soccerball.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("magnifier.obj");

    //Load mtl files
    ifstream mtl("magnifier.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    this->age = 0.0f;
    this->duration = 5.0f;
    this->picked = false;
    this->magnifiedPlayerId = 0;
}

Magnify::Magnify(vec3 scale) : Magnify() {
    this->scale = scale;
    this->position = {linearRand(-((Scene::WIDTH) / 100.0f) + 3.5f, ((Scene::WIDTH) / 100.0f)) - 3.5f, ((Scene::WIDTH) / 100.0f) - 2.5f, 0.0f};

    addKeyFrame(600, this->rotation, this->scale, {this->position.x, -((Scene::WIDTH) / 100.0f) + 2.5f, this->position.z});
    addKeyFrame(600, this->rotation, this->scale, {this->position.x, ((Scene::WIDTH) / 100.0f) - 2.5f, this->position.z});
}


bool Magnify::update(Scene &scene, float dt) {
    age += dt;

    if(!picked && this->age > this->duration)
        return false;

    if(!picked) {
        for (auto &obj : scene.objects) {
            if (obj.get() == this) continue;

            auto ball = dynamic_cast<Ball *>(obj.get());

            if (!ball) continue;

            if (distance(position, ball->position) <= (scale.x * 4.0) && !picked) {

                for (auto &obj : scene.objects) {
                    if (obj.get() == this) continue;

                    auto player = dynamic_cast<Player *>(obj.get());

                    if (!player) continue;

                    if (player->pos == ball->lastHitByPlayerId) {
                        player->scale.x *= 2.0f;
                        player->scale.y *= 2.0f;

                        //Destroy this attribute
                        this->picked = true;
                        this->magnifiedPlayerId = player->pos;
                        this->effectDuration += this->age;
                        break;
                    }
                }

                break;
            }
        }
    }
    else {
        if(this->age > this->effectDuration) {
            for (auto &obj : scene.objects) {
                if (obj.get() == this) continue;

                auto player = dynamic_cast<Player *>(obj.get());

                if (!player) continue;

                if(player->pos == this->magnifiedPlayerId) {
                    player->scale.x /= 2.0f;
                    player->scale.y /= 2.0f;

                    //Delete this object after effect expiration
                    this->picked = false;
                    this->age += this->duration * 2;
                    break;
                }
            }
        }
    }

    updateKeyFrame();
    generateModelMatrix();
    return true;
}

void Magnify::render(Scene &scene) {

    if(!picked) {
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
}