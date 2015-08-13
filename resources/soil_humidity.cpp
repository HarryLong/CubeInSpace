#include "soil_humidity.h"
#include <cstring>

//SoilHumidityHeightmap::SoilHumidityHeightmap() :
//    TextureElement2D<GLuint>(QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
//{

//}

//SoilHumidityHeightmap::~SoilHumidityHeightmap()
//{

//}

//-----------------------------------------------------------------------------------------------

SoilHumidity::SoilHumidity() :
    TextureElement2DArray<GLuint>(12, QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

SoilHumidity::~SoilHumidity()
{

}

//SoilHumidityHeightmap& SoilHumidity::operator[](int month)
//{
//    return m_soil_humidity[month-1];
//}

//void SoilHumidity::reset(int width, int depth)
//{
//    for(SoilHumidityHeightmap & heightmap : m_soil_humidity)
//    {
//        heightmap.reset(width, depth);
//    }
//}
