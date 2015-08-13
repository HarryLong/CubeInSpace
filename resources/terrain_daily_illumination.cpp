#include "terrain_daily_illumination.h"

TerrainDailyIllumination::TerrainDailyIllumination() :
    TextureElement2DArray<GLubyte>(2, QOpenGLTexture::TextureFormat::R8_UNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8)
{

}

TerrainDailyIllumination::~TerrainDailyIllumination()
{

}
