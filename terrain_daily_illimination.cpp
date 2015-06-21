#include "terrain_daily_illimination.h"

TerrainDailyIllumination::TerrainDailyIllumination() :
    TextureElement<GLubyte>(QOpenGLTexture::TextureFormat::RG8_UNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8)
{

}

TerrainDailyIllumination::~TerrainDailyIllumination()
{

}
