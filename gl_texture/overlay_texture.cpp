#include "overlay_texture.h"

OverlayTexture::OverlayTexture() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::RGBA32F, QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::Float32)
{

}

OverlayTexture::~OverlayTexture()
{

}
