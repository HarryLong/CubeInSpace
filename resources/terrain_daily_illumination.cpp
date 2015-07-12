#include "terrain_daily_illumination.h"

TerrainDailyIllumination::TerrainDailyIllumination() :
    m_min_illumination(new TerrainDailyIlluminationTexture),
    m_max_illumination(new TerrainDailyIlluminationTexture)
{

}

TerrainDailyIllumination::~TerrainDailyIllumination()
{

}

void TerrainDailyIllumination::bindMin()
{
    m_min_illumination->bind();
}

void TerrainDailyIllumination::bindMax()
{
    m_max_illumination->bind();
}

void TerrainDailyIllumination::setData(GLubyte * min_data, GLubyte * max_data, int width, int height)
{
    m_min_illumination->setData(min_data, width, height);
    m_max_illumination->setData(max_data, width, height);
}

void TerrainDailyIllumination::invalidate()
{
    m_min_illumination->invalidate();
    m_max_illumination->invalidate();
}

bool TerrainDailyIllumination::isValid()
{
    return m_min_illumination->isValid() && m_max_illumination->isValid();
}

void TerrainDailyIllumination::getIlluminationData(int x, int z, GLubyte & min, GLubyte & max)
{
    min = (*m_min_illumination)(x,z);
    max = (*m_max_illumination)(x,z);
}

void TerrainDailyIllumination::pushToGPU()
{
    m_min_illumination->pushToGPU();
    m_max_illumination->pushToGPU();
}

TerrainDailyIlluminationTexture::TerrainDailyIlluminationTexture() :
    TextureElement<GLubyte>(QOpenGLTexture::TextureFormat::RG8_UNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8)
{

}

TerrainDailyIlluminationTexture::~TerrainDailyIlluminationTexture()
{

}
