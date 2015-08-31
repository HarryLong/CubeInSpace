#include "terrain_temperature.h"

TerrainTemperature::TerrainTemperature() :
    TextureElement2DArray<GLbyte>(12, QOpenGLTexture::TextureFormat::R8_SNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Int8)
{

}

TerrainTemperature::~TerrainTemperature()
{

}
