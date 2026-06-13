#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Model.h"
#include "Texture.h"

class GameObject {
public:
    GameObject(Model* model, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool isLightSource = false);
    void Render(GLuint program) const;

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);

    glm::vec3 GetPosition() const;

private:
    Model*    m_model;
    Texture*  m_texture;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    bool m_isLightSource;
};