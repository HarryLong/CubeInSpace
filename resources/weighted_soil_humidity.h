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

    void setMin(int month, float min);
    void setMax(int month, float max);

    float getMin(int month) const;
    float getMax(int month) const;
    float getMin() const;
    float getMax() const;

private:
    std::vector<float> min_;
    std::vector<float> max_;
    float min_min_;
    float max_max_;
};

#endif // SOIL_HUMIDITY_H
