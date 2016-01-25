#include "slope.h"

Slope::Slope() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

Slope::~Slope()
{

}

void Slope::setMin(float min)
{
    min_ = min;
}

void Slope::setMax(float max)
{
    max_ = max;
}

float Slope::getMin() const
{
    return min_;
}

float Slope::getMax() const
{
    return max_;
}
