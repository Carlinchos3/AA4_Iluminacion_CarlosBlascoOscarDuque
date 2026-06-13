// Camera.h
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Camera 
{
public:
    Camera(glm::vec3 position, float yaw, float pitch);

    void MoveForward(float deltaTime);
    void MoveRight(float deltaTime);
    void Rotate(float offsetX, float offsetY);

    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetForward() const;

private:
    void UpdateVectors();

    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_right;
    glm::vec3 m_up;

    float m_yaw;
    float m_pitch;
    float m_speed = 5.0f;
    float m_sensitivity = 0.1f;
};

