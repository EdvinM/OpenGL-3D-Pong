// Task 5 - Draw a 2D shape using polygons and animate it
//        - Encapsulate the shape using a class
//        - Use color_vert/frag shader to display the polygon
//        - Animate the object position, rotation and scale.

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

const unsigned int SIZE = 512;

// Object to represent 2D OpenGL shape
class Shape {
private:
  // 2D vectors define points/vertices of the shape
  // TODO: Define your shape points
  vector<vec3> vetrices = {
    {-0.3, 0.6, 0},
    {0.3, 0.6, 0},

    {-0.3, 0.35, 0},
    {0.3, 0.35, 0},

    {-0.1, 0.35, 0},
    {0.1, 0.35, 0},

    {-0.1, -0.2, 0},
    {0.1, -0.2, 0}
  };

  // Structure representing a triangular face, usually indexes into vertices
  struct Face {
    // TODO: Define your face structure
    int p1, p2, p3;
  };

  // Indices define triangles that index into vertices
  // TODO: Define your mesh indices
  vector<Face> mesh = {
          {0,1,2}, //1 Trojuholnik
          {2, 3,1}, //2 Trojuholnik

          {4, 5, 6}, //, //3 Trojuholnik
          {7, 6, 5} // 4 Trojuholnik
  };

  // Program to associate with the object
  Shader program = {color_vert_glsl, color_frag_glsl};

  // These will hold the data and object buffers
  GLuint vao, vbo, cbo, ibo;
  mat4 modelMatrix{1};
public:
  // Public attributes that define position, color ..
  vec3 position{0,0,0};
  vec3 rotation{0,0,0};
  vec3 scale{1,1,1};
  vec3 color{1,0,0};

  // Initialize object data buffers
  Shape() {
    // Copy data to OpenGL
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Copy positions to gpu
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vetrices.size() * sizeof(vec3), vetrices.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto position_attrib = program.getAttribLocation("Position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy mesh indices to gpu
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(Face), mesh.data(), GL_STATIC_DRAW);

    // Set projection matrices to identity
    program.setUniform("ProjectionMatrix", mat4{1});
    program.setUniform("ViewMatrix", mat4{1});
  };
  // Clean up
  ~Shape() {
    // Delete data from OpenGL
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &cbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }

  // Set the object transformation matrix
  void update() {
    // TODO: Compute transformation by scaling, rotating and then translating the shape
    modelMatrix = translate(mat4{1}, position) * rotate(mat4{1}, rotation.z, vec3{0,0,1}) * glm::scale(mat4{1}, scale);
  }

  // Draw polygons
  void render(){
    // Update transformation and color uniforms in the shader
    program.use();
    program.setUniform("OverallColor", color);
    program.setUniform("ModelMatrix", modelMatrix);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh.size() * 3, GL_UNSIGNED_INT, 0);
  };
};

class ShapeWindow : public Window {
private:
  Shape shape1, shape2, shape3;
public:
  ShapeWindow() : Window{"task5_2dshapes", SIZE, SIZE} {
    shape1.color = {1,0,0};
    shape2.color = {0,1,0};
    shape3.color = {0,0,1};
  }

  void onIdle() {
    // Set gray background
    glClearColor(.1f,.1f,.1f,1.0f);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Move and Render shape\    // Get time for animation
    auto t = (float) glfwGetTime();

    // TODO: manipuate shape1 and shape2 position to rotate clockwise
    shape1.position = {0, 0.2, 0};
    shape2.position = {sin(t), -0.5, 0};
    shape3.position = {shape2.position.x, sin(t), 0};

    // Manipulate shape size
    shape1.scale = {abs(cos(t)),abs(cos(t)), 1};
    shape2.scale = {0.3, 0.3, 0};
    shape3.scale = {0.3, 0.3, 0};

    shape3.rotation.z = t * 6.0f;

    // Update and render each shape
    shape1.update();
    shape2.update();
    shape3.update();

    shape1.render();
    shape2.render();
    shape3.render();
  }
};

int main() {
  // Create our window
  auto window = ShapeWindow{};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
