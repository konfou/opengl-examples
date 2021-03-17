#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <thread>
#include <chrono>
#define CHECK_ABS false
#if CHECK_ABS
#include <cmath>
#endif

/**
 * Simple GLFW/GLEW-based demo of colliding squares (Modern OpenGL).
 * checkCollision() uses either absolute difference or AABB.
 */

void delay(double seconds);
bool checkCollision(float x1, float y1, float x2, float y2, float side);
void drawSquare(float x, float y, float side,
                float r=1.0, float g=1.0, float b=1.0);
GLuint createShaderProgram(void);

//globals
GLuint shaderProgram;
float vertices[] = {
  //positions
  0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f
};
unsigned int indices[] = {
  //triangles
  0, 1, 3,
  1, 2, 3
};

int main(void)
{
  //initialize
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  auto window = glfwCreateWindow(800, 600, "", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  auto err = glewInit();
  if (GLEW_OK != err) {
    std::cout << "Error: " << glewGetErrorString(err) << std::endl;
  }

  //square opts
  float
    side = 0.2,
    x1 = -1.0, y1 = 0.0, x2 = 1.0, y2 = 0.1,
    v = 0.01;

  //objects
  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  //shader black magic
  shaderProgram = createShaderProgram();
  glUseProgram(shaderProgram);

  //target framerate
  const auto invFPS = 0.016;
  auto t0 = glfwGetTime();

  //render loop
  while (!glfwWindowShouldClose(window)) {
    //target framerate
    auto t1 = glfwGetTime();
    auto dt = t1 - t0;
    t0 = t1;
    if (dt < invFPS)
      delay(invFPS - dt);

    //drawing code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSquare(x1, y1, side);
    drawSquare(x2, y2, side, 0.0);

    //square move and collision detection
    x1 += v;
    x2 -= v;
    if (checkCollision(x1, y1, x2, y2, side))
      v = -v;

    //window
    glfwSwapBuffers(window);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}

bool checkCollision(float x1, float y1, float x2, float y2, float side)
{
#if CHECK_ABS
  return
    std::abs(x2 - x1) <= side &&
    std::abs(y2 - y1) <= side;
#else // AABB
  return
    x1 + side >= x2 &&
    x2 + side >= x1 &&
    y1 + side >= y2 &&
    y2 + side >= y1;
#endif
}

void delay(double seconds)
{
  std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
}

void drawSquare(float x, float y, float side,
                float r, float g, float b)
{
  auto half = side/2;
  auto vertexColorLocation = glGetUniformLocation(shaderProgram, "color");
  glUniform4f(vertexColorLocation, r, g, b, 1.0f);
  vertices[0] = x + half; vertices[1]  = y + half;
  vertices[3] = x + half; vertices[4]  = y - half;
  vertices[6] = x - half; vertices[7]  = y - half;
  vertices[9] = x - half; vertices[10] = y + half;
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLuint createShaderProgram(void)
{
  const char *vertexShaderSource =
    "#version 400\n"
    "layout (location = 0) in vec3 pos;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(pos, 1.0);\n"
    "}\0";

  const char *fragmentShaderSource =
    "#version 400\n"
    "out vec4 FragColor;\n"
    "uniform vec4 color;\n"
    "void main()\n"
    "{\n"
    "  FragColor = color;\n"
    "}\0";

  auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  auto shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
