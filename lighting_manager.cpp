#include "lighting_manager.h"
#include "constants.h"
#include <iostream>
#include "glm/glm.hpp"

/********************
 * LIGHTING MANAGER *
 ********************/
LightingManager::LightingManager(glm::vec2 north_orientation) : m_sunlight_properties(north_orientation)
{
    setMonth(DEFAULT_MONTH_OF_YEAR);
    setTime(DEFAULT_TIME_OF_DAY);
}

LightingManager::~LightingManager()
{

}

const SunLightProperties & LightingManager::getSunlightProperties()
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

void LightingManager::setNorth(glm::vec2 orientation)
{
    m_sunlight_properties.setNorth(orientation);
}
