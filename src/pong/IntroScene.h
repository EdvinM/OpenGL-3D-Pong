//
// Created by Edvin Mako on 2018-11-29.
//

#ifndef PPGSO_INTROSCENE_H
#define PPGSO_INTROSCENE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class IntroScene : public Object {
    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;

        //Things for mtl
        static std::vector<tinyobj::material_t> material;
        static std::map<std::string, int> material_map;

    public:
        IntroScene();

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


#endif //PPGSO_INTROSCENE_H
