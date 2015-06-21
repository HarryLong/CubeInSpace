#ifndef TERRAIN_TEMPERATURE_H
#define TERRAIN_TEMPERATURE_H

#include "texture_element.h"

/***********************
 * TERRAIN TEMPERATURE *
 ***********************/
class TerrainTemperature : public TextureElement<GLbyte>{
public:
    TerrainTemperature();
    ~TerrainTemperature();
};

#endif // TERRAIN_TEMPERATURE_H
