#include <iostream>
#include "Vec.h"
#include "Camera.h"
camera cam;

int WIDTH = 1480;
int HEIGHT = 860;

// Pixel buffer:
std::vector<float> framebuffer(WIDTH * HEIGHT * 3, 0.0f);

// Resizing Window:
void onResize(GLFWwindow*, int w, int h)
{
    WIDTH = w;
    HEIGHT = h;
    framebuffer.assign(WIDTH * HEIGHT * 3, 0.0f);
    glViewport(0, 0, WIDTH, HEIGHT);
}

// Pixel Buffer: setting each pixel:
void setPixel(float x, float y, float red, float green, float blue)
{
    int index = (y * WIDTH + x) * 3;
    framebuffer[index + 0] = red;
    framebuffer[index + 1] = green;
    framebuffer[index + 2] = blue;
}

int main()
{
    // Making the window with the screen dimensions:
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Physics_eng - v1: Basic Mechanics and Ray Tracing", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Changing window size in real time:
    glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &WIDTH, &HEIGHT);
    framebuffer.assign(WIDTH * HEIGHT * 3, 0.0f);
    glfwSetWindowSizeCallback(window, onResize);

    float lastTime = glfwGetTime(); float deltaTime = 0;

    // Main loop:
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        deltaTime = glfwGetTime() - lastTime; lastTime = glfwGetTime();
        cam.update(window, deltaTime);

        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, framebuffer.data());

        glfwSwapBuffers(window);
    }

    // End loop:
    glfwTerminate();
    return 0;
}