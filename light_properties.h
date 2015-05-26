#ifndef LIGHT_PROPERTIES_H
#define LIGHT_PROPERTIES_H

#include "glm/glm.hpp"

class LightProperties{
public:
    LightProperties(glm::vec4 position, glm::vec4 diffuse_color, glm::vec4 specular_color, glm::vec4 ambient_color);
    ~LightProperties();

    glm::vec4 getPosition() const;
    void setPosition(glm::vec4 position);

    const glm::vec4 m_diffuse_color;
    const glm::vec4 m_specular_color;
    const glm::vec4 m_ambient_color;

private:
    glm::vec4 m_position;
};

class SunLightProperties : public LightProperties {
public:
    SunLightProperties();
    ~SunLightProperties();

    void setLatitude(int latitude);
    void setMonth(int month);
    void setTime(int hour_of_day);
    void setTerrainDimensions(int width, int depth);

private:
    void refresh_position();
    float hour_to_angle(int hour);

    int m_latitude;
    int m_month;
    int m_hour_of_day;

    int m_terrain_width;
    int m_terrain_depth;
};

#endif //LIGHT_PROPERTIES_H
