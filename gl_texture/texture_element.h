#ifndef TEXTURE_ELEMENT_H
#define TEXTURE_ELEMENT_H

#include "../gl_core/glheader.h"
#include <QOpenGLTexture>
#include "../fixed_size_stack.h"
#include <QObject>

class TextureElement : public QObject{
Q_OBJECT
public:
    TextureElement(QOpenGLTexture::Target target, QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format,
                   QOpenGLTexture::PixelType pixel_type);
    virtual ~TextureElement();
    void bind();
    int width();
    int height();
    int depth();
    virtual void setDimensions(int width, int height, int depth = 1) = 0;
    virtual void pushToGPU() = 0;
    GLuint textureId();
    QOpenGLTexture::Target textureTarget() const;
    QOpenGLTexture::TextureFormat textureFormat() const;
    QOpenGLTexture::PixelFormat pixelFormat() const;
    QOpenGLTexture::PixelType pixelType() const;

    static int pixel_format_to_color_element_count(const QOpenGLTexture::PixelFormat & format);
    static GLenum qt_texture_target_to_gl_texture_target(const QOpenGLTexture::Target & qt_format);
    static bool multi_level_target(const QOpenGLTexture::Target & qt_format);
protected:
    int get_index(int x, int y) const;
    int m_width, m_height, m_depth, m_color_element_count;
    QOpenGLTexture m_texture;
    QOpenGLTexture::Target m_texture_target;
    QOpenGLTexture::TextureFormat m_texture_format;
    QOpenGLTexture::PixelFormat m_pixel_format;
    QOpenGLTexture::PixelType m_pixel_type;
    void delete_texture();

private:
};

#endif // TEXTURE_ELEMENT_H
