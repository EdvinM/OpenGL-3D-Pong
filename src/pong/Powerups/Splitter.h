//
// Created by Edvin Mako on 27/11/2018.
//

#ifndef PPGSO_SPLITTER_H
#include <memory>

#include <ppgso/ppgso.h>

#include "RotatingBall.h"
#include "QuakePU.h"
#include "../Ball.h"
#include "../scene.h"
#include "../object.h"

#define PPGSO_SPLITTER_H


class Splitter : public Object {
private:
    std::unique_ptr<QuakePU> ball;
    std::vector<std::unique_ptr<RotatingBall>> rotatingBalls;

    bool spawned;

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
