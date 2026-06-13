#include "Camera.h"

Camera::Camera(glm::vec3 _position, float _yaw, float _pitch)
    : position(_position), yaw(_yaw), pitch(_pitch) {
    UpdateVectors();
}

void Camera::MoveForward(float _deltaTime) {
    position += forward * speed * _deltaTime;
}

void Camera::MoveRight(float _deltaTime) {
    position += right * speed * _deltaTime;
}

void Camera::Rotate(float _offsetX, float _offsetY) {
    yaw += _offsetX * sensitivity;
    pitch -= _offsetY * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    UpdateVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::GetPosition() const {
    return position;
}

glm::vec3 Camera::GetForward() const {
    return forward;
}

void Camera::UpdateVectors() {
    glm::vec3 _forward;
    _forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _forward.y = sin(glm::radians(pitch));
    _forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    forward = glm::normalize(_forward);
    right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));
}

void Camera::PassUniforms(GLuint _program) const
{
    glUniformMatrix4fv(glGetUniformLocation(_program, "view"), 1, GL_FALSE, glm::value_ptr(GetViewMatrix()));
}