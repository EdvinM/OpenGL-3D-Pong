//
// Created by Edvin Mako on 27/11/2018.
//

#include <glm/gtc/random.hpp>
#include "Splitter.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

Splitter::Splitter() {

}

Splitter::Splitter(vec3 position) : Splitter() {
    this->ball.position = position;
};

bool Splitter::update(Scene &scene, float dt) {
    for (auto& obj : this->rotatingBalls) {
        obj->update(scene, dt);
    }
}

void Splitter::render(Scene &scene) {
    this->ball.render(scene);

    for (auto& obj : this->rotatingBalls) {
        obj->render(scene);
    }
}
