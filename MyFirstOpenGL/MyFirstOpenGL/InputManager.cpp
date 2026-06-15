#include "InputManager.h"
#include <iostream>

InputManager::InputManager(Camera& camera) : camera(camera) {}

void InputManager::ProcessKeyboard(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.MoveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.MoveForward(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.MoveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.MoveRight(-deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !flashlightPressed) {
        flashlightOn = !flashlightOn;
        flashlightPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        flashlightPressed = false;
}

void InputManager::ProcessMouse(float offsetX, float offsetY) {
    camera.Rotate(offsetX, offsetY);
}

void InputManager::MouseCallback(GLFWwindow* window, double posX, double posY) {
    InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!manager) return;

    if (manager->isFirstMouse) {
        manager->lastX = (float)posX;
        manager->lastY = (float)posY;
        manager->isFirstMouse = false;
    }

    float offsetX = (float)posX - manager->lastX;
    float offsetY = (float)posY - manager->lastY;

    manager->lastX = (float)posX;
    manager->lastY = (float)posY;

    manager->ProcessMouse(offsetX, offsetY);
}

bool InputManager::IsFlashlightOn() const {
    return flashlightOn;
}

void InputManager::PassUniforms(GLuint program) const 
{
    glUniform3fv(glGetUniformLocation(program, "flashlightPos"), 1, glm::value_ptr(camera.GetPosition()));
    glUniform3fv(glGetUniformLocation(program, "flashlightDir"), 1, glm::value_ptr(camera.GetForward()));
    glUniform1i(glGetUniformLocation(program, "flashlightOn"), flashlightOn ? 1 : 0);
}