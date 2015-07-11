#include "texture_element.h"
#include <QOpenGLContext>
#include <QWindow>

template <class T> TextureElement<T>::TextureElement(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format,
                                                     QOpenGLTexture::PixelType pixel_type) : QOpenGLTexture(QOpenGLTexture::Target::Target2D),
    m_texture_format(texture_format), m_pixel_format(pixel_format), m_pixel_type(pixel_type),
    m_valid(false), m_requires_push_to_GPU(true), m_raw_data(NULL)
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

template <class T> void TextureElement<T>::setData(T * data, int w, int h)
{
    if(m_raw_data)
        free(m_raw_data);
    m_raw_data = data;
    m_width = w;
    m_depth = h;
    m_valid = true;
    m_requires_push_to_GPU = true;
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
    if(this->isCreated()) this->destroy();
}

template <class T> const T * TextureElement<T>::getRawData() const
{
    return m_raw_data;
}

template <class T> void TextureElement<T>::syncFromGPU()
{
    this->bind();
    glGetTexImage(GL_TEXTURE_2D, 0, m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data ); CE();
}

template <class T> void TextureElement<T>::pushToGPU()
{
    if(m_valid && m_requires_push_to_GPU)
    {
        delete_texture(); CE();

        this->create(); CE();
        setWrapMode(QOpenGLTexture::WrapMode::MirroredRepeat);
        this->bind();
        this->setSize(m_width, m_depth); CE();
        this->setFormat(m_texture_format); CE();

        this->allocateStorage(); CE();

        QOpenGLTexture::setData(m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data); CE();

        this->release();

        m_requires_push_to_GPU = false;
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

template class TextureElement<GLubyte>;
template class TextureElement<GLbyte>;
template class TextureElement<GLfloat>;
template class TextureElement<GLuint>;
