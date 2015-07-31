#ifndef SOIL_HUMIDITY_H
#define SOIL_HUMIDITY_H

#include "../gl_texture/texture_element.h"
#include "glm/common.hpp"
#include <QObject>

class SoilHumidityHeightmap : public TextureElement<GLuint> // TODO: And GL Drawable to actually draw the water bruuuu
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
//    void pushToGPU();
    void syncFromGPU();
    void reset(int width, int height);
    SoilHumidityHeightmap& operator[](int);

private:
    SoilHumidityHeightmap m_soil_humidity[12];
};

#endif // SOIL_HUMIDITY_H
