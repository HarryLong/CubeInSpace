#ifndef LIGHT_PROPERTIES_H
#define LIGHT_PROPERTIES_H

#include "glm/glm.hpp"

class LightProperties{
public:
    LightProperties(glm::vec4 position, glm::vec4 diffuse_color, glm::vec4 specular_color, glm::vec4 ambient_color);
    ~LightProperties();

    glm::vec4 getPosition() const;

    const glm::vec4 m_diffuse_color;
    const glm::vec4 m_specular_color;
    const glm::vec4 m_ambient_color;

private:
    glm::vec4 m_position;
};

#define SUNLIGHT_DIFFUSE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define SUNLIGHT_SPECULAR glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define SUNLIGHT_AMBIANT glm::vec4(0.4f, 0.4f, 0.4f, 1.0f)
#define SUNLIGHT_BASE_POSITION glm::vec4(500.0f, 500.0f, -500.0f, 1.0f)
class SunLightProperties : public LightProperties {
public:
    SunLightProperties() : LightProperties (SUNLIGHT_BASE_POSITION, SUNLIGHT_DIFFUSE, SUNLIGHT_SPECULAR, SUNLIGHT_AMBIANT) {}
};

#endif //LIGHT_PROPERTIES_H
