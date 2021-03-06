//
// Created by Edvin Mako on 27/11/2018.
//

#include <glm/gtc/random.hpp>
#include "QuakePU.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> QuakePU::mesh;
unique_ptr<Texture> QuakePU::texture;
unique_ptr<Shader> QuakePU::shader;

map<std::string, int> QuakePU::material_map;
vector<tinyobj::material_t> QuakePU::material;

QuakePU::QuakePU() {

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    //if (!texture) texture = make_unique<Texture>(image::loadBMP("soccerball.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("skull.obj");

    //Load mtl files
    ifstream mtl("skull.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    this->age = 0.0f;
    this->duration = 5.0f;
    this->picked = false;
}

bool QuakePU::update(Scene &scene, float dt) {
    age += dt;

    this->rotation = {0.0f, age, 0.0f};

    if(!picked && this->age > this->duration)
        return false;

    if(picked && this->age > this->effectDuration)
        return false;

    if(!picked) {
        for (auto &obj : scene.objects) {
            if (obj.get() == this) continue;

            auto ball = dynamic_cast<Ball *>(obj.get());

            if (!ball) continue;

            if (distance(position, ball->position) <= (scale.x * 1.5)) {
                picked = true;
                this->effectDuration += this->age;
                break;
            }
        }
    }
    else {
        scene.camera->position.x = cos(this->age * this->age);
    }

    generateModelMatrix();
    return true;
}

void QuakePU::render(Scene &scene) {

    if(!picked) {
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

        //We don't need texture, because we are using material properties to set it's color
        //shader->setUniform("Texture", *texture);
        mesh->render();
    }

}