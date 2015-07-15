#include "light_properties.h"
#include <algorithm>
#include "../gl_core/glm_wrapper.h"
#include <iostream>
#include "constants.h"
#include "../widgets/orientation_widget.h"

/********************
 * LIGHT PROPERTIES *
 ********************/
LightProperties::LightProperties(glm::vec3 position, glm::vec4 diffuse_color, glm::vec4 specular_color, glm::vec4 ambient_color) :
        m_position(position), m_diffuse_color(diffuse_color), m_specular_color(specular_color), m_ambient_color(ambient_color)
{

}

LightProperties::~LightProperties()
{
}

void LightProperties::setPosition(glm::vec3 position)
{
    m_position = position;
}

glm::vec3 LightProperties::getPosition() const
{
    return m_position;
}

/***********************
 * SUNLIGHT PROPERTIES *
 ***********************/
#define SUNLIGHT_DIFFUSE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define SUNLIGHT_SPECULAR glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define SUNLIGHT_AMBIANT glm::vec4(0.4f, 0.4f, 0.4f, 1.0f)
#define SUNLIGHT_BASE_POSITION glm::vec3(500.0f, 500.0f, -500.0f)
const float SunLightProperties::_axis_tilt = AXIS_TILT;
const float SunLightProperties::_monthly_axis_tilt = (AXIS_TILT)/3.f;
const int SunLightProperties::_half_day_in_minutes = 720;
const int SunLightProperties::_quarter_day_in_minutes = SunLightProperties::_half_day_in_minutes/2.0f;
const int SunLightProperties::_3_quarters_day_in_minutes = SunLightProperties::_half_day_in_minutes + SunLightProperties::_quarter_day_in_minutes;

SunLightProperties::SunLightProperties() :
    LightProperties (SUNLIGHT_BASE_POSITION, SUNLIGHT_DIFFUSE, SUNLIGHT_SPECULAR, SUNLIGHT_AMBIANT),
    m_terrain_depth(0), m_terrain_width(0), m_month(1), m_time_of_day(0),
    m_latitude(0)
{
    setNorthOrientation(OrientationWidget::_BASE_NORTH_ORIENTATION[0],
                        OrientationWidget::_BASE_NORTH_ORIENTATION[1],
                        OrientationWidget::_BASE_NORTH_ORIENTATION[2]);
    refresh_position();
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

void SunLightProperties::setTime(int minutes)
{
    m_time_of_day = minutes;
    refresh_position();
}

int SunLightProperties::latitude() const
{
    return m_latitude;
}

int SunLightProperties::month() const
{
    return m_month;
}

int SunLightProperties::time() const
{
    return m_time_of_day;
}

void SunLightProperties::setTerrainDimensions(int width, int depth, int base_height, int max_height)
{
    m_terrain_width = width;
    m_terrain_depth = depth;

    m_center_x = m_terrain_width/2.0f;
    m_center_y = 0;
    m_center_z = m_terrain_depth/2.0f;

    refresh_position();
}

#include "geom.h"
#define LATITUDE 90
void SunLightProperties::refresh_position()
{
    // First calculate some orientations we need values we need
    glm::vec3 east_orientation = glm::rotateY(m_north_orientation, (float)M_PI_2);
    glm::vec3 true_north_orientation = glm::rotate(m_north_orientation, SunLightProperties::latitude_to_angle(m_latitude), east_orientation);

    int sun_trajectory_radius(500000);
    float max_axis_tilt(SunLightProperties::get_axis_tilt_angle(m_month));
    float day_angle(SunLightProperties::minutes_to_angle(m_time_of_day));
    glm::vec3 cp_tn_and_east (glm::normalize(glm::cross(true_north_orientation, east_orientation)));

    // First calculate the sun position at midday during the equinox
    glm::vec3 sun_position ( ((float)sun_trajectory_radius) * cp_tn_and_east );

    // Now take into consideration axis tilt based on the month
    sun_position = glm::rotate( sun_position, -max_axis_tilt, east_orientation ); // PITCH

    // Now rotate around true north for the day
    sun_position = glm::rotate(sun_position, day_angle, true_north_orientation);

    // Now align to the center of the terrain (i.e the center of the terrain is at the latitude specified)
    sun_position += glm::vec3(m_center_x, m_center_y, m_center_z);

    // Done!
    setPosition(sun_position);

    emit sunPositionChanged(sun_position[0], sun_position[1], sun_position[2]);
}

void SunLightProperties::setNorthOrientation(float north_x, float north_y, float north_z)
{
    m_north_orientation = glm::normalize(glm::vec3(north_x, north_y, north_z));

    refresh_position();
}

// Static methods
// Returns in radians, the rotation relative to noon
float SunLightProperties::minutes_to_angle(int minutes)
{
    return (((float) _half_day_in_minutes - minutes) / _half_day_in_minutes) * M_PI;
}

float SunLightProperties::get_axis_tilt_angle(int month)
{
    return -_axis_tilt + (std::abs(6 - month) * _monthly_axis_tilt);
}

float SunLightProperties::latitude_to_angle(int latitude)
{
    return Geom::toRadians(-latitude);
}

void SunLightProperties::split_tilt(int time_of_day, float & pitch, float & roll)
{
    float f_time_of_day ((float) time_of_day);
    // Pitch
    {
        if(time_of_day <= _half_day_in_minutes) // Before midday
            pitch = 1.0f - ((f_time_of_day/_half_day_in_minutes) * 2);
        else // After midday
            pitch = -1 + (((f_time_of_day-_half_day_in_minutes)/_half_day_in_minutes) * 2);
    }

    // Roll
    {
        if(time_of_day < (_quarter_day_in_minutes))
            roll = (f_time_of_day/_quarter_day_in_minutes) * 1.0f;
        else if(f_time_of_day >= _quarter_day_in_minutes && f_time_of_day <= _3_quarters_day_in_minutes)
            roll = 1 - (((f_time_of_day-_quarter_day_in_minutes)/_half_day_in_minutes)*2.0f);
        else // 6 pm -> midnight
            roll = -1 + ((f_time_of_day - _3_quarters_day_in_minutes) / _quarter_day_in_minutes);
    }
}
