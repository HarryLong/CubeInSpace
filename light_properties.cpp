#include "light_properties.h"
#include <algorithm>
#include "glm_rotations.h"
#include <iostream>

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
const int SunLightProperties::_midday_in_minutes = 720;
SunLightProperties::SunLightProperties(glm::vec2 north_orientation) : LightProperties (SUNLIGHT_BASE_POSITION, SUNLIGHT_DIFFUSE, SUNLIGHT_SPECULAR, SUNLIGHT_AMBIANT),
    m_terrain_depth(0), m_terrain_width(0), m_time_of_day(0)
{
    setNorth(north_orientation);
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
//    for(int latitude(0); latitude <= 90; latitude+=10)
//    {
//        std::cout << "*********** LATITUDE: " << latitude << "*************" << std::endl;
//        for(int month(1); month < 13; month++)
//        {
//            std::cout << "Month: " << month << " | Sun angle: " << SunLightProperties::latitude_to_sun_angle(latitude, month) << std::endl;
//        }
//    }
//    std::cout << "Latitude: " << 0 << std::endl;
    int sun_trajectory_radius(m_diagonal_length/2.0d + 50);

    // First calculate true north based on the inclinaison of the earth
    float axis_tilt(SunLightProperties::get_axis_tilt_angle(m_month));
    float day_angle(SunLightProperties::minutes_to_angle(m_time_of_day));

    // First calculate the sun position as if it was midday on the equinox (i.e no earth tilt) at the equator
    glm::vec3 sun_position ( glm::normalize(glm::cross(append_y(m_north), SunLightProperties::append_y(m_east))) );
    sun_position *= sun_trajectory_radius;

    // Now rotate the sun based on our latitude
    float sun_angle(latitude_to_sun_angle(m_latitude, m_month));
    float sun_rotation_angle(M_PI_2 - sun_angle);
    sun_position = (glm::rotate(sun_position, sun_rotation_angle, SunLightProperties::append_y(m_east)));

    // True north takes into consideration axis tilt
    glm::vec3 true_north(glm::rotate(SunLightProperties::append_y(m_north), -axis_tilt, SunLightProperties::append_y(m_east)));

    // Then rotate aroung the true north for the day
    sun_position = (glm::rotate(sun_position, day_angle, true_north));

    // Now align to the center of the terrain (i.e the center of the terrain is at the latitude specified)
    sun_position += glm::vec3(m_center_x, m_center_y, m_center_z);

    // Done!
    setPosition(glm::vec4(sun_position, 1));
}

//void SunLightProperties::refresh_position()
//{
////    int sun_trajectory_radius(m_diagonal_length/2.0d + 50);
//    int sun_trajectory_radius(1000);

//    // position of the sun @ noon on the equator @ Equinox
//    glm::vec3 sun_position ( glm::normalize(glm::cross(append_y(m_north), SunLightProperties::append_y(m_east))) );
//    sun_position *= sun_trajectory_radius;

//    // Build rotation matrix
//    float day_angle(SunLightProperties::minutes_to_angle(m_time_of_day));
//    float year_angle(SunLightProperties::get_axis_tilt_angle(m_month));

//    std::cout << "Year rotation angle: " << Geom::toDegrees(year_angle) << std::endl;
//    std::cout << "Day rotation angle: " << Geom::toDegrees(day_angle) << std::endl;

//    // First rotate around the EAST-WEST axis for the season
//    sun_position = (glm::rotate(sun_position, year_angle, SunLightProperties::append_y(m_east)));

//    // Then rotate aroung the north axis for the day
//    sun_position = (glm::rotate(sun_position, day_angle, SunLightProperties::append_y(m_north)));

//    sun_position += glm::vec3(m_center_x, m_center_y, m_center_z);

//    setPosition(glm::vec4(sun_position, 1));
//}

//void SunLightProperties::refresh_position()
//{
//    int sun_trajectory_radius(m_diagonal_length/2.0d + 50);

//    // position of the sun @ noon on the equator @ Equinox
//    glm::vec3 up ( glm::cross(m_north, m_east) );
//    up *= sun_trajectory_radius;

//    // Build rotation matrix
//    float day_angle(SunLightProperties::minutes_to_angle(m_time_of_day));
//    float year_angle(SunLightProperties::get_axis_tilt_angle(m_month));

//    // First rotate around the EAST-WEST axis for the seasons


//    // First rotate around
//    glm::vec3 sun_position(glm::rotate(SunLightProperties::append_y(m_east), day_angle,
//                                       SunLightProperties::append_y(m_north)));

//    sun_position *= sun_trajectory_radius;
//    sun_position += glm::vec3(m_center_x, m_center_y, m_center_z);

//    setPosition(glm::vec4(sun_position, 1));
//}

#include <iostream>
void SunLightProperties::setNorth(glm::vec2 orientation)
{
    m_north = glm::normalize(orientation);

    // Calculate east
    glm::vec3 north(SunLightProperties::append_y(m_north));
    glm::vec3 east = glm::rotateY(north, (float) M_PI_2);
    east = glm::normalize(east);

    m_east = SunLightProperties::discard_y(east);

    refresh_position();
}

// Static methods
// Returns in radians, the rotation relative to noon
float SunLightProperties::minutes_to_angle(int minutes)
{
    return (((float) _midday_in_minutes - minutes) / _midday_in_minutes) * M_PI;
}

float SunLightProperties::get_axis_tilt_angle(int month)
{
    // June = AXIS_TILT
    // Dec = -AXIS_TILT
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

float SunLightProperties::latitude_to_sun_angle(float latitude, int month)
{
    return Geom::toRadians(90 + latitude + Geom::toDegrees(get_axis_tilt_angle(month)));
}
