#ifndef TEXTURE_ELEMENT_H
#define TEXTURE_ELEMENT_H

#include "glheader.h"
#include <QOpenGLTexture>

template <class T> class TextureElement : public QOpenGLTexture {
public:
    TextureElement(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format, QOpenGLTexture::PixelType pixel_type);
    virtual ~TextureElement();
    // Terrain shade functions
    bool isValid() const;
    void setData(T * data, int width, int depth);
    void invalidate();
    virtual void synchronize_from_GPU();
    virtual T operator()(int x, int z) const;
    const T * getRawData() const;
    QOpenGLTexture::TextureFormat getTextureFormat() const;
    QOpenGLTexture::PixelFormat getPixelFormat() const;
    QOpenGLTexture::PixelType getPixelType() const;

protected:
    T * m_raw_data;
    int m_width, m_depth;
    bool m_valid;

private:
    void delete_texture();

    QOpenGLTexture::TextureFormat m_texture_format;
    QOpenGLTexture::PixelFormat m_pixel_format;
    QOpenGLTexture::PixelType m_pixel_type;
};

#endif // TEXTURE_ELEMENT_H
