#include "texture_element.h"
#include <QOpenGLContext>
#include <QWindow>
#include <cstring>
#include <assert.h>

template <class T> TextureElement<T>::TextureElement(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format,
                                                     QOpenGLTexture::PixelType pixel_type) :
    m_texture(QOpenGLTexture::Target::Target2D), m_texture_format(texture_format), m_pixel_format(pixel_format), m_pixel_type(pixel_type),
    m_raw_data(nullptr), m_stack(10)
{
}

template <class T> TextureElement<T>::~TextureElement()
{
    if(m_raw_data)
        delete m_raw_data;
}

template <class T> void TextureElement<T>::setData(T * data, int w, int h, bool stack)
{
    if(w != m_width || h != m_height)
        m_stack.clear();

    if(stack)
        push();
    if(m_raw_data)
        delete m_raw_data;
    m_raw_data = data;
    m_width = w;
    m_height = h;
    pushToGPU();
}

template <class T> T TextureElement<T>::operator()(int x, int z) const
{
    int index((z*m_width)+x);
    return m_raw_data[index];
}

template <class T> void TextureElement<T>::delete_texture()
{    
    if(m_texture.isCreated())
        m_texture.destroy();
}

template <class T> const T * TextureElement<T>::getRawData() const
{
    return m_raw_data;
}

template <class T> void TextureElement<T>::syncFromGPU(bool stack)
{
    if(stack)
        push();
    m_texture.bind();
    glGetTexImage(GL_TEXTURE_2D, 0, m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data ); CE();
}

template <class T> void TextureElement<T>::pushToGPU()
{
    assert(m_raw_data != nullptr && width() != 0 && height() != 0);

    delete_texture(); CE();

    m_texture.create(); CE();
    m_texture.setWrapMode(QOpenGLTexture::WrapMode::MirroredRepeat);

    m_texture.bind();
    m_texture.setSize(m_width, m_height); CE();
    m_texture.setFormat(m_texture_format); CE();
    m_texture.allocateStorage(); CE();
    m_texture.setData(m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data); CE();

    m_texture.release();
}

template <class T> QOpenGLTexture::TextureFormat TextureElement<T>::textureFormat() const
{
    return m_texture_format;
}

template <class T> QOpenGLTexture::PixelFormat TextureElement<T>::pixelFormat() const
{
    return m_pixel_format;
}

template <class T> QOpenGLTexture::PixelType TextureElement<T>::pixelType() const
{
    return m_pixel_type;
}

template <class T> void TextureElement<T>::set(int x, int y, T data)
{
    int index(y*m_width+x);
    m_raw_data[index] = data;
}

template <class T> void TextureElement<T>::bind()
{
    m_texture.bind();
}

template <class T> int TextureElement<T>::width()
{
    return m_width;
}

template <class T> int TextureElement<T>::height()
{
    return m_height;
}

template <class T> GLuint TextureElement<T>::textureId()
{
    return m_texture.textureId();
}

template <class T> void TextureElement<T>::pop()
{
    if(!m_stack.empty())
    {
        setData(m_stack.pop(), m_width, m_height, false);
    }
}

template <class T> void TextureElement<T>::push()
{
    if(m_raw_data)
    {
        T * cpy = new T[m_width*m_height];
        std::memcpy(cpy, m_raw_data, sizeof(T)*m_width*m_height);
        m_stack.push(cpy);
    }
}

template class TextureElement<GLubyte>;
template class TextureElement<GLbyte>;
template class TextureElement<GLfloat>;
template class TextureElement<GLuint>;
