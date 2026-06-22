#include <iostream>
#include "Vec.h"
#include "Camera.h"
camera cam;

int WIDTH = 640;
int HEIGHT = 430;

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

void render(Vec3 &CameraPos, double &yaw, double &pitch,int WIDTH, int HEIGHT, double r_s);

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
        render(cam.cameraPos, cam.yaw, cam.pitch, WIDTH, HEIGHT, 1.0);
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, framebuffer.data());

        glfwSwapBuffers(window);
    }

    // End loop:
    glfwTerminate();
    return 0;
}