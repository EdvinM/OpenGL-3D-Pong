//
// Created by Edvin Mako on 27/11/2018.
//

#ifndef PPGSO_SPLITTER_H
#include <memory>

#include <ppgso/ppgso.h>

#include "RotatingBall.h"
#include "../Ball.h"
#include "../scene.h"
#include "../object.h"

#define PPGSO_SPLITTER_H


class Splitter : public Object {
private:
    Ball ball;
    std::vector<std::unique_ptr<RotatingBall>> rotatingBalls;

public:
    Splitter();
    Splitter(glm::vec3 position);

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


#endif //PPGSO_SPLITTER_H
