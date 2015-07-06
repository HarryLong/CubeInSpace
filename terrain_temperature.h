#ifndef TERRAIN_TEMPERATURE_H
#define TERRAIN_TEMPERATURE_H

#include "texture_element.h"

/***********************
 * TERRAIN TEMPERATURE *
 ***********************/
class TerrainTemperatureTexture : public TextureElement<GLbyte>{
public:
    TerrainTemperatureTexture();
    ~TerrainTemperatureTexture();
};

class TerrainTemperature
{
public:
    TerrainTemperature();
    ~TerrainTemperature();

    TerrainTemperatureTexture * getJunTemperature();
    TerrainTemperatureTexture * getDecTemperature();

    void invalidate();
    bool isValid();

private:
    TerrainTemperatureTexture * m_jun_temp;
    TerrainTemperatureTexture * m_dec_temp;
};


#endif // TERRAIN_TEMPERATURE_H
