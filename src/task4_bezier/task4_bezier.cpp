// Task 4 - Render the letter R using OpenGL
//        - Implement a function to generate bezier curve points
//        - Generate multiple points on each bezier curve
//        - Draw lines connecting the generated points using OpenGL

#include <iostream>
#include <vector>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

using namespace std;
using namespace glm;
using namespace ppgso;

class BezierWindow : public Window {
private:

  // Control points for the bezier curves
  // First curve is 4 control points
  // Rest of the curves are 3 control points, each reusing the last curve end
  // Defines the letter "R"
  vector<vec2> controlPoints = {
      //X, Y
      {-0.5, 0},
      {-0.5, -0.5},
      {0, -0.5},
      {0, 0},

      { 0, 0.25},
      { 0,  0.75},
      { 0,   1},



      {0.5,   1},
      {0.5,  0.5},
      {0,   0.5},

      {0.5,   0.5},
      {0.5,   0},
      {0,   0},
      {0,   0},
  };

  // This will hold the bezier curve geometry once we generate it
  vector<vec3> points;

  // GLSL Program to use for rendering
  Shader program = {color_vert_glsl, color_frag_glsl};

  // These numbers are used to pass buffer data to OpenGL
  GLuint vao = 0, vbo = 0;

  // Compute points for Bezier curve using 4 control points
  vec2 bezierPoint(const vec2 &p0, const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec2 &p4, const vec2 &p5, const vec2 &p6, const float t) {
    // TODO: Compute point on the Bezier curve
    vec2 l_vecs[21] = {};

    l_vecs[0] = lerp(p0, p1, t);
    l_vecs[1] = lerp(p1, p2, t);
    l_vecs[2] = lerp(p2, p3, t);
    l_vecs[3] = lerp(p3, p4, t);
    l_vecs[4] = lerp(p4, p5, t);
    l_vecs[5] = lerp(p5, p6, t);

    l_vecs[6] = lerp(l_vecs[0], l_vecs[1], t);
    l_vecs[7] = lerp(l_vecs[1], l_vecs[2], t);
    l_vecs[8] = lerp(l_vecs[2], l_vecs[3], t);
    l_vecs[9] = lerp(l_vecs[3], l_vecs[4], t);
    l_vecs[10] = lerp(l_vecs[4], l_vecs[5], t);

    l_vecs[11] = lerp(l_vecs[6], l_vecs[7], t);
    l_vecs[12] = lerp(l_vecs[7], l_vecs[8], t);
    l_vecs[13] = lerp(l_vecs[8], l_vecs[9], t);
    l_vecs[14] = lerp(l_vecs[9], l_vecs[10], t);

    l_vecs[15] = lerp(l_vecs[11], l_vecs[12], t);
    l_vecs[16] = lerp(l_vecs[12], l_vecs[13], t);
    l_vecs[17] = lerp(l_vecs[13], l_vecs[14], t);

    l_vecs[18] = lerp(l_vecs[15], l_vecs[16], t);
    l_vecs[19] = lerp(l_vecs[16], l_vecs[17], t);

    l_vecs[20] = lerp(l_vecs[18], l_vecs[19], t);

    return {l_vecs[20]};
  }

  // Compute points for a sequence of Bezier curves defined by a vector of control points
  // Each bezier curve will reuse the end point of the previous curve
  // count - Number of points to generate on each curve
  void bezierShape(int count) {
    int i;
    for(i = 1; i < (int) controlPoints.size(); i+=6) {
      for (int j = 0; j <= count; j++) {
        // TODO: Generate points for each Bezier curve and insert them

        vec2 point = bezierPoint(controlPoints[i - 1], controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], controlPoints[i + 4], controlPoints[i + 5], (j / (float)count));
        points.emplace_back(point, 0);
      }
    }

    cout << "debug " << i << endl;
  }

public:
  BezierWindow() : Window{"task4_bezier", SIZE, SIZE} {
    // Generate Bezier curve points
    bezierShape(15);

    // Generate a vertex array object
    // This keeps track of what attributes are associated with buffers
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object, this will feed data to the vertex shader
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // TODO: Pass the control points to the GPU
    glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(vec3), this->points.data(), GL_STATIC_DRAW);

    // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input
    auto position_attrib = program.getAttribLocation("Position");
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(position_attrib);

    // Set model matrix to identity
    program.setUniform("ModelMatrix", mat4{1});
    program.setUniform("ViewMatrix", mat4{1});
    program.setUniform("ProjectionMatrix", mat4{1});

    // Set the color uniform
    program.setUniform("OverallColor", vec3{1.0f,1.0f,1.0f});
  }

  ~BezierWindow() final {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }

  void onIdle() final {
    // Set gray background
    glClearColor(0.5f,0.5f,0.5f,0);

    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw shape
    glBindVertexArray(vao);

    // TODO: Define the correct render mode
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)points.size());
  }
};

int main() {
  // Create our window
  BezierWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
