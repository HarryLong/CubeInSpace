#include "lighting_manager.h"
#include "constants.h"
#include <iostream>
#include "glm/glm.hpp"

/********************
 * LIGHTING MANAGER *
 ********************/
LightingManager::LightingManager(glm::vec3 north_orientation, glm::vec3 true_north_orientation, glm::vec3 east_orientation) :
    m_sunlight_properties(north_orientation, true_north_orientation, east_orientation)
{
    connect(&m_sunlight_properties, SIGNAL(sunPositionChanged(float,float,float)), this, SLOT(emit_sun_position_change(float,float,float)));
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

void LightingManager::setOrientation(float north_x, float north_y, float north_z,
                    float true_north_x, float true_north_y, float true_north_z,
                    float east_x, float east_y, float east_z)
{
    m_sunlight_properties.setOrientation(glm::vec3(north_x, north_y, north_z),
                                         glm::vec3(true_north_x, true_north_y, true_north_z),
                                         glm::vec3(east_x, east_y, east_z));
}

void LightingManager::emit_sun_position_change(float pos_x, float pos_y, float pos_z)
{
    emit sunPositionChanged(pos_x, pos_y, pos_z);
}
