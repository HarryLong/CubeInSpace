#ifndef TERRAIN_DAILY_ILLUMINATION_H
#define TERRAIN_DAILY_ILLUMINATION_H

#include "../gl_texture/texture_element.h"

class TerrainDailyIlluminationTexture : public TextureElement<GLubyte>
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

    void bindMax();
    void bindMin();

    void setData(GLubyte * min_data, GLubyte * max_data, int width, int height);

    void getIlluminationData(int x, int z, GLubyte & min, GLubyte & max);

private:
    TerrainDailyIlluminationTexture m_min_illumination;
    TerrainDailyIlluminationTexture m_max_illumination;
};



#endif // TERRAIN_DAILY_ILLUMINATION_H
