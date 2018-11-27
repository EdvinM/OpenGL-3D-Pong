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

Splitter::Splitter() {
    this->spawned = false;
}

Splitter::Splitter(vec3 position) : Splitter() {
    this->ball = make_unique<QuakePU>();
    this->ball->position = position;

    int random = rand() % 4 + 1;
    for(int i = 0; i < random; i++) {
        auto rotatingBall = make_unique<RotatingBall>();

        //Blbne pre 180 stupnov
        if(sin(90 * i * PI / 180) >= -0.1 && sin(90 * i * PI / 180) <= 0.1)
            rotatingBall->position.y = 2.0f * cos(90 * i * PI / 180);
        else
            rotatingBall->position.x = 2.0f * sin(90 * i * PI / 180);

        this->rotatingBalls.push_back(move(rotatingBall));
    }
};

bool Splitter::update(Scene &scene, float dt) {

    if(!spawned) {
        scene.objects.push_back(move(this->ball));

        for (auto& obj : this->rotatingBalls) {
            scene.objects.push_back(move(obj));
        }

        this->spawned = true;
    }

    return false;
}

void Splitter::render(Scene &scene) {

}
