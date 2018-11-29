//
// Created by Edvin Mako on 09/11/2018.
//

#include "Floor.h"
#include <iterator>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Floor::mesh;
unique_ptr<Texture> Floor::texture;
unique_ptr<Shader> Floor::shader;

map<std::string, int> Floor::material_map;
vector<tinyobj::material_t> Floor::material;

Floor::Floor() {
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("grass.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("uvmappedcube.obj");

    ifstream mtl("uvmappedcube.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    //Starting camera position
    this->cameraPositions[0].position = {0, -22.0, -30.0f};
    this->cameraPositions[0].up = {0,1,0};
    this->cameraPositions[0].back = {0,-1.0f,-1.5f};

    //Up camera projection
    this->cameraPositions[1].position = {0, -2.31, -48.9007};
    this->cameraPositions[1].up = {0,1,0};
    this->cameraPositions[1].back = {0,-2.71988,-37.0402};
}

bool Floor::update(Scene &scene, float dt) {

    if (scene.keyboard[GLFW_KEY_0] && scene.keyboard[GLFW_KEY_K]){
        scene.camera->position.x -= 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_1]&& scene.keyboard[GLFW_KEY_K]){
        scene.camera->position.y -= 0.04f;
    }
    else if (scene.keyboard[GLFW_KEY_2]&& scene.keyboard[GLFW_KEY_K]){
        scene.camera->position.z -= 0.04f;
    }
    else if (scene.keyboard[GLFW_KEY_3]&& scene.keyboard[GLFW_KEY_K]){
        scene.camera->back.y -= 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_4]&& scene.keyboard[GLFW_KEY_K]){
        scene.camera->back.z -= 0.01f;
    }

    else if (scene.keyboard[GLFW_KEY_0]){
        scene.camera->position.x += 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_1]){
        scene.camera->position.y += 0.04f;
    }
    else if (scene.keyboard[GLFW_KEY_2]){
        scene.camera->position.z += 0.04f;
    }
    else if (scene.keyboard[GLFW_KEY_3]){
        scene.camera->back.y += 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_4]){
        scene.camera->back.z += 0.01f;
    }
    else if (scene.keyboard[GLFW_KEY_J]){
        scene.camera->up = {0,1,0};
        scene.camera->position = {0,0,0};
        scene.camera->back = {0,0,-1};
        scene.camera->position.z = -20.0f;
        scene.camera->position.y = -15.0f;
        scene.camera->back.y = -0.8f;
    }

    else if(scene.keyboard[GLFW_KEY_C] && scene.keyboard[GLFW_KEY_L]){
        scene.camera->up = this->cameraPositions[0].up;
        scene.camera->back = this->cameraPositions[0].back;
        scene.camera->position = this->cameraPositions[0].position;
    }
    else if(scene.keyboard[GLFW_KEY_C] && scene.keyboard[GLFW_KEY_U]){
        scene.camera->up = this->cameraPositions[1].up;
        scene.camera->back = this->cameraPositions[1].back;
        scene.camera->position = this->cameraPositions[1].position;
    }


    //Light Movement
    else if(scene.keyboard[GLFW_KEY_L] && scene.keyboard[GLFW_KEY_LEFT]) {
        scene.lightDirection.x -= 0.05;
    }
    else if(scene.keyboard[GLFW_KEY_L] && scene.keyboard[GLFW_KEY_RIGHT]) {
        scene.lightDirection.x += 0.05;
    }
    else if(scene.keyboard[GLFW_KEY_L] && scene.keyboard[GLFW_KEY_UP]) {
        scene.lightDirection.y += 0.05;
    }
    else if(scene.keyboard[GLFW_KEY_L] && scene.keyboard[GLFW_KEY_DOWN]) {
        scene.lightDirection.y -= 0.05;
    }
    else if(scene.keyboard[GLFW_KEY_L] && scene.keyboard[GLFW_KEY_Z]) {
        scene.lightDirection.z -= 0.05;
    }
    else if(scene.keyboard[GLFW_KEY_L] && scene.keyboard[GLFW_KEY_X]) {
        scene.lightDirection.z += 0.05;
    }

    else if(scene.keyboard[GLFW_KEY_F]) {

        cout << "Pos X = " << scene.camera->position.x << endl;
        cout << "Pos Y = " << scene.camera->position.y << endl;
        cout << "Pos Z = " << scene.camera->position.z << endl;

        cout << "Back X = " << scene.camera->back.x << endl;
        cout << "Back Y = " << scene.camera->back.y << endl;
        cout << "Back Z = " << scene.camera->back.z << endl;

        cout << "Up X = " << scene.camera->up.x << endl;
        cout << "Up Y = " << scene.camera->up.y << endl;
        cout << "Up Z = " << scene.camera->up.z << endl;

    }

    generateModelMatrix();
    return true;
}

void Floor::render(Scene &scene) {

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