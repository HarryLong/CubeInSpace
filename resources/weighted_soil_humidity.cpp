#include "weighted_soil_humidity.h"
#include <cstring>

WeightedSoilHumidity::WeightedSoilHumidity() :
    TextureElement2DArray<GLfloat>(12, QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

WeightedSoilHumidity::~WeightedSoilHumidity()
{

}
