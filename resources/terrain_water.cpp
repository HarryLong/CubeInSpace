#include "terrain_water.h"

const glm::vec4 TerrainWater::MaterialProperties::_DIFFUSE = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_SPECULAR = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_AMBIENT = glm::vec4(0.f, 0.f, 1.f, 1.0f);
TerrainWater::TerrainWater() :
    m_terrain_water_jun(new TerrainWaterHeightmap),
    m_terrain_water_dec(new TerrainWaterHeightmap),
    m_balanced(true)
{

}

TerrainWater::~TerrainWater()
{
    delete m_terrain_water_jun;
    delete m_terrain_water_dec;
}

bool TerrainWater::isValid()
{
    return m_terrain_water_jun->isValid() && m_terrain_water_dec->isValid();
}

void TerrainWater::pushToGPU()
{
    m_terrain_water_jun->pushToGPU();
    m_terrain_water_dec->pushToGPU();
}

void TerrainWater::invalidate()
{
    m_terrain_water_jun->invalidate();
    m_terrain_water_dec->invalidate();

    m_balanced = true; // No point trying to balance
}

void TerrainWater::setData(GLuint * jun_data, GLuint * dec_data, int width, int height)
{
    m_terrain_water_jun->setData(jun_data, width, height);
    m_terrain_water_dec->setData(dec_data, width, height);
    m_balanced = false;
}

void TerrainWater::bindDec()
{
    m_terrain_water_dec->bind();
}

void TerrainWater::bindJun()
{
    m_terrain_water_jun->bind();
}

GLuint TerrainWater::getDecTextureId()
{
    return m_terrain_water_dec->textureId();
}

GLuint TerrainWater::getJunTextureId()
{
    return m_terrain_water_jun->textureId();
}

void TerrainWater::syncFromGPU()
{
    m_terrain_water_jun->syncFromGPU();
    m_terrain_water_dec->syncFromGPU();
}

bool TerrainWater::balanced()
{
    return m_balanced;
}

void TerrainWater::setBalanced(bool balanced)
{
    m_balanced = balanced;
}

TerrainWaterHeightmap::TerrainWaterHeightmap() :
    TextureElement<GLuint>(QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

TerrainWaterHeightmap::~TerrainWaterHeightmap()
{

}
