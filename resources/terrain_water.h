#ifndef TERRAIN_WATER_H
#define TERRAIN_WATER_H

#include "../gl_texture/texture_element.h"
#include "glm/common.hpp"

class TerrainWaterHeightmap : public TextureElement<GLuint> // TODO: And GL Drawable to actually draw the water bruuuu
{
public:
    TerrainWaterHeightmap();
    ~TerrainWaterHeightmap();
};

class TerrainWater{
public:
    struct MaterialProperties{
        static const glm::vec4 _DIFFUSE;
        static const glm::vec4 _SPECULAR;
        static const glm::vec4 _AMBIENT;
    };
    TerrainWater();
    ~TerrainWater();
    void bindDec();
    void bindJun();
    GLuint getDecTextureId();
    GLuint getJunTextureId();
    void pushToGPU();
    void setData(GLuint * jun_data, GLuint * dec_data, int width, int height);
    void invalidate();
    bool isValid();
    bool balanced();
    void setBalanced(bool balanced);
    void syncFromGPU();

private:
    TerrainWaterHeightmap * m_terrain_water_jun;
    TerrainWaterHeightmap * m_terrain_water_dec;

    bool m_balanced;
};

#endif // TERRAIN_WATER_H
