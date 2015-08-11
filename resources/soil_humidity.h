#ifndef WEIGHTED_SOIL_HUMIDITY_H
#define WEIGHTED_SOIL_HUMIDITY_H

#include "../gl_texture/texture_element_2d.h"
#include "glm/common.hpp"
#include <QObject>

class SoilHumidityHeightmap : public TextureElement2D<GLuint>
{
public:
    SoilHumidityHeightmap();
    ~SoilHumidityHeightmap();
};

class SoilHumidity : public QObject
{
public:
    SoilHumidity();
    ~SoilHumidity();
    void reset(int width, int height);
    SoilHumidityHeightmap& operator[](int);

private:
    SoilHumidityHeightmap m_soil_humidity[12];
};

#endif // WEIGHTED_SOIL_HUMIDITY_H
