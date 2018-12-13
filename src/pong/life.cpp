//
// Created by Edvin Mako on 10/11/2018.
//

#include "life.h"
#include "player.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Life::mesh;
unique_ptr<Texture> Life::texture;
unique_ptr<Shader> Life::shader;

map<std::string, int> Life::material_map;
vector<tinyobj::material_t> Life::material;

Life::Life() {
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("heart_lp.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("heart_lp.obj");

    this->time = 0;
    this->duration = 0;
    this->active = true;

    //Load mtl files
    ifstream mtl("heart_lp.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);
}

Life::Life(glm::vec3 rotation, glm::vec3 scale, glm::vec3 position) : Life() {
    this->rotation = rotation;
    this->scale = scale;
    this->position = position;

    addKeyFrame(100, {0, 2.5, 0}, this->scale, this->position);
    addKeyFrame(150, {0, 5, 0}, this->scale, this->position);
    addKeyFrame(400, {0, 2.5, 0}, this->scale, this->position);
    addKeyFrame(450, {0, 0, 0}, this->scale, this->position);
}

bool Life::update(Scene &scene, float dt) {

    if(!this->active) {
        return false;
    }

    time += dt;

    if(this->duration != 0 && this->time > this->duration)
        return false;

    for (auto &obj : scene.objects) {
        if (obj.get() == this) continue;

        auto ball = dynamic_cast<Ball*>(obj.get());

        if(!ball) continue;

        if (distance(position, ball->position) <= (scale.x * 2)) {

            for (auto &obj : scene.objects) {
                if (obj.get() == this) continue;

                auto player = dynamic_cast<Player*>(obj.get());

                if (!player) continue;

                if(player->pos == ball->lastHitByPlayerId) {

                    auto life = make_unique<Life>(vec3({0, 0, 0}), vec3({0.5f,0.5f,0.5}), vec3({((Scene::WIDTH) / 100.0f + 1.3f) * player->pos, (player->lifes[player->lifes.size() - 1]->position.y) - 2.5f, 0}));
                    player->lifes.push_back(move(life));

                    //Destroy the heart
                    this->time += this->duration * 2;
                    break;
                }
            }
        }

        break;
    }

    updateKeyFrame();
    generateModelMatrix();
    return true;
}

void Life::render(Scene &scene) {

    vec3 ambient = vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]);
    vec4 diffuse = vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f);
    vec3 specular = vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]);
    float shininess = material.data()->shininess * 128;

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

    shader->setUniform("MaterialAmbient", {ambient.x, ambient.y, ambient.z});
    shader->setUniform("MaterialDiffuse", {diffuse.x, diffuse.y, diffuse.z, 1.0f});
    shader->setUniform("MaterialSpecular", {specular.x, specular.y, specular.z});
    shader->setUniform("MaterialShininess", shininess);

    shader->setUniform("Texture", *texture);
    mesh->render();
}