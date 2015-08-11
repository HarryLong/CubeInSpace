#include "texture_element.h"
#include <QOpenGLContext>
#include <QWindow>
#include <cstring>
#include <assert.h>
#include <QOpenGLFunctions_4_5_Core>

TextureElement::TextureElement(QOpenGLTexture::Target target, QOpenGLTexture::TextureFormat texture_format,
                               QOpenGLTexture::PixelFormat pixel_format, QOpenGLTexture::PixelType pixel_type) :
    m_texture_target(target), m_texture_format(texture_format), m_pixel_format(pixel_format), m_pixel_type(pixel_type), m_texture(target),
    m_color_element_count(TextureElement::pixel_format_to_color_element_count(pixel_format))
{
}

TextureElement::~TextureElement()
{
    delete_texture();
}

void TextureElement::delete_texture()
{
    if(m_texture.isCreated())
            m_texture.destroy();
}

QOpenGLTexture::Target TextureElement::textureTarget() const
{
    return m_texture_target;
}

QOpenGLTexture::TextureFormat TextureElement::textureFormat() const
{
    return m_texture_format;
}

QOpenGLTexture::PixelFormat TextureElement::pixelFormat() const
{
    return m_pixel_format;
}

QOpenGLTexture::PixelType TextureElement::pixelType() const
{
    return m_pixel_type;
}

void TextureElement::bind()
{
    m_texture.bind();
}

int TextureElement::width()
{
    return m_width;
}

int TextureElement::height()
{
    return m_height;
}

int TextureElement::depth()
{
    return m_depth;
}

GLuint TextureElement::textureId()
{
    return m_texture.textureId();
}

int TextureElement::get_index(int x, int y) const
{
    return m_width*y+x;
}

bool TextureElement::multi_level_target(const QOpenGLTexture::Target & qt_target)
{
    return qt_target == QOpenGLTexture::Target::Target1DArray ||
            qt_target == QOpenGLTexture::Target::Target2DArray ||
            qt_target == QOpenGLTexture::Target::TargetCubeMapArray ||
            qt_target == QOpenGLTexture::Target::Target2DMultisampleArray;
}

GLenum TextureElement::qt_texture_target_to_gl_texture_target(const QOpenGLTexture::Target & qt_target)
{
    switch(qt_target){
    case QOpenGLTexture::Target::Target1D:
        return GL_TEXTURE_1D;
    case QOpenGLTexture::Target::Target1DArray:
        return GL_TEXTURE_1D_ARRAY;
    case QOpenGLTexture::Target::Target2D:
        return GL_TEXTURE_2D;
    case QOpenGLTexture::Target::Target2DArray:
        return GL_TEXTURE_2D_ARRAY;
    case QOpenGLTexture::Target::Target3D:
        return GL_TEXTURE_3D;
    case QOpenGLTexture::Target::TargetCubeMap:
        return GL_TEXTURE_CUBE_MAP;
    case QOpenGLTexture::Target::TargetCubeMapArray:
        return GL_TEXTURE_CUBE_MAP_ARRAY;
    case QOpenGLTexture::Target::Target2DMultisample:
        return GL_TEXTURE_2D_MULTISAMPLE;
    case QOpenGLTexture::Target::Target2DMultisampleArray:
        return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
    case QOpenGLTexture::Target::TargetRectangle:
        return GL_TEXTURE_RECTANGLE;
    case QOpenGLTexture::Target::TargetBuffer:
        return GL_TEXTURE_BUFFER;
    }
}

 int TextureElement::pixel_format_to_color_element_count(const QOpenGLTexture::PixelFormat & format)
{
    switch(format)
    {
    case QOpenGLTexture::PixelFormat::NoSourceFormat:
        return 0;
    case QOpenGLTexture::PixelFormat::Red:
    case QOpenGLTexture::PixelFormat::Red_Integer:
    case QOpenGLTexture::PixelFormat::Stencil:
    case QOpenGLTexture::PixelFormat::Depth:
    case QOpenGLTexture::PixelFormat::DepthStencil:
    case QOpenGLTexture::PixelFormat::Alpha:
    case QOpenGLTexture::PixelFormat::Luminance:
    case QOpenGLTexture::PixelFormat::LuminanceAlpha:
        return 1;
    case QOpenGLTexture::PixelFormat::RG:
    case QOpenGLTexture::PixelFormat::RG_Integer:
        return 2;
    case QOpenGLTexture::PixelFormat::RGB:
    case QOpenGLTexture::PixelFormat::RGB_Integer:
        return 3;
    case QOpenGLTexture::PixelFormat::RGBA:
    case QOpenGLTexture::PixelFormat::BGRA:
    case QOpenGLTexture::PixelFormat::RGBA_Integer:
    case QOpenGLTexture::PixelFormat::BGRA_Integer:
        return 4;
    }
}
