#include "soil_humidity.h"
#include <cstring>

SoilHumidityHeightmap::SoilHumidityHeightmap() :
    TextureElement<GLuint>(QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

SoilHumidityHeightmap::~SoilHumidityHeightmap()
{

}

//-----------------------------------------------------------------------------------------------

SoilHumidity::SoilHumidity()
{

}

SoilHumidity::~SoilHumidity()
{

}

SoilHumidityHeightmap& SoilHumidity::operator[](int month)
{
    return m_soil_humidity[month-1];
}

void SoilHumidity::reset(int width, int depth)
{
    int sz(sizeof(GLuint) * width * depth);

    for(SoilHumidityHeightmap & heightmap : m_soil_humidity)
    {
        GLuint * data = (GLuint *) std::malloc(sz);
        std::memset(data, 0, sz);
        heightmap.setData(data, width, depth);
    }
    pushToGPU();
}

void SoilHumidity::pushToGPU()
{
    for(SoilHumidityHeightmap & heightmap : m_soil_humidity)
    {
        heightmap.pushToGPU();
    }
}

void SoilHumidity::syncFromGPU()
{
    for(SoilHumidityHeightmap & heightmap : m_soil_humidity)
    {
        heightmap.syncFromGPU();
    }
}
