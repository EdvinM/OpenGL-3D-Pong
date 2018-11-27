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
}

bool PowerupManager::update(Scene &scene, float dt) {

    time += dt;

    if(time > 10.0f) {
        time = 0.0f;

        int spawnPowerupIndex = rand() % 4;

        //Generate spawn position for the attribute
        vec3 spawnPosition = {linearRand(-10.0f, 10.0f), linearRand(-10.0f, 10.0f), -0.05f};

        scene.objects.push_back(getPowerUp(spawnPosition, spawnPowerupIndex));
    }

    return true;
}

std::unique_ptr<Object> PowerupManager::getPowerUp(vec3 spawnPosition, int number) {
    switch(number) {
        case 0: {
            auto powerup = make_unique<Magnify>();
            powerup->position = spawnPosition;
            powerup->scale *= 0.5f;
            powerup->duration = linearRand(4.5f, 6.0f);

            return powerup;
        }
        case 1: {
            auto powerup2 = make_unique<QuakePU>();
            powerup2->position = spawnPosition;
            powerup2->duration = linearRand(3.5f, 5.0f);
            return powerup2;
        }
        case 2: {
            auto powerup3 = make_unique<Life>();
            powerup3->position = spawnPosition;
            powerup3->scale *= 0.2f;
            powerup3->duration = linearRand(5.0f, 7.0f);
            return powerup3;
        }
        default: {
            auto powerup4 = make_unique<Splitter>(spawnPosition);
            powerup4->duration = linearRand(3.5f, 6.0f);
            return powerup4;
        }
    }
}

void PowerupManager::render(Scene &scene) { }
