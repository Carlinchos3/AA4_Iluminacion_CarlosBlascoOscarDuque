#include "RenderManager.h"

RenderManager::RenderManager(ShaderManager* shaderManager)
    : m_shaderManager(shaderManager) {
}

void RenderManager::AddObject(GameObject* gameObject) 
{
    m_gameObjects.push_back(gameObject);
}
void RenderManager::Render(const Camera& camera, Sky& sky, InputManager& inputManager, float currentTime) const 
{
    GLuint program = m_shaderManager->GetProgram();

    camera.PassUniforms(program);
    inputManager.PassUniforms(program);
    sky.Update(currentTime, program);

    for (const auto& gameObject : m_gameObjects)
        gameObject->Render(program);
}

GLuint RenderManager::GetProgram() const {
    return m_shaderManager->GetProgram();
}