#include "weighted_soil_humidity.h"
#include <cstring>

WeightedSoilHumidityHeightmap::WeightedSoilHumidityHeightmap() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

WeightedSoilHumidityHeightmap::~WeightedSoilHumidityHeightmap()
{

}

//-----------------------------------------------------------------------------------------------

WeightedSoilHumidity::WeightedSoilHumidity()
{

}

WeightedSoilHumidity::~WeightedSoilHumidity()
{

}

WeightedSoilHumidityHeightmap& WeightedSoilHumidity::operator[](int month)
{
    return m_weighted_soil_humidity[month-1];
}

void WeightedSoilHumidity::reset(int width, int depth)
{
    for(WeightedSoilHumidityHeightmap & heightmap : m_weighted_soil_humidity)
    {
        heightmap.reset(width, depth);
    }
}
