//
// Created by Edvin Mako on 10/11/2018.
//

#include "life.h"
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

Keyframe setKeyframe(int duration, vec3 keyFrameRotation, vec3 keyFrameScale, vec3 keyFramePosition){
    Keyframe newKeyframe;
    newKeyframe.duration = duration;
    newKeyframe.keyframeRotation = keyFrameRotation;
    newKeyframe.keyframeScale = keyFrameScale;
    newKeyframe.keyframePosition = keyFramePosition;

    return newKeyframe;
}

Life::Life() {
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("heart_lp.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("heart_lp.obj");

    this->time = 0.001f;

    this->active = true;

    //Load mtl files
    ifstream mtl("heart_lp.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);
}

Life::Life(glm::vec3 rotation, glm::vec3 scale, glm::vec3 position) : Life() {
    this->rotation = rotation;
    this->scale = scale;
    this->position = position;

    keyframeAnimation[0] = setKeyframe(100, {0, 2.5, 0}, this->scale, this->position);
    keyframeAnimation[1] = setKeyframe(100, {0, 5, 0}, this->scale, this->position);
    keyframeAnimation[2] = setKeyframe(100, {0, 2.5, 0}, this->scale, this->position);
    keyframeAnimation[3] = setKeyframe(100, {0, 0, 0}, this->scale, this->position);
}

vec3 Life::linearInterpolation(vec3 a, vec3 b, float t){
    vec3 result = (1 - t) * a + t * b;
    return result;
}

bool Life::update(Scene &scene, float dt) {

    if(!this->active) {
        explode(scene, 10);
        return false;
    }

    time += 0.01f;

    Keyframe current = keyframeAnimation[processedKeyframes];
    Keyframe next = keyframeAnimation[(processedKeyframes + 1) % keyframeCount];

    float t = keyframeDuration / current.duration;

    position = linearInterpolation(current.keyframePosition, next.keyframePosition, t);
    scale = linearInterpolation(current.keyframeScale, next.keyframeScale, t);
    rotation = linearInterpolation(current.keyframeRotation, next.keyframeRotation, t);

    keyframeDuration++;
    if (keyframeDuration >= current.duration) {
        keyframeDuration = 0;
        processedKeyframes = (processedKeyframes + 1) % keyframeCount;
    }

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

void Life::explode(Scene &scene, int pieces) {
    // Generate smaller asteroids

}