#include "Sky.h"
#include <cmath>
#include <gtc/matrix_transform.hpp>

Sky::Sky(Satelite* sun, Satelite* moon, float cycleDuration)
    : m_sun(sun), m_moon(moon), m_cycleDuration(cycleDuration) { }

void Sky::Update(float currentTime, GLuint program) 
{
    float angle = (currentTime / m_cycleDuration) * 2.0f * glm::pi<float>();

    m_sun->Update(angle);
    m_moon->Update(angle + glm::pi<float>());

    glm::vec3 ambientColor = CalcAmbientColor(angle);

    glUniform3fv(glGetUniformLocation(program, "sunDir"), 1, glm::value_ptr(m_sun->GetDirection()));
    glUniform3fv(glGetUniformLocation(program, "moonDir"), 1, glm::value_ptr(m_moon->GetDirection()));
    glUniform3fv(glGetUniformLocation(program, "ambientColor"), 1, glm::value_ptr(ambientColor));
}

glm::vec3 Sky::CalcAmbientColor(float angle) const 
{
    float dayFactor = glm::clamp(sin(angle), 0.0f, 1.0f);

    glm::vec3 nightColor = glm::vec3(0.05f, 0.05f, 0.2f);
    glm::vec3 dayColor = glm::vec3(1.0f, 0.95f, 0.7f);

    return glm::mix(nightColor, dayColor, dayFactor);
}