#ifndef LIGHT_PROPERTIES_H
#define LIGHT_PROPERTIES_H

#include "glm/glm.hpp"
#include <QObject>

class LightProperties : public QObject{
Q_OBJECT
public:
    LightProperties(glm::vec3 position, glm::vec4 diffuse_color, glm::vec4 specular_color, glm::vec4 ambient_color);
    ~LightProperties();

    glm::vec3 getPosition() const;
    void setPosition(glm::vec3 position);

    const glm::vec4 m_diffuse_color;
    const glm::vec4 m_specular_color;
    const glm::vec4 m_ambient_color;

private:
    glm::vec3 m_position;
};

#define AXIS_TILT 0.408407f
class SunLightProperties : public LightProperties {
Q_OBJECT
public:
    SunLightProperties();
    ~SunLightProperties();

    int month() const;
    int time() const;
    int latitude() const;

    static float minutes_to_angle(int minutes);
    static float get_axis_tilt_angle(int month);
    static float latitude_to_angle(int latitude);

public slots:
    void setNorthOrientation(float north_x, float north_y, float north_z);
    void setMonth(int month);
    void setTime(int minutes);
    void setLatitude(int latitude);
    void setTerrainDimensions(int width, int depth);

signals:
    void sunPositionChanged(float pos_x, float pos_y, float pos_z);

private:
    void refresh_position();

    static void split_tilt(int time_of_day, float & pitch, float & roll);

    int m_month;
    int m_time_of_day;
    int m_latitude;

    int m_terrain_width, m_terrain_depth;
    float m_center_x, m_center_y, m_center_z;

    glm::vec3 m_north_orientation;

    static const float _axis_tilt;
    static const float _monthly_axis_tilt;
    static const int _half_day_in_minutes;
    static const int _quarter_day_in_minutes;
    static const int _3_quarters_day_in_minutes;
};

#endif //LIGHT_PROPERTIES_H
