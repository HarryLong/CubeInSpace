#ifndef TERRAIN_TEMPERATURE_H
#define TERRAIN_TEMPERATURE_H

#include "../gl_texture/texture_element_2d_array.h"

class TerrainTemperature : public TextureElement2DArray<GLbyte>
{
public:
    TerrainTemperature();
    ~TerrainTemperature();
};


#endif // TERRAIN_TEMPERATURE_H
