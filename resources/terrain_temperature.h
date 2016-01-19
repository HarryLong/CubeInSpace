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

private:
    std::vector<float> min_;
    std::vector<float> max_;
};


#endif // TERRAIN_TEMPERATURE_H
