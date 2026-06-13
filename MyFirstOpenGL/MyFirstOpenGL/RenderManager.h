#pragma once
#include <GL/glew.h>
#include <vector>
#include "GameObject.h"
#include "Camera.h"
#include "Sky.h"
#include "ShaderManager.h"
#include "InputManager.h"

class RenderManager 
{
public:
    RenderManager(ShaderManager* shaderManager);
    void AddObject(GameObject* gameObject);
    void Render(const Camera& camera, Sky& sky, InputManager& inputManager, float currentTime) const;
    GLuint GetProgram() const;

private:
    ShaderManager* shaderManager;
    std::vector<GameObject*> gameObjects;
};