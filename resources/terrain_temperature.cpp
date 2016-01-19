#include "terrain_temperature.h"

TerrainTemperature::TerrainTemperature() :
    TextureElement2DArray<GLbyte>(12, QOpenGLTexture::TextureFormat::R8_SNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Int8)
{
    min_.resize(12);
    max_.resize(12);
}

TerrainTemperature::~TerrainTemperature()
{

}

void TerrainTemperature::setMin(int month, float min)
{
    min_.at(month-1) = min;
}

void TerrainTemperature::setMax(int month, float max)
{
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
