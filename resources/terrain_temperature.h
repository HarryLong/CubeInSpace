#ifndef TERRAIN_TEMPERATURE_H
#define TERRAIN_TEMPERATURE_H

#include "../gl_texture/texture_element_2d_array.h"

class TerrainTemperature : public TextureElement2DArray<GLbyte>
{
public:
    static const int _JUN_LAYER_IDX = 0;
    static const int _DEC_LAYER_IDX = 1;

    TerrainTemperature();
    ~TerrainTemperature();
};


#endif // TERRAIN_TEMPERATURE_H
