#ifndef SOIL_HUMIDITY_H
#define SOIL_HUMIDITY_H

#include "../gl_texture/texture_element_2d_array.h"
#include "glm/common.hpp"
#include <QObject>

class WeightedSoilHumidity : public TextureElement2DArray<GLfloat>
{
public:
    WeightedSoilHumidity();
    ~WeightedSoilHumidity();
};

#endif // SOIL_HUMIDITY_H
