#ifndef TERRAIN_TEMPERATURE_H
#define TERRAIN_TEMPERATURE_H

#include "../gl_texture/texture_element.h"

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

    void bindDec();
    void bindJun();

    void setData(GLbyte * jun_data, GLbyte * dec_data, int width, int height);

    void pushToGPU();

    void invalidate();
    bool isValid();

    void getTempData(int x, int z, GLbyte & jun_temp, GLbyte & dec_temp);

private:
    TerrainTemperatureTexture * m_jun_temp;
    TerrainTemperatureTexture * m_dec_temp;
};


#endif // TERRAIN_TEMPERATURE_H
