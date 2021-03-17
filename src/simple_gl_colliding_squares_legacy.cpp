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
 * Simple GLFW/GLEW-based demo of colliding squares (Legacy OpenGL).
 * checkCollision() uses either absolute difference or AABB.
 */

void delay(double seconds);
bool checkCollision(float x1, float y1, float x2, float y2, float side);
void drawSquare(float x, float y, float side,
                float r=1.0, float g=1.0, float b=1.0);

int main(void)
{
  //initialize
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
  glColor3f(r, g, b);
  glBegin(GL_QUADS);
  {
    glVertex2d(x + half, y + half);
    glVertex2d(x + half, y - half);
    glVertex2d(x - half, y - half);
    glVertex2d(x - half, y + half);
  }
  glEnd();
}
