#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "object.h"

using namespace std;
using namespace glm;

void Object::generateModelMatrix() {
  modelMatrix =
          glm::translate(mat4(1.0f), position)
          * glm::orientate4(rotation)
          * glm::scale(mat4(1.0f), scale);
}

void Object::updateKeyFrame() {

  if(this->keyframeAnimation.size() <= 0)
    return;

  t_keyframe current = keyframeAnimation[processedKeyframes];
  t_keyframe next = keyframeAnimation[(processedKeyframes + 1) % this->keyframeAnimation.size()];

  float t = keyframeDuration / current.duration;

  position = linearInterpolation(current.keyframePosition, next.keyframePosition, t);
  scale = linearInterpolation(current.keyframeScale, next.keyframeScale, t);
  rotation = linearInterpolation(current.keyframeRotation, next.keyframeRotation, t);

  keyframeDuration++;
  if (keyframeDuration >= current.duration) {
    keyframeDuration = 0;
    processedKeyframes = (processedKeyframes + 1) % this->keyframeAnimation.size();
  }
}

glm::vec3 Object::linearInterpolation(glm::vec3 a, glm::vec3 b, float t){
  glm::vec3 result = (1 - t) * a + t * b;
  return result;
}

void Object::addKeyFrame(int duration, glm::vec3 keyFrameRotation, glm::vec3 keyFrameScale, glm::vec3 keyFramePosition){
  t_keyframe newKeyframe;
  newKeyframe.duration = duration;
  newKeyframe.keyframeRotation = keyFrameRotation;
  newKeyframe.keyframeScale = keyFrameScale;
  newKeyframe.keyframePosition = keyFramePosition;

  this->keyframeAnimation.push_back(newKeyframe);
}

void Object::empltyKeyFrames() {
  if(this->keyframeAnimation.size() <= 0)
    return;

  this->keyframeAnimation.clear();
}