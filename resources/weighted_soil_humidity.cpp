#include "weighted_soil_humidity.h"
#include <cstring>

WeightedSoilHumidity::WeightedSoilHumidity() :
    TextureElement2DArray<GLfloat>(12, QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32),
    min_min_(-1), max_max_(-1)
{
    min_.resize(12);
    max_.resize(12);
}

WeightedSoilHumidity::~WeightedSoilHumidity()
{

}

void WeightedSoilHumidity::setMin(int month, float min)
{
    if(min_min_ == -1 || min < min_min_)
        min_min_ = min;
    min_.at(month-1) = min;
}

void WeightedSoilHumidity::setMax(int month, float max)
{
    if(max_max_ == -1 || max > max_max_)
        max_max_ = max;
    max_.at(month-1) = max;
}

float WeightedSoilHumidity::getMin(int month) const
{
    return min_.at(month);
}

float WeightedSoilHumidity::getMax(int month) const
{
    return max_.at(month);
}

float WeightedSoilHumidity::getMin() const
{
    return min_min_;
}

float WeightedSoilHumidity::getMax() const
{
    return max_max_;
}
