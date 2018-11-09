//
// Created by Edvin Mako on 09/11/2018.
//

#pragma once

#ifndef PPGSO_FLOOR_H
#define PPGSO_FLOOR_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class Floor final : public Object {

    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:
        Floor();

        /*!
        * Update player position considering keyboard inputs
        * @param scene Scene to update
        * @param dt Time delta
        * @return true to delete the object
        */
        bool update(Scene &scene, float dt) override;

        /*!
       * Render asteroid
       * @param scene Scene to render in
       */
        void render(Scene &scene) override;
};


#endif //PPGSO_FLOOR_H
