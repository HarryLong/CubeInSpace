#include "texture_element.h"

template <class T> TextureElement<T>::TextureElement(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format,
                                                     QOpenGLTexture::PixelType pixel_type) : QOpenGLTexture(QOpenGLTexture::Target::Target2D),
    m_texture_format(texture_format), m_pixel_format(pixel_format), m_pixel_type(pixel_type),
    m_valid(false), m_raw_data(NULL)
{

}

template <class T> TextureElement<T>::~TextureElement()
{
    delete_texture();
    if(m_raw_data)
        delete m_raw_data;
}

// Terrain shade functions
template <class T> bool TextureElement<T>::isValid() const
{
    return m_valid;
}

template <class T> void TextureElement<T>::setData(T * data, int width, int depth)
{
    delete_texture();

    this->create();
    this->setSize(width, depth);
    this->setFormat(m_texture_format);

    this->allocateStorage();
    QOpenGLTexture::setData(m_pixel_format, m_pixel_type, (GLvoid*) data);

    if(m_raw_data)
        free(m_raw_data);
    m_raw_data = data;
    m_width = width;
    m_depth = depth;
    m_valid = true;
}

template <class T> void TextureElement<T>::invalidate()
{
    m_valid = false;
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

template class TextureElement<GLubyte>;
template class TextureElement<GLbyte>;
template class TextureElement<GLfloat>;
