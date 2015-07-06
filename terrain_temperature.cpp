#include "terrain_temperature.h"


#include "terrain_shade.h"

TerrainTemperature::TerrainTemperature() :
    m_jun_temp(new TerrainTemperatureTexture),
    m_dec_temp(new TerrainTemperatureTexture)
{

}

TerrainTemperature::~TerrainTemperature()
{
    delete m_jun_temp;
    delete m_dec_temp;
}

TerrainTemperatureTexture * TerrainTemperature::getJunTemperature()
{
    return m_jun_temp;
}

TerrainTemperatureTexture * TerrainTemperature::getDecTemperature()
{
    return m_dec_temp;
}

void TerrainTemperature::invalidate()
{
    m_jun_temp->invalidate();
    m_dec_temp->invalidate();
}

bool TerrainTemperature::isValid()
{
    return m_jun_temp->isValid() && m_dec_temp->isValid();
}

TerrainTemperatureTexture::TerrainTemperatureTexture() :
    TextureElement<GLbyte>(QOpenGLTexture::TextureFormat::RG8_SNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Int8)
{

}

TerrainTemperatureTexture::~TerrainTemperatureTexture()
{

}
