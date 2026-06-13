#include "RenderManager.h"

RenderManager::RenderManager(ShaderManager* shaderManager)
    : shaderManager(shaderManager) {
}

void RenderManager::AddObject(GameObject* gameObject) 
{
    gameObjects.push_back(gameObject);
}
void RenderManager::Render(const Camera& camera, Sky& sky, InputManager& inputManager, float currentTime) const 
{
    GLuint program = shaderManager->GetProgram();

    camera.PassUniforms(program);
    inputManager.PassUniforms(program);
    sky.Update(currentTime, program);

    for (const auto& gameObject : gameObjects)
        gameObject->Render(program);
}

GLuint RenderManager::GetProgram() const {
    return shaderManager->GetProgram();
}