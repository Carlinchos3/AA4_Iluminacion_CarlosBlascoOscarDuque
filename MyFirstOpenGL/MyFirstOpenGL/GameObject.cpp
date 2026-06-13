#include "GameObject.h"

GameObject::GameObject(Model* model, Texture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool isLightSource)
    : m_model(model), m_texture(texture), m_position(position), m_rotation(rotation), m_scale(scale), m_isLightSource(isLightSource) 
{

}

void GameObject::Render(GLuint program) const 
{
    m_texture->Bind();

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

    glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
    glUniform1i(glGetUniformLocation(program, "isLightSource"), m_isLightSource);

    m_model->Render();
}

void GameObject::SetPosition(const glm::vec3& position) { m_position = position; }

void GameObject::SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }

void GameObject::SetScale(const glm::vec3& scale) { m_scale = scale; }

glm::vec3 GameObject::GetPosition() const { return m_position; }