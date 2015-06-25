#include "lighting_manager.h"
#include "constants.h"
#include <iostream>
#include "glm/glm.hpp"

/********************
 * LIGHTING MANAGER *
 ********************/
LightingManager::LightingManager(TimeControllers & time_controllers, PositionControllers & position_controllers) :
    m_time_controllers(time_controllers), m_position_controllers(position_controllers),
    m_sunlight_properties(position_controllers.latitude_slider->value())
{
    establish_connections();
}

LightingManager::~LightingManager()
{

}

SunLightProperties & LightingManager::getSunlightProperties()
{
    return m_sunlight_properties;
}

void LightingManager::setTerrainDimensions(int width, int depth, int base_height, int max_height)
{
    m_sunlight_properties.setTerrainDimensions(width, depth, base_height, max_height);
}

void LightingManager::setMonth(int month)
{
    m_sunlight_properties.setMonth(month);
}

void LightingManager::setTime(int minutes)
{
    m_sunlight_properties.setTime(minutes);
}

void LightingManager::setLatitude(int latitude)
{
    m_sunlight_properties.setLatitude(latitude);
}

void LightingManager::setNorthOrientation(float north_x, float north_y, float north_z)
{
    m_sunlight_properties.setNorthOrientation(north_x, north_y, north_z);
}

void LightingManager::emit_sun_position_change(float pos_x, float pos_y, float pos_z)
{
    emit sunPositionChanged(pos_x, pos_y, pos_z);
}

int LightingManager::currentMonth() const
{
    return m_sunlight_properties.month();
}

int LightingManager::currentTime() const
{
    return m_sunlight_properties.time();
}

void LightingManager::establish_connections()
{
    connect(&m_sunlight_properties, SIGNAL(sunPositionChanged(float,float,float)), this, SLOT(emit_sun_position_change(float,float,float)));
    connect(m_time_controllers.month_slider, SIGNAL(valueChanged(int)), this, SLOT(setMonth(int)));
    connect(m_time_controllers.time_of_day_slider, SIGNAL(valueChanged(int)), this, SLOT(setTime(int)));
    connect(m_position_controllers.latitude_slider, SIGNAL(valueChanged(int)), this, SLOT(setLatitude(int)));
}
