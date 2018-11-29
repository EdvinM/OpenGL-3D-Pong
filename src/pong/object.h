#pragma once
#include <memory>
#include <list>
#include <map>
#include <vector>

#include <glm/glm.hpp>

// Forward declare a scene
class Scene;

/*!
 *  Abstract scene object interface
 *  All objects in the scene should be able to update and render
 *  Generally we also want to keep position, rotation and scale for each object to generate a modelMatrix
 */
struct t_keyframe {
    glm::vec3 keyframePosition = {0, 0, 0};
    glm::vec3 keyframeRotation = {0, 0, 0};
    glm::vec3 keyframeScale = {1, 1, 1};
    float duration = 100;
};

class Object {

protected:
    float keyframeDuration = 0;
    int processedKeyframes = 0;

    std::vector<t_keyframe> keyframeAnimation;

public:
  // Define default constructors as this is an abstract class
  Object() = default;
  Object(const Object&) = default;
  Object(Object&&) = default;
  virtual ~Object() {};

  /*!
   * Update Object parameters, usually used to update the modelMatrix based on position, scale and rotation
   *
   * @param scene - Reference to the Scene the object is rendered in
   * @param dt - Time delta for animation purposes
   * @return true to delete the object
   */
  virtual bool update(Scene &scene, float dt) = 0;

  /*!
   * Render the object in the scene
   * @param scene
   */
  virtual void render(Scene &scene) = 0;


  /*!
   * Event to be called when the object is clicked
   * @param scene
   */
  virtual void onClick(Scene &scene) {};

  // Object properties
  glm::vec3 position{0,0,0};
  glm::vec3 rotation{0,0,0};
  glm::vec3 scale{1,1,1};
  glm::mat4 modelMatrix{1};

  float duration;
  float effectDuration;

protected:
  /*!
   * Generate modelMatrix from position, rotation and scale
   */
  void generateModelMatrix();

  void updateKeyFrame();

  void addKeyFrame(int duration, glm::vec3 keyFrameRotation, glm::vec3 keyFrameScale, glm::vec3 keyFramePosition);

  glm::vec3 linearInterpolation(glm::vec3 a, glm::vec3 b, float t);
};

