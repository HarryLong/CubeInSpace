#include "texture_element.h"
#include <QOpenGLContext>
#include <QWindow>
#include <cstring>

template <class T> TextureElement<T>::TextureElement(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format,
                                                     QOpenGLTexture::PixelType pixel_type) : QOpenGLTexture(QOpenGLTexture::Target::Target2D),
    m_texture_format(texture_format), m_pixel_format(pixel_format), m_pixel_type(pixel_type),
    m_valid(false), m_raw_data(nullptr)//, m_pushed_data(nullptr)
{
}

template <class T> TextureElement<T>::~TextureElement()
{
    if(m_raw_data)
        delete m_raw_data;
}

// Terrain shade functions
template <class T> bool TextureElement<T>::isValid() const
{
    return m_valid;
}

template <class T> void TextureElement<T>::setData(T * data, int w, int h, bool cache)
{
//    if(cache)
//        push();
    if(m_raw_data)
        free(m_raw_data);
    m_raw_data = data;
    m_width = w;
    m_depth = h;
    m_valid = true;
    pushToGPU();
}

template <class T> void TextureElement<T>::invalidate()
{
    m_valid = false;
}

template <class T> void TextureElement<T>::setValid(bool valid)
{
    m_valid = valid;
}

template <class T> T TextureElement<T>::operator()(int x, int z) const
{
    int index((z*m_width)+x);
    return m_raw_data[index];
}

template <class T> void TextureElement<T>::delete_texture()
{    
    if(QOpenGLTexture::isCreated())
    {
        QOpenGLTexture::destroy();
    }
}

template <class T> const T * TextureElement<T>::getRawData() const
{
    return m_raw_data;
}

template <class T> void TextureElement<T>::syncFromGPU(bool cache)
{
    this->bind();
    glGetTexImage(GL_TEXTURE_2D, 0, m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data ); CE();
}

template <class T> void TextureElement<T>::pushToGPU()
{
    if(m_valid)
    {
        delete_texture(); CE();

        QOpenGLTexture::create(); CE();
        QOpenGLTexture::setWrapMode(QOpenGLTexture::WrapMode::MirroredRepeat);
        QOpenGLTexture::bind();
        QOpenGLTexture::setSize(m_width, m_depth); CE();
        QOpenGLTexture::setFormat(m_texture_format); CE();

        QOpenGLTexture::allocateStorage(); CE();

        QOpenGLTexture::setData(m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data); CE();

        QOpenGLTexture::release();
    }
}

template <class T> QOpenGLTexture::TextureFormat TextureElement<T>::getTextureFormat() const
{
    return m_texture_format;
}

template <class T> QOpenGLTexture::PixelFormat TextureElement<T>::getPixelFormat() const
{
    return m_pixel_format;
}

template <class T> QOpenGLTexture::PixelType TextureElement<T>::getPixelType() const
{
    return m_pixel_type;
}

template <class T> void TextureElement<T>::set(int x, int y, T data)
{
    int index(y*m_width+x);
    m_raw_data[index] = data;
}

template <class T> void TextureElement<T>::pop()
{
//    if(m_pushed_data != NULL)
//        setData(m_pushed_data, width(), height());
}

template <class T> void TextureElement<T>::push()
{
//    if(m_pushed_data != NULL)
//        free(m_pushed_data);

//    // Make a copy
//    if(m_raw_data != NULL)
//    {
//        int sz(width()*height()*sizeof(T));
//        T * copy = (T*) std::malloc(sz);
//        std::memcpy(copy, m_raw_data, sz);
//    }
}

template class TextureElement<GLubyte>;
template class TextureElement<GLbyte>;
template class TextureElement<GLfloat>;
template class TextureElement<GLuint>;
