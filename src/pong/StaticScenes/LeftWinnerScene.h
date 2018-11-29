//
// Created by Edvin Mako on 2018-11-29.
//

#ifndef PPGSO_LEFTWINNERSCENE_H
#define PPGSO_LEFTWINNERSCENE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/pong/scene.h"
#include "src/pong/object.h"

class LeftWinnerScene : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    //Things for mtl
    static std::vector<tinyobj::material_t> material;
    static std::map<std::string, int> material_map;

public:
    LeftWinnerScene();

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


#endif //PPGSO_LEFTWINNERSCENE_H
