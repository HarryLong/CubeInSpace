#ifndef TERRAIN_TEMPERATURE_H
#define TERRAIN_TEMPERATURE_H

#include "../gl_texture/texture_element_2d_array.h"

class TerrainTemperature : public TextureElement2DArray<GLbyte>
{
public:
    TerrainTemperature();
    ~TerrainTemperature();

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


#endif // TERRAIN_TEMPERATURE_H
