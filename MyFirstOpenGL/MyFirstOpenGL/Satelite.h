#pragma once
#include "GameObject.h"

class Satelite : public GameObject 
{
public:
    Satelite(Model* model, Texture* texture, float orbitRadius, glm::vec3 scale, bool isLightSource = true);
    void Update(float angle);
    glm::vec3 GetDirection() const;

private:
    float orbitRadius;
};