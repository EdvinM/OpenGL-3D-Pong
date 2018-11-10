//
// Created by Edvin Mako on 09/11/2018.
//

#pragma once

#ifndef PPGSO_BORDER_H
#define PPGSO_BORDER_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"


class border final : public Object {

    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    public:

        int border_position;

        border();
        border(int border_position);

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


#endif //PPGSO_BORDER_H
