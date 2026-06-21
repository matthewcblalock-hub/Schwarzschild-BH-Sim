#pragma once
#include "Vec.h"

// This file contains all of the camera Classes/Functions

class camera{

    // Checking mouse conditions and initializing position:
    double lastMouseX = 0.0, lastMouseY = 0.0;
    bool firstClick = true;

    // Values that feel good (no real meaning):
    float sensitivity = 0.00009f;
    float speed = 600.0f;

    float limit = M_PI/2 - 0.01;

    public:

    // Initialize componets:
    Vec3 cameraPos{0,3,-5};
    Vec3 velocity{0,0,0};
    double yaw, pitch;

    void update(GLFWwindow* window, float deltaTime){
        Vec3 targetVelocity(0, 0, 0);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            if (firstClick)
                {
                    lastMouseX = mouseX;
                    lastMouseY = mouseY;
                    firstClick = false;
                }

            double deltaX = mouseX - lastMouseX;
            double deltaY = mouseY - lastMouseY;
            yaw += deltaX * sensitivity;
            pitch += deltaY * sensitivity;
            if(pitch > limit) pitch = limit;
            if(pitch < -limit) pitch = -limit;

        }

        else
        {
            firstClick = true;
        }

         // Checking for clicks on user keyboard to be used for camera movement.
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {targetVelocity.x += sin(yaw) * speed * deltaTime; targetVelocity.z += cos(yaw) * speed * deltaTime;}
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {targetVelocity.x -= sin(yaw) * speed * deltaTime; targetVelocity.z -= cos(yaw) * speed * deltaTime;}
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {targetVelocity.x += cos(yaw) * speed * deltaTime; targetVelocity.z += -sin(yaw) * speed * deltaTime;}
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {targetVelocity.x -= cos(yaw) * speed * deltaTime; targetVelocity.z -= -sin(yaw) * speed * deltaTime;}
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) targetVelocity.y -= speed*deltaTime;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) targetVelocity.y += speed*deltaTime;

        // Calculating velocity for Camera to automatically slow down and adding smoothing:
        float smoothing = 1.0f;

        float t = 1.0f - std::exp(-smoothing * deltaTime);
        velocity.x += (targetVelocity.x - velocity.x) * t;
        velocity.y += (targetVelocity.y - velocity.y) * t;
        velocity.z += (targetVelocity.z - velocity.z) * t;

        if(cameraPos.y <= 0.1){
            cameraPos.y += abs(velocity.y * deltaTime);
        }

        // Updating camera position with aspects to velocity and time:
        cameraPos.x += velocity.x * deltaTime;
        cameraPos.y += velocity.y * deltaTime;
        cameraPos.z += velocity.z * deltaTime;

    }    
};