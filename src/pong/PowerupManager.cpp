//
// Created by Edvin Mako on 27/11/2018.
//

//
// Created by Edvin Mako on 27/11/2018.
//
#include <glm/gtc/random.hpp>
#include "PowerupManager.h"

#include "life.h"
#include "Powerups/Magnify.h"
#include "Powerups/QuakePU.h"
#include "Powerups/Splitter.h"

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

    this->spawned = false;
}

bool PowerupManager::update(Scene &scene, float dt) {

    if(!this->spawned) {

        vec3 spawnPosition = {linearRand(-10.0f, 10.0f), linearRand(-10.0f, 10.0f), -0.05f};

//        auto magnifier = make_unique<Magnify>();
//        magnifier->position = spawnPosition;
//        magnifier->scale *= 0.5f;
//        scene.objects.push_back(move(magnifier));

//        auto quake = make_unique<QuakePU>();
//        quake->position = spawnPosition;
//        scene.objects.push_back(move(quake));

//        auto life_ = make_unique<Life>();
//        life_->position = spawnPosition;
//        life_->scale *= 0.2f;
//        scene.objects.push_back(move(life_));

        scene.objects.push_back(make_unique<Splitter>(spawnPosition));

        this->spawned = true;
    }

}

void PowerupManager::render(Scene &scene) { }