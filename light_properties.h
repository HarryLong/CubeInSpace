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

#define AXIS_TILT 0.408407f
class SunLightProperties : public LightProperties {
public:
    SunLightProperties(glm::vec2 north_orientation);
    ~SunLightProperties();

    void setLatitude(int latitude);
    void setMonth(int month);
    void setTime(int minutes);
    void setTerrainDimensions(int width, int depth);
    void setNorth(glm::vec2 orientation);

    static float minutes_to_angle(int minutes);
    static float get_axis_tilt_angle(int month);

private:
    void refresh_position();

    static glm::vec3 append_y(const glm::vec2 & vec, float y = .0f);
    static glm::vec2 discard_y(const glm::vec3 & vec);
    static float latitude_to_sun_angle(float latitude, int month);

    int m_latitude;
    int m_month;
    int m_time_of_day;

    int m_terrain_width, m_terrain_depth;
    float m_center_x, m_center_y, m_center_z;
    float m_diagonal_length;

    glm::vec2 m_north;
    glm::vec2 m_east;
    static const float _axis_tilt;
    static const float _monthly_axis_tilt;
    static const int _midday_in_minutes;
};

#endif //LIGHT_PROPERTIES_H
