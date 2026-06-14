#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Camera 
{
public:
    Camera(glm::vec3 _position, float _yaw, float _pitch);

    void MoveForward(float _deltaTime);
    void MoveRight(float _deltaTime);
    void Rotate(float _offsetX, float _offsetY);

    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetForward() const;

    void PassUniforms(GLuint _program) const;

    float GetFov() { return fFov; }
    float GetFar() { return fFar; }
    float GetNear() { return fNear; }

private:
    void UpdateVectors();

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    float yaw;
    float pitch;
    float speed = 5.0f;
    float sensitivity = 0.1f;
    float yawLimit = 89.0f;

    float fFov = 45.f;
    float fFar = 0.1f;
    float fNear = 100.f;
};

