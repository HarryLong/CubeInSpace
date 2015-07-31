#ifndef TEXTURE_ELEMENT_H
#define TEXTURE_ELEMENT_H

#include "../gl_core/glheader.h"
#include <QOpenGLTexture>
#include "../fixed_size_stack.h"

template <class T> class TextureElement {
public:
    TextureElement(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format, QOpenGLTexture::PixelType pixel_type);
    virtual ~TextureElement();
    // Terrain shade functions
    virtual void setData(T * data, int width, int depth, bool stack = false);
    virtual void syncFromGPU(bool stack = false);
    virtual void pushToGPU();
    virtual T operator()(int x, int z) const;
    const T * getRawData() const;
    void set(int x, int y, T data);
    void bind();
    int width();
    int height();
    GLuint textureId();
    QOpenGLTexture::TextureFormat textureFormat() const;
    QOpenGLTexture::PixelFormat pixelFormat() const;
    QOpenGLTexture::PixelType pixelType() const;

    void pop();
    void push();

protected:
    int m_width, m_height;

private:
    void delete_texture();
    T * m_raw_data;

    FixedSizeStack<T> m_stack;
    QOpenGLTexture m_texture;
    QOpenGLTexture::TextureFormat m_texture_format;
    QOpenGLTexture::PixelFormat m_pixel_format;
    QOpenGLTexture::PixelType m_pixel_type;
};

#endif // TEXTURE_ELEMENT_H
