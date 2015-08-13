#ifndef TERRAIN_DAILY_ILLUMINATION_H
#define TERRAIN_DAILY_ILLUMINATION_H

#include "../gl_texture/texture_element_2d_array.h"

class TerrainDailyIllumination : public TextureElement2DArray<GLubyte>
{
public:
    static const int _MIN_LAYER_IDX = 0;
    static const int _MAX_LAYER_IDX = 1;

    TerrainDailyIllumination();
    ~TerrainDailyIllumination();
};



#endif // TERRAIN_DAILY_ILLUMINATION_H
