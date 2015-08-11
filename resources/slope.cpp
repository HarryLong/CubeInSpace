#include "slope.h"

Slope::Slope() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

Slope::~Slope()
{

}
