#include "lighting_manager.h"
#include "constants.h"
#include <iostream>
#include "glm/glm.hpp"

/********************
 * LIGHTING MANAGER *
 ********************/
LightingManager::LightingManager() :
    m_signal_sun_position_change(true)
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

void LightingManager::setTerrainDimensions(int width, int depth)
{
    m_sunlight_properties.setTerrainDimensions(width, depth);
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
    if(m_signal_sun_position_change)
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
}

void LightingManager::signal_sun_position_change(bool enabled)
{
    m_signal_sun_position_change = enabled;
}

