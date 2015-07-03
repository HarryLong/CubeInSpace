#include "texture_element.h"

template <class T> TextureElement<T>::TextureElement(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format,
                                                     QOpenGLTexture::PixelType pixel_type) : QOpenGLTexture(QOpenGLTexture::Target::Target2D),
    m_texture_format(texture_format), m_pixel_format(pixel_format), m_pixel_type(pixel_type),
    m_valid(false), m_raw_data(NULL)
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
    delete_texture(); CE();

    this->create(); CE();
    this->bind();
    this->setSize(w, h); CE();
    this->setFormat(m_texture_format); CE();

    this->allocateStorage(); CE();

//    for(int y(0); y < h; y++)
//    {
//        for(int x(0); x < w; x++)
//        {
//            qCritical() << "SETTING DATA: X: " << x << " & Y: " << y << " [ " << data[y*x+x] << "]";
//        }
//    }

    QOpenGLTexture::setData(m_pixel_format, m_pixel_type, (GLvoid*) data); CE();

    this->release();
    if(m_raw_data)
        free(m_raw_data);
    m_raw_data = data;
    m_width = w;
    m_depth = h;
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

template <class T> void TextureElement<T>::synchronize_from_GPU()
{
    qCritical() << "Synchronizing texture from GPU!";

//    for(int y(0); y < height(); y++)
//    {
//        for(int x(0); x < width(); x++)
//        {
//            if((*this)(x, y) != 20)
//                qCritical() << "PRE Not 20 for X: " << x << " & Y: " << y << " [ " << (*this)(x, y) << "]";
//        }
//    }

    memset(m_raw_data, 0x01, width()*height()*sizeof(GLuint));

    this->bind();
    glGetTexImage(GL_TEXTURE_2D, 0, m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data ); CE();

//    for(int y(0); y < height(); y++)
//    {
//        for(int x(0); x < width(); x++)
//        {
//            if((*this)(x, y) != 30)
//                qCritical() << "POST Not 30 for X: " << x << " & Y: " << y << " [ " << (*this)(x, y) << "]";
//        }
//    }
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
