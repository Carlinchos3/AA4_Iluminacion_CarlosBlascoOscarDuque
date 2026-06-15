#include "Sky.h"
#include <cmath>
#include <gtc/matrix_transform.hpp>

Sky::Sky(Satelite* sun, Satelite* moon, float cycleDuration)
    : sun(sun), moon(moon), cycleDuration(cycleDuration) { }

void Sky::Update(float currentTime, GLuint program) 
{
    float angle = (currentTime / cycleDuration) * glm::two_pi<float>();

    sun->Update(angle);
    moon->Update(angle + glm::pi<float>());

    glm::vec3 ambientColor = CalcAmbientColor(angle);

    glUniform3fv(glGetUniformLocation(program, "sunDir"), 1, glm::value_ptr(sun->GetDirection()));
    glUniform3fv(glGetUniformLocation(program, "moonDir"), 1, glm::value_ptr(moon->GetDirection()));
    glUniform3fv(glGetUniformLocation(program, "ambientColor"), 1, glm::value_ptr(ambientColor));
}

glm::vec3 Sky::CalcAmbientColor(float angle) const 
{
    float dayFactor = glm::clamp(sin(angle), 0.0f, 1.0f);

    glm::vec3 nightColor = glm::vec3(0.05f, 0.05f, 0.2f);
    glm::vec3 dayColor = glm::vec3(1.0f, 0.95f, 0.7f);

    return glm::mix(nightColor, dayColor, dayFactor);
}