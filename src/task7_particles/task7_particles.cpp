// Task 7 - Dynamically generate objects in a 3D scene
//        - Implement a particle system where particles have position and speed
//        - Any object can be a generator and can add objects to the scene
//        - Create dynamic effect such as fireworks, rain etc.
//        - Encapsulate camera in a class

#include <iostream>
#include <vector>
#include <map>
#include <list>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

const unsigned int SIZE = 512;

class Camera {
public:
  // TODO: Add parameters
  mat4 viewMatrix;
  mat4 projectionMatrix;

  vec3 position = {0, 0, -25};
  vec3 view     = {0, 0, 1};
  vec3 up       = {0, 1, 0};

  /// Representaiton of
  /// \param fov - Field of view (in degrees)
  /// \param ratio - Viewport ratio (width/height)
  /// \param near - Distance of the near clipping plane
  /// \param far - Distance of the far clipping plane
  Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f) {
    // TODO: Initialize perspective projection (hint: glm::perspective)

      projectionMatrix = perspective((PI / 180.0f) * fov, ratio, near, far);
  }

  /// Recalculate viewMatrix from position, rotation and scale
  void update() {
    // TODO: Update viewMatrix (hint: glm::lookAt)

    viewMatrix = lookAt(position, position + view, up);
  }
};

/// Abstract renderable object interface
class Renderable; // Forward declaration for Scene
using Scene = list<unique_ptr<Renderable>>; // Type alias

class Renderable {
public:
  // Virtual destructor is needed for abstract interfaces
  virtual ~Renderable() = default;

  /// Render the object
  /// \param camera - Camera to use for rendering
  virtual void render(const Camera& camera) = 0;

  /// Update the object. Useful for specifing animation and behaviour.
  /// \param dTime - Time delta
  /// \param scene - Scene reference
  /// \return - Return true to keep object in scene
  virtual bool update(float dTime, Scene &scene) = 0;
};

/// Basic particle that will render a sphere
/// TODO: Implement Renderable particle
class Particle final : public Renderable {
  // Static resources shared between all particles
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;

  // TODO: add more parameters as needed
public:
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 scale = {1, 1, 1};
    vec3 rotation = {0, 0, 0};
    vec3 wind_speed = {-linearRand(0.3f, 0.6f), 0, 0};
    mat4 modelMatrix{1};

    float age = 0;
    float speed = 25.0f ;
  /// Construct a new Particle
  /// \param p - Initial position
  /// \param s - Initial speed
  /// \param c - Color of particle
  Particle(vec3 p, vec3 s, vec3 c) {

      position = p;
      direction = s;
      color = c;
      age = 0;

    // First particle will initialize resources
    if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = make_unique<Mesh>("sphere.obj");
  }

  bool update(float dTime, Scene &scene) override {
    // TODO: Animate position using speed and dTime.
    // - Return true to keep the object alive
    // - Returning false removes the object from the scene
    // - hint: you can add more particles to the scene here also

      age += dTime;

      position += direction * dTime * speed;

      if(age >= 1.5f) {
          position.x = position.x + (sin(age * 25) / 50.0f / (age * 2));
      }

      scale -= 0.00035f;

      if(age <= 2.5f) {
          color.g += dTime * 0.5f;
      }
      else {
          position += (dTime * speed * wind_speed);
      }

      modelMatrix = glm::translate(mat4(1.0f), position) * glm::scale(mat4(1.0f), scale);

      return age < 4.0f;
  }

  void render(const Camera& camera) override {
    // TODO: Render the object
    // - Use the shader
    // - Setup all needed shader inputs
    // - hint: use OverallColor in the color_vert_glsl shader for color
    // - Render the mesh
      shader->use();

      //shader->setUniform("LightDirection", scene.lightDirection);

      shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
      shader->setUniform("ViewMatrix", camera.viewMatrix);

      shader->setUniform("ModelMatrix", modelMatrix);
//        shader->setUniform("Texture", *texture);
      shader->setUniform("OverallColor", color);
      mesh->render();
  }
};
// Static resources need to be instantiated outside of the class as they are globals
std::unique_ptr<ppgso::Mesh> Particle::mesh;
std::unique_ptr<ppgso::Shader> Particle::shader;

class ParticleWindow : public Window {
private:
  // Scene of objects
  Scene scene;

  // Create camera
  Camera camera = {120.0f, (float)width/(float)height, 1.0f, 400.0f};

  // Store keyboard state
  map<int, int> keys;
public:
  ParticleWindow() : Window{"task7_particles", SIZE, SIZE} {
    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  }

  void onKey(int key, int scanCode, int action, int mods) override {
    // Collect key state in a map
    keys[key] = action;
    if (keys[GLFW_KEY_SPACE]) {
      // TODO: Add renderable object to the scene

        for(int i = 0; i < 20; i++) {
            vec3 pos = {linearRand(0.0f, 20.0f), linearRand(-27.0f, -24.0f), linearRand(0.0f, 7.0f)};
            vec3 direction = {0, 0.5f, 0};
            vec3 color = {1, 0, 0};

            unique_ptr<Particle> renderable = make_unique<Particle>(pos, direction, color);
            scene.push_back(move(renderable));
        }
    }
  }

  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();
    // Compute time delta
    float dTime = (float)glfwGetTime() - time;
    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.1f,.1f,.1f,1.0f);

    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update all objects in scene
    // Because we need to delete while iterating this is implemented using c++ iterators
    // In most languages mutating the container during iteration is undefined behaviour
    auto i = std::begin(scene);
    while (i != std::end(scene)) {
      // Update object and remove from list if needed
      auto obj = i->get();
      if (!obj->update(dTime, scene))
        i = scene.erase(i);
      else
        ++i;
    }

    // Render every object in scene
    camera.update();
    for(auto& object : scene) {
      object->render(camera);
    }
  }
};

int main() {
  // Create new window
  auto window = ParticleWindow{};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
