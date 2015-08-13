#ifndef WEIGHTED_SOIL_HUMIDITY_H
#define WEIGHTED_SOIL_HUMIDITY_H

#include "../gl_texture/texture_element_2d_array.h"
#include "glm/common.hpp"
#include <QObject>

//class SoilHumidityHeightmap : public TextureElement2D<GLuint>
//{
//public:
//    SoilHumidityHeightmap();
//    ~SoilHumidityHeightmap();
//};

class SoilHumidity : public TextureElement2DArray<GLuint>
{
public:
    SoilHumidity();
    ~SoilHumidity();
};

#endif // WEIGHTED_SOIL_HUMIDITY_H
