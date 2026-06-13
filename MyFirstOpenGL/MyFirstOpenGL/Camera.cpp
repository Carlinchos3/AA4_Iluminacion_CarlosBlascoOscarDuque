#include "Camera.h"

Camera::Camera(glm::vec3 position, float yaw, float pitch)
    : m_position(position), m_yaw(yaw), m_pitch(pitch) {
    UpdateVectors();
}

void Camera::MoveForward(float deltaTime) {
    m_position += m_forward * m_speed * deltaTime;
}

void Camera::MoveRight(float deltaTime) {
    m_position += m_right * m_speed * deltaTime;
}

void Camera::Rotate(float offsetX, float offsetY) {
    m_yaw += offsetX * m_sensitivity;
    m_pitch -= offsetY * m_sensitivity;

    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    UpdateVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::vec3 Camera::GetPosition() const {
    return m_position;
}

glm::vec3 Camera::GetForward() const {
    return m_forward;
}

void Camera::UpdateVectors() {
    glm::vec3 forward;
    forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    forward.y = sin(glm::radians(m_pitch));
    forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_forward = glm::normalize(forward);
    m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}

void Camera::PassUniforms(GLuint program) const 
{
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(GetViewMatrix()));
}