#include "terrain_daily_illumination.h"

TerrainDailyIllumination::TerrainDailyIllumination() :
    TextureElement2DArray<GLubyte>(12, QOpenGLTexture::TextureFormat::R8_UNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8),
    min_min_(-1), max_max_(-1)
{
    min_.resize(12);
    max_.resize(12);
}

TerrainDailyIllumination::~TerrainDailyIllumination()
{

}

void TerrainDailyIllumination::setMin(int month, float min)
{
    if(min_min_ == -1 || min < min_min_)
        min_min_ = min;
    min_.at(month-1) = min;
}

void TerrainDailyIllumination::setMax(int month, float max)
{
    if(max_max_ == -1 || max > max_max_)
        max_max_ = max;
    max_.at(month-1) = max;
}

float TerrainDailyIllumination::getMin(int month) const
{
    return min_.at(month);
}

float TerrainDailyIllumination::getMax(int month) const
{
    return max_.at(month);
}

float TerrainDailyIllumination::getMin() const
{
    return min_min_;
}

float TerrainDailyIllumination::getMax() const
{
    return max_max_;
}
