#include "lighting_manager.h"
#include "constants.h"
#include <iostream>
#include "glm/glm.hpp"

/********************
 * LIGHTING MANAGER *
 ********************/
LightingManager::LightingManager()
{
    setMonth(DEFAULT_MONTH_OF_YEAR);
    setTime(DEFAULT_HOUR_OF_DAY);
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

void LightingManager::setTime(int hour_of_day)
{
    m_sunlight_properties.setTime(hour_of_day);
}
