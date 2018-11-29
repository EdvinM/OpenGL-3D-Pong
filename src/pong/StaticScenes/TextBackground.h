//
// Created by Edvin Mako on 2018-11-29.
//

#ifndef PPGSO_TEXTBACKGROUND_H
#define PPGSO_TEXTBACKGROUND_H

#include <ppgso/ppgso.h>

#include "src/pong/object.h"

class TextBackground final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec2 textureOffset;
public:
    /*!
     * Create new Space background
     */
    TextBackground();

    /*!
     * Update space background
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render space background
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};


#endif //PPGSO_TEXTBACKGROUND_H
