#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

class InputManager {
public:
    InputManager(Camera& camera);

    void ProcessKeyboard(GLFWwindow* window, float deltaTime);
    void ProcessMouse(float offsetX, float offsetY);

    static void MouseCallback(GLFWwindow* window, double posX, double posY);

    bool IsFlashlightOn() const;

    void PassUniforms(GLuint program) const;

private:
    Camera& camera;

    float lastX = 0.0f;
    float lastY = 0.0f;
    bool isFirstMouse = true;

    bool flashlightOn = false;
    bool flashlightPressed = false;
};

