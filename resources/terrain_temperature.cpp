#include "terrain_temperature.h"

TerrainTemperature::TerrainTemperature() :
    TextureElement2DArray<GLbyte>(12, QOpenGLTexture::TextureFormat::R8_SNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Int8),
    min_min_(1000), max_max_(1000)
{
    min_.resize(12);
    max_.resize(12);
}

TerrainTemperature::~TerrainTemperature()
{

}

void TerrainTemperature::setMin(int month, float min)
{
    if(min_min_ == 1000 || min < min_min_)
        min_min_ = min;
    min_.at(month-1) = min;
}

void TerrainTemperature::setMax(int month, float max)
{
    if(max_max_ == 1000 || max > max_max_)
        max_max_ = max;
    max_.at(month-1) = max;
}

float TerrainTemperature::getMin(int month) const
{
    return min_.at(month);
}

float TerrainTemperature::getMax(int month) const
{
    return max_.at(month);
}

float TerrainTemperature::getMin() const
{
    return min_min_;
}

float TerrainTemperature::getMax() const
{
    return max_max_;
}
