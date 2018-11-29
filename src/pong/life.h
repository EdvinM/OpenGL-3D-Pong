//
// Created by Edvin Mako on 10/11/2018.
//

#ifndef PPGSO_LIFE_H
#define PPGSO_LIFE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "Ball.h"

class Life final : public Object {


    private:
        // Static resources (Shared between instances)
        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

        //Things for mtl
        static std::vector<tinyobj::material_t> material;
        static std::map<std::string, int> material_map;

        float time;

    public:
        Life();

        Life(glm::vec3 rotation, glm::vec3 scale, glm::vec3 position);

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

        bool active;

};


#endif //PPGSO_LIFE_H
