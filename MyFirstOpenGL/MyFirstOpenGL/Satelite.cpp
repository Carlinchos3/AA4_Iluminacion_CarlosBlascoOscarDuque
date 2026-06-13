#include "Satelite.h"
#include <cmath>

Satelite::Satelite(Model* model, Texture* texture, float orbitRadius, glm::vec3 scale, bool isLightSource)
    : GameObject(model, texture, glm::vec3(0.0f), glm::vec3(0.0f), scale, isLightSource), orbitRadius(orbitRadius) 
{

}

void Satelite::Update(float angle) 
{
    glm::vec3 position = glm::vec3(cos(angle) * orbitRadius, sin(angle) * orbitRadius, 0.0f);
    SetPosition(position);
}

glm::vec3 Satelite::GetDirection() const 
{
    return glm::normalize(GetPosition());
}