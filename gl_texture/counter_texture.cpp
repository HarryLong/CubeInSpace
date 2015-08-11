#include "counter_texture.h"

/*****************
 * TERRAIN SHADE *
 *****************/
CounterTexture::CounterTexture() :
    TextureElement2D<GLuint>(QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

CounterTexture::~CounterTexture()
{

}
