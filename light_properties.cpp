#include "light_properties.h"
#include <algorithm>

/********************
 * LIGHT PROPERTIES *
 ********************/
LightProperties::LightProperties(glm::vec4 position, glm::vec4 diffuse_color, glm::vec4 specular_color, glm::vec4 ambient_color) :
        m_position(position), m_diffuse_color(diffuse_color), m_specular_color(specular_color), m_ambient_color(ambient_color)
{

}

LightProperties::~LightProperties()
{
}

void LightProperties::setPosition(glm::vec4 position)
{
    m_position = position;
}

glm::vec4 LightProperties::getPosition() const
{
    return m_position;
}

/***********************
 * SUNLIGHT PROPERTIES *
 ***********************/
#define SUNLIGHT_DIFFUSE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define SUNLIGHT_SPECULAR glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define SUNLIGHT_AMBIANT glm::vec4(0.4f, 0.4f, 0.4f, 1.0f)
#define SUNLIGHT_BASE_POSITION glm::vec4(500.0f, 500.0f, -500.0f, 1.0f)
SunLightProperties::SunLightProperties() : LightProperties (SUNLIGHT_BASE_POSITION, SUNLIGHT_DIFFUSE, SUNLIGHT_SPECULAR, SUNLIGHT_AMBIANT),
    m_terrain_depth(0), m_terrain_width(0), m_hour_of_day(0)
{

}

SunLightProperties::~SunLightProperties()
{

}

void SunLightProperties::setLatitude(int latitude)
{
    m_latitude = latitude;
    refresh_position();
}

void SunLightProperties::setMonth(int month)
{
    m_month = month;
    refresh_position();
}

void SunLightProperties::setTime(int hour_of_day)
{
    m_hour_of_day = hour_of_day;
    refresh_position();
}

void SunLightProperties::setTerrainDimensions(int width, int depth)
{
    m_terrain_width = width;
    m_terrain_depth = depth;
    refresh_position();
}

void SunLightProperties::refresh_position()
{
    // Calculate the center of the terrain (and the circle
    int center_x(m_terrain_width/2.0f);
    int center_z(m_terrain_depth/2.0f);

    int radius(std::max(m_terrain_depth, m_terrain_width));

    glm::vec4 pos;

    float angle(hour_to_angle(m_hour_of_day));
    // X
    pos[0] = center_x + (-std::sin(angle) * radius);
    // Y
    pos[1] = -std::cos(angle) * radius;
    // Z
    pos[2] = center_z;
    // W
    pos[3] = 1;

    setPosition(pos);
}

float SunLightProperties::hour_to_angle(int hour)
{
    return (hour/24.0f) * 2 * M_PI;
}
