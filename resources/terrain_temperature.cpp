#include "terrain_temperature.h"

TerrainTemperature::TerrainTemperature() :
    TextureElement2DArray<GLbyte>(2, QOpenGLTexture::TextureFormat::R8_SNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Int8)
{

}

TerrainTemperature::~TerrainTemperature()
{

}

//TerrainTemperatureTexture & TerrainTemperature::getJun()
//{
//    return m_jun_temp;
//}

//TerrainTemperatureTexture & TerrainTemperature::getDec()
//{
//    return m_dec_temp;
//}

//void TerrainTemperature::setData(GLbyte * jun_data, GLbyte * dec_data, int width, int height)
//{
//    m_jun_temp.setData(jun_data);
//    m_dec_temp.setData(dec_data);
//}

//void TerrainTemperature::getTempData(int x, int z, GLbyte & jun_temp, GLbyte & dec_temp)
//{
//    jun_temp = m_jun_temp(x,z);
//    dec_temp = m_dec_temp(x,z);
//}

//TerrainTemperatureTexture::TerrainTemperatureTexture() :
//    TextureElement2D<GLbyte>(QOpenGLTexture::TextureFormat::R8_SNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Int8)
//{

//}

//TerrainTemperatureTexture::~TerrainTemperatureTexture()
//{

//}
