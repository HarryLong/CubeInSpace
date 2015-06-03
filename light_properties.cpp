#include "light_properties.h"
#include <algorithm>
#include "glm_rotations.h"
#include <iostream>
#include "constants.h"

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
const float SunLightProperties::_axis_tilt = AXIS_TILT;
const float SunLightProperties::_monthly_axis_tilt = (AXIS_TILT)/3.f;
const int SunLightProperties::_half_day_in_minutes = 720;
const int SunLightProperties::_quarter_day_in_minutes = SunLightProperties::_half_day_in_minutes/2.0f;
const int SunLightProperties::_3_quarters_day_in_minutes = SunLightProperties::_half_day_in_minutes + SunLightProperties::_quarter_day_in_minutes;
SunLightProperties::SunLightProperties(glm::vec3 north_orientation, glm::vec3 true_north_orientation, glm::vec3 east_orientation) : LightProperties (SUNLIGHT_BASE_POSITION, SUNLIGHT_DIFFUSE, SUNLIGHT_SPECULAR, SUNLIGHT_AMBIANT),
    m_terrain_depth(0), m_terrain_width(0), m_month(DEFAULT_MONTH_OF_YEAR), m_time_of_day(DEFAULT_TIME_OF_DAY)
{
    setOrientation(north_orientation, true_north_orientation, east_orientation);
}

SunLightProperties::~SunLightProperties()
{

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

void SunLightProperties::setTerrainDimensions(int width, int depth)
{
    m_terrain_width = width;
    m_terrain_depth = depth;

    m_center_x = m_terrain_width/2.0f;
    m_center_y = 0;
    m_center_z = m_terrain_depth/2.0f;

    m_diagonal_length = std::sqrt(m_terrain_depth*m_terrain_depth + m_terrain_width * m_terrain_width);

    refresh_position();
}

#include "geom.h"
#define LATITUDE 90
void SunLightProperties::refresh_position()
{
    int sun_trajectory_radius(m_diagonal_length /*+ 50000*/);

    float max_axis_tilt(SunLightProperties::get_axis_tilt_angle(m_month));
    float day_angle(SunLightProperties::minutes_to_angle(m_time_of_day));
    glm::vec3 cp_tn_and_east (glm::normalize(glm::cross(m_true_north, m_east)));

    // First calculate the sun position at midday during the equinox
    std::cout << "True north: " << m_true_north[0] << ", " << m_true_north[1] << ", " << m_true_north[2] << std::endl;
    std::cout << "East: " << m_east[0] << ", " << m_east[1] << ", " << m_east[2] << std::endl;
    glm::vec3 sun_position ( ((float)sun_trajectory_radius) * cp_tn_and_east );

    // Now take into consideration axis tilt based on the month
    sun_position = glm::rotate( sun_position, -max_axis_tilt, m_east ); // PITCH

    // Now rotate around true north for the day
    sun_position = glm::rotate(sun_position, day_angle, m_true_north);

    // Now align to the center of the terrain (i.e the center of the terrain is at the latitude specified)
    sun_position += glm::vec3(m_center_x, m_center_y, m_center_z);

    emit sunPositionChanged(sun_position[0], sun_position[1], sun_position[2]);

    // Done!
    setPosition(glm::vec4(sun_position, 1));
}

#include <iostream>
void SunLightProperties::setOrientation(glm::vec3 north, glm::vec3 true_north, glm::vec3 east)
{
    m_north = glm::normalize(north);
    m_true_north = glm::normalize(true_north);
    m_east = east;

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

glm::vec2 SunLightProperties::discard_y(const glm::vec3 & vec)
{
    return glm::vec2(vec[0], vec[2]);
}

glm::vec3 SunLightProperties::append_y(const glm::vec2 & vec, float y)
{
    return glm::vec3(vec[0], y, vec[1]);
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
