#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "Satelite.h"

class Sky {
public:
    Sky(Satelite* sun, Satelite* moon, float cycleDuration);
    void Update(float currentTime, GLuint program);

private:
    Satelite* sun;
    Satelite* moon;
    float cycleDuration;

    glm::vec3 CalcAmbientColor(float angle) const;
};