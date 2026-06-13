#include "InputManager.h"
#include <iostream>

InputManager::InputManager(Camera& camera) : m_camera(camera) {}

void InputManager::ProcessKeyboard(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.MoveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.MoveForward(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.MoveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.MoveRight(-deltaTime);
    
    //Linterna
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !flashlightPressed) {
        flashlightOn = !flashlightOn;
        flashlightPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        flashlightPressed = false;
}

void InputManager::ProcessMouse(float offsetX, float offsetY) {
    m_camera.Rotate(offsetX, offsetY);
}

void InputManager::MouseCallback(GLFWwindow* window, double posX, double posY) {
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!manager) return;

    if (manager->Is_firstMouse) {
        manager->s_lastX = (float)posX;
        manager->s_lastY = (float)posY;
        manager->Is_firstMouse = false;
    }

    float offsetX = (float)posX - manager->s_lastX;
    float offsetY = (float)posY - manager->s_lastY;

    manager->s_lastX = (float)posX;
    manager->s_lastY = (float)posY;

    manager->ProcessMouse(offsetX, offsetY);
}

bool InputManager::IsFlashlightOn() const {
    return flashlightOn;
    std::cout << IsFlashlightOn() << std::endl;
}