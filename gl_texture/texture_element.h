#ifndef TEXTURE_ELEMENT_H
#define TEXTURE_ELEMENT_H

#include "../gl_core/glheader.h"
#include <QOpenGLTexture>

template <class T> class TextureElement : public QOpenGLTexture {
public:
    TextureElement(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format, QOpenGLTexture::PixelType pixel_type);
    virtual ~TextureElement();
    // Terrain shade functions
    virtual void setData(T * data, int width, int depth, bool cache = false);
    virtual void syncFromGPU(bool cache = false);
    virtual void pushToGPU();
    virtual T operator()(int x, int z) const;
    const T * getRawData() const;
    bool isValid() const;
    void setValid(bool valid);
    void invalidate();
    void set(int x, int y, T data);
    QOpenGLTexture::TextureFormat getTextureFormat() const;
    QOpenGLTexture::PixelFormat getPixelFormat() const;
    QOpenGLTexture::PixelType getPixelType() const;

    void pop();
    void push();

protected:
//    bool m_requires_push_to_GPU;
    bool m_valid;
    int m_width, m_depth;

private:
    void delete_texture();
//    T * m_pushed_data;
    T * m_raw_data;

    QOpenGLTexture::TextureFormat m_texture_format;
    QOpenGLTexture::PixelFormat m_pixel_format;
    QOpenGLTexture::PixelType m_pixel_type;
};

#endif // TEXTURE_ELEMENT_H
