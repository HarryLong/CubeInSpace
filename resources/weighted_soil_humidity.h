#ifndef SOIL_HUMIDITY_H
#define SOIL_HUMIDITY_H

#include "../gl_texture/texture_element.h"
#include "glm/common.hpp"
#include <QObject>

class WeightedSoilHumidityHeightmap : public TextureElement<GLfloat>
{
public:
    WeightedSoilHumidityHeightmap();
    ~WeightedSoilHumidityHeightmap();
};

class WeightedSoilHumidity : public QObject
{
public:
    WeightedSoilHumidity();
    ~WeightedSoilHumidity();
    void reset(int width, int height);
    WeightedSoilHumidityHeightmap& operator[](int);

private:
    WeightedSoilHumidityHeightmap m_weighted_soil_humidity[12];
};

#endif // SOIL_HUMIDITY_H
