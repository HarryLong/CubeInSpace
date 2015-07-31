#include "terrain_temperature.h"

TerrainTemperature::TerrainTemperature()
{

}

TerrainTemperature::~TerrainTemperature()
{

}

void TerrainTemperature::bindDec()
{
    m_dec_temp.bind();
}

void TerrainTemperature::bindJun()
{
    m_jun_temp.bind();
}

void TerrainTemperature::setData(GLbyte * jun_data, GLbyte * dec_data, int width, int height)
{
    m_jun_temp.setData(jun_data, width, height);
    m_dec_temp.setData(dec_data, width, height);
}

void TerrainTemperature::getTempData(int x, int z, GLbyte & jun_temp, GLbyte & dec_temp)
{
    jun_temp = m_jun_temp(x,z);
    dec_temp = m_dec_temp(x,z);
}

TerrainTemperatureTexture::TerrainTemperatureTexture() :
    TextureElement<GLbyte>(QOpenGLTexture::TextureFormat::R8_SNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Int8)
{

}

TerrainTemperatureTexture::~TerrainTemperatureTexture()
{

}
