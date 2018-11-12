#include "scene.h"
#include <time.h>

Scene::Scene() {
  srand (time(NULL));
}

glm::vec3 Scene::RotateColor(glm::vec3& inColor , float H){
  float U = glm::cos(H * 3.14159f / 180.0f);
  float W = glm::sin(H * 3.14159f / 180.0f);

  glm::vec3 ret;
  ret.r = (0.299f + 0.701f * U + 0.168f * W) * inColor.r + (0.587f - 0.587f * U + 0.330f * W) * inColor.g + (0.114f - 0.114f * U - 0.497f * W) * inColor.b;
  ret.g = (0.299f - 0.299f* U - 0.328f * W) * inColor.r + (0.587f + 0.413f * U + 0.035f * W) * inColor.g + (0.114f - 0.114f * U + 0.292f * W) * inColor.b;
  ret.b = (0.299f - 0.3f * U + 1.25f * W) * inColor.r + (0.587f - 0.588f * U - 1.05f * W) * inColor.g + (0.114f + 0.886f * U - 0.203f * W) * inColor.b;
  return ret;

}

void Scene::update(float time) {
  camera->update();

//  lightColor = Scene::RotateColor(lightColor,t);
//   t += 0.0005f;
//   if (t >= 120.0f)
//     t = 0.0f;

  // Use iterator to update all objects so we can remove while iterating
  auto i = std::begin(objects);

  while (i != std::end(objects)) {
    // Update and remove from list if needed
    auto obj = i->get();
    if (!obj->update(*this, time))
      i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;
  }
}

void Scene::render() {
  // Simply render all objects
  for ( auto& obj : objects )
    obj->render(*this);
}

std::vector<Object*> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
  std::vector<Object*> intersected = {};
  for(auto& object : objects) {
    // Collision with sphere of size object->scale.x
    auto oc = position - object->position;
    auto radius = object->scale.x;
    auto a = glm::dot(direction, direction);
    auto b = glm::dot(oc, direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto dis = b * b - a * c;

    if (dis > 0) {
      auto e = sqrt(dis);
      auto t = (-b - e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }

      t = (-b + e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }
    }
  }

  return intersected;
}
