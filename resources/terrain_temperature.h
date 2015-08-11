#ifndef TERRAIN_TEMPERATURE_H
#define TERRAIN_TEMPERATURE_H

#include "../gl_texture/texture_element_2d_array.h"

/***********************
 * TERRAIN TEMPERATURE *
 ***********************/
//class TerrainTemperatureTexture : public TextureElement2D<GLbyte>{
//public:
//    TerrainTemperatureTexture();
//    ~TerrainTemperatureTexture();
//};

class TerrainTemperature : public TextureElement2DArray<GLbyte>
{
public:
    static const int _JUN_LAYER_IDX = 0;
    static const int _DEC_LAYER_IDX = 1;

    TerrainTemperature();
    ~TerrainTemperature();

//    TerrainTemperatureTexture & getJun();
//    TerrainTemperatureTexture & getDec();

//    void setData(GLbyte * jun_data, GLbyte * dec_data, int width, int height);

//    void getTempData(int x, int z, GLbyte & jun_temp, GLbyte & dec_temp);

//private:
//    TerrainTemperatureTexture m_jun_temp;
//    TerrainTemperatureTexture m_dec_temp;
};


#endif // TERRAIN_TEMPERATURE_H
