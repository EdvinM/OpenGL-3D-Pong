// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "generator.h"
#include "player.h"
#include "space.h"
#include "asteroid.h"

using namespace std;
using namespace glm;
using namespace ppgso;

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public Window {
private:
  Scene scene;
  bool animate = true;

  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene() {
    scene.objects.clear();

    // Create a camera
    auto camera = make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -30.0f;
    scene.camera = move(camera);

    // Add space background
    auto space = make_unique<Space>();
    space->position.x = 0;
    space->position.y = 0;
    space->rotation.x = 90.0f;
    //space->rotation.z = 90.0f;
    //space->rotation.y = 110.0f;
    //scene.objects.push_back(move(space));

    // Add player to the scene
    auto player1 = make_unique<Player>(GLFW_KEY_W, GLFW_KEY_S, 1);
    player1->position.x = 10;
    scene.objects.push_back(move(player1));

    auto player2 = make_unique<Player>(GLFW_KEY_UP, GLFW_KEY_DOWN, 2);
    player2->position.x = -10;
    scene.objects.push_back(move(player2));

    auto obj = make_unique<Asteroid>();
    obj->position.x = 0.0f;
    obj->position.y = 0.0f;
    scene.objects.push_back(move(obj));
  }

public:
  /*!
   * Construct custom game window
   */
  SceneWindow() : Window{"gl9_scene", WIDTH, HEIGHT} {
    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    initScene();
  }

  /*!
   * Handles pressed key when the window is focused
   * @param key Key code of the key being pressed/released
   * @param scanCode Scan code of the key being pressed/released
   * @param action Action indicating the key state change
   * @param mods Additional modifiers to consider
   */
  void onKey(int key, int scanCode, int action, int mods) override {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      initScene();
    }

    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }
  }

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
  void onCursorPos(double cursorX, double cursorY) override {
    scene.cursor.x = cursorX;
    scene.cursor.y = cursorY;
  }

  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
  void onMouseButton(int button, int action, int mods) override {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {

      scene.cursor.left = action == GLFW_PRESS;

      if (scene.cursor.left) {
        // Convert pixel coordinates to Screen coordinates
        float u = (scene.cursor.x / width - 0.5f) * 2.0f;
        float v = - (scene.cursor.y / height - 0.5f) * 2.0f;

        // Get mouse pick vector in world coordinates
        auto direction = scene.camera->cast(u, v);
        auto position = scene.camera->position;

        // Get all objects in scene intersected by ray
        auto picked = scene.intersect(position, direction);

        // Go through all objects that have been picked
        for (auto &obj: picked) {
          // Pass on the click event
          obj->onClick(scene);
        }
      }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      scene.cursor.right = action == GLFW_PRESS;
    }
  }

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update and render all objects
    scene.update(dt);
    scene.render();
  }
};

int main() {
  // Initialize our window
  SceneWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
