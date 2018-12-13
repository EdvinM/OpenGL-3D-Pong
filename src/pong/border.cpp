//
// Created by Edvin Mako on 09/11/2018.
//

#include "border.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> border::mesh;
unique_ptr<Texture> border::texture;
unique_ptr<Shader> border::shader;

map<std::string, int> border::material_map;
vector<tinyobj::material_t> border::material;

border::border() {

    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("Sand-2.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("uvmappedcube.obj");

    //Load mtl files
    ifstream mtl("uvmappedcube.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    this->mutex = false;
}

border::border(int border_position) : border() {
    this->border_position = border_position;
}

bool border::update(Scene &scene, float dt) {

    generateModelMatrix();
    return true;
}

void border::render(Scene &scene) {
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

    shader->setUniform("MaterialAmbient", vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]));
    shader->setUniform("MaterialDiffuse", vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f));
    shader->setUniform("MaterialSpecular", vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]));
    shader->setUniform("MaterialShininess", material.data()->shininess * 128);

    shader->setUniform("Texture", *texture);
    mesh->render();
}