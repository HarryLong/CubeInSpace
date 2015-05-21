#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "light_properties.h"

class LightingManager {
public:
    LightingManager();
    ~LightingManager();

    const LightProperties & getSunlightProperties();

private:
    SunLightProperties m_sunlight_properties;
};

#endif // LIGHT_MANAGER_H
