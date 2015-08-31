#ifndef TERRAIN_DAILY_ILLUMINATION_H
#define TERRAIN_DAILY_ILLUMINATION_H

#include "../gl_texture/texture_element_2d_array.h"

class TerrainDailyIllumination : public TextureElement2DArray<GLubyte>
{
public:
    TerrainDailyIllumination();
    ~TerrainDailyIllumination();
};



#endif // TERRAIN_DAILY_ILLUMINATION_H
