#ifndef LIGHT_PROPERTIES_H
#define LIGHT_PROPERTIES_H

#include "glm/glm.hpp"
#include <QObject>

class LightProperties : public QObject{
Q_OBJECT
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
Q_OBJECT
public:
    SunLightProperties(glm::vec3 north_orientation, glm::vec3 true_north_orientation, glm::vec3 east_orientation);
    ~SunLightProperties();

    int currentMonth() const;
    int currentTime() const;

    static float minutes_to_angle(int minutes);
    static float get_axis_tilt_angle(int month);

public slots:
    void setOrientation(glm::vec3 north, glm::vec3 true_north, glm::vec3 east);
    void setMonth(int month);
    void setTime(int minutes);
    void setTerrainDimensions(int width, int depth, int base_height, int max_height);

signals:
    void sunPositionChanged(float pos_x, float pos_y, float pos_z);

private:
    void refresh_position();

    static glm::vec3 append_y(const glm::vec2 & vec, float y = .0f);
    static glm::vec2 discard_y(const glm::vec3 & vec);
    static void split_tilt(int time_of_day, float & pitch, float & roll);

    int m_month;
    int m_time_of_day;

    int m_terrain_width, m_terrain_depth;
    float m_center_x, m_center_y, m_center_z;

    float m_true_north_rotation;

    glm::vec3 m_north;
    glm::vec3 m_true_north;
    glm::vec3 m_east;
    static const float _axis_tilt;
    static const float _monthly_axis_tilt;
    static const int _half_day_in_minutes;
    static const int _quarter_day_in_minutes;
    static const int _3_quarters_day_in_minutes;
};

#endif //LIGHT_PROPERTIES_H
