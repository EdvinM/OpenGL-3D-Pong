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
    this->time = 0.0f;

    auto magnifier = make_unique<Magnify> ();
    magnifier->scale *= 0.5f;
    this->powerups.push_back(move(magnifier));

    auto quake = make_unique<QuakePU>();
    this->powerups.push_back(move(quake));

    auto life_ = make_unique<Life>();
    life_->scale *= 0.2f;
    this->powerups.push_back(move(life_));
}

bool PowerupManager::update(Scene &scene, float dt) {

    time += dt;

    if(time > 10.0f) {
        time = 0.0f;

        int spawnPowerupIndex = (int)(rand() % (this->powerups.size() + 1));

        //Generate spawn position for the attribute
        vec3 spawnPosition = {linearRand(-10.0f, 10.0f), linearRand(-10.0f, 10.0f), -0.05f};

        //If random is 3, that means splitter has to be spawned
        if(spawnPowerupIndex == 3) {
            scene.objects.push_back(make_unique<Splitter>(spawnPosition));
        }
        else {
            auto &obj = this->powerups[spawnPowerupIndex];
            obj->position = spawnPosition;

            scene.objects.push_back(move(obj));
        }
    }

    return true;
}

void PowerupManager::render(Scene &scene) { }