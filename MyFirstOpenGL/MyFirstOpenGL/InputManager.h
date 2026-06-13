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

private:
    Camera& m_camera;

    float s_lastX = 0.0f;
    float s_lastY = 0.0f;
    bool Is_firstMouse = true;

    bool flashlightOn = false;
    bool flashlightPressed = false;
};

