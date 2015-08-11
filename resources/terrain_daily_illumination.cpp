#include "terrain_daily_illumination.h"

TerrainDailyIllumination::TerrainDailyIllumination()
{

}

TerrainDailyIllumination::~TerrainDailyIllumination()
{

}

TerrainDailyIlluminationTexture & TerrainDailyIllumination::getMin()
{
    return m_min_illumination;
}

TerrainDailyIlluminationTexture & TerrainDailyIllumination::getMax()
{
    return m_max_illumination;
}

//void TerrainDailyIllumination::setData(GLubyte * min_data, GLubyte * max_data, int width, int height)
//{
//    m_min_illumination.setData(min_data, width, height);
//    m_max_illumination.setData(max_data, width, height);
//}

//void TerrainDailyIllumination::getIlluminationData(int x, int z, GLubyte & min, GLubyte & max)
//{
//    min = m_min_illumination(x,z);
//    max = m_max_illumination(x,z);
//}

TerrainDailyIlluminationTexture::TerrainDailyIlluminationTexture() :
    TextureElement2D<GLubyte>(QOpenGLTexture::TextureFormat::R8_UNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8)
{

}

TerrainDailyIlluminationTexture::~TerrainDailyIlluminationTexture()
{

}
