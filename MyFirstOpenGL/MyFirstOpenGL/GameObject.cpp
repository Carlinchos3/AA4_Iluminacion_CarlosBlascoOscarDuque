#include "GameObject.h"

GameObject::GameObject(Model* _model, Texture* _texture, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, bool _isLightSource)
    : model(_model), texture(_texture), position(_position), rotation(_rotation), scale(_scale), isLightSource(_isLightSource)
{

}

void GameObject::Render(GLuint _program) const
{
    texture->Bind();

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    glUniformMatrix4fv(glGetUniformLocation(_program, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(_program, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
    glUniform1i(glGetUniformLocation(_program, "isLightSource"), isLightSource);

    model->Render();
}

void GameObject::SetPosition(const glm::vec3& _position) { position = _position; }

void GameObject::SetRotation(const glm::vec3& _rotation) { rotation = _rotation; }

void GameObject::SetScale(const glm::vec3& _scale) { scale = _scale; }

glm::vec3 GameObject::GetPosition() const { return position; }