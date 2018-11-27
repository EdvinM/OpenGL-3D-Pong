//
// Created by Edvin Mako on 27/11/2018.
//

#ifndef PPGSO_QUAKEPU_H
#include <memory>

#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../object.h"
#define PPGSO_QUAKEPU_H


class QuakePU : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    //Things for mtl
    static std::vector<tinyobj::material_t> material;
    static std::map<std::string, int> material_map;

    // Age of the object in seconds
    float age{0.0f};

public:
    QuakePU();

    /*!
   * Update asteroid
   * @param scene Scene to interact with
   * @param dt Time delta for animation purposes
   * @return
   */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};


#endif //PPGSO_QUAKEPU_H
