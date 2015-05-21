#include "lighting_manager.h"
LightingManager::LightingManager()
{

}

LightingManager::~LightingManager()
{

}

const LightProperties & LightingManager::getSunlightProperties()
{
    return m_sunlight_properties;
}
