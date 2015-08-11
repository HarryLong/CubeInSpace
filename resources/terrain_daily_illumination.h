#ifndef TERRAIN_DAILY_ILLUMINATION_H
#define TERRAIN_DAILY_ILLUMINATION_H

#include "../gl_texture/texture_element_2d.h"

class TerrainDailyIlluminationTexture : public TextureElement2D<GLubyte>
{
public:
    TerrainDailyIlluminationTexture();
    ~TerrainDailyIlluminationTexture();
};

class TerrainDailyIllumination
{
public:
    TerrainDailyIllumination();
    ~TerrainDailyIllumination();

    TerrainDailyIlluminationTexture & getMin();
    TerrainDailyIlluminationTexture & getMax();

//    void setData(GLubyte * min_data, GLubyte * max_data, int width, int height);

//    void getIlluminationData(int x, int z, GLubyte & min, GLubyte & max);

private:
    TerrainDailyIlluminationTexture m_min_illumination;
    TerrainDailyIlluminationTexture m_max_illumination;
};



#endif // TERRAIN_DAILY_ILLUMINATION_H
