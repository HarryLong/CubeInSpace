#ifndef TERRAIN_DAILY_ILLUMINATION_H
#define TERRAIN_DAILY_ILLUMINATION_H

#include "../gl_texture/texture_element_2d_array.h"

class TerrainDailyIllumination : public TextureElement2DArray<GLubyte>
{
public:
    TerrainDailyIllumination();
    ~TerrainDailyIllumination();

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



#endif // TERRAIN_DAILY_ILLUMINATION_H
