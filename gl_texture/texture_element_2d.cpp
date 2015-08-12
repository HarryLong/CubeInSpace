#include "texture_element_2d.h"
#include <QOpenGLContext>
#include <QWindow>
#include <cstring>
#include <assert.h>
#include <QOpenGLFunctions_4_5_Core>

template <class T> TextureElement2D<T>::TextureElement2D(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format,
                                                     QOpenGLTexture::PixelType pixel_type) :
    TextureElement(QOpenGLTexture::Target::Target2D, texture_format, pixel_format, pixel_type),
    m_raw_data(nullptr), m_stack(10)
{
}

template <class T> TextureElement2D<T>::~TextureElement2D()
{
    if(m_raw_data)
        delete m_raw_data;
}

template <class T> void TextureElement2D<T>::setDimensions(int width, int height, int depth)
{
    if(m_width != width || m_height != height || m_depth != depth)
    {
        m_stack.clear();

        m_width = width;
        m_height = height;
        m_depth = depth;

        delete_texture();
        m_texture.create(); CE();

        m_texture.bind();

        m_texture.setFormat(m_texture_format); CE();
        m_texture.setSize(m_width, m_height); CE();
        m_texture.allocateStorage(m_pixel_format, m_pixel_type); CE();
        m_texture.setWrapMode(QOpenGLTexture::WrapMode::MirroredRepeat);CE();
        m_texture.setMinMagFilters(QOpenGLTexture::Filter::Linear, QOpenGLTexture::Filter::Linear);

        m_texture.release();
    }
}

template <class T> void TextureElement2D<T>::setData(T * data, bool stack)
{
    if(stack)
        push();
    if(m_raw_data)
        delete m_raw_data;

    m_raw_data = data;

    pushToGPU();
}

template <class T> T TextureElement2D<T>::operator()(int x, int y) const
{
    return m_raw_data[get_index(x,y)];
}

template <class T> void TextureElement2D<T>::reset(int w, int h)
{
    setDimensions(w,h);    // First set the dimensions

    T * data = new T[w*h*m_color_element_count];
    std::memset(data, 0, w*h*m_color_element_count*sizeof(T));
    setData(data);
}

template <class T> const T * TextureElement2D<T>::getRawData() const
{
    return m_raw_data;
}

template <class T> void TextureElement2D<T>::syncFromGPU(bool stack)
{
    if(stack)
        push();
    m_texture.bind();
    glGetTexImage(GL_TEXTURE_2D, 0, m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data ); CE();
}

template <class T> void TextureElement2D<T>::pushToGPU()
{
    assert(m_raw_data != nullptr && width() != 0 && height() != 0 && depth() != 0);

    m_texture.bind(); CE();
    m_texture.setData(m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data); CE();
    m_texture.release();
}

template <class T> void TextureElement2D<T>::set(T data, int x, int y)
{
    m_raw_data[get_index(x,y)] = data;
}


template <class T> void TextureElement2D<T>::pop()
{
    if(!m_stack.empty())
    {
        setData(m_stack.pop(), false);
    }
}

template <class T> void TextureElement2D<T>::push()
{
    if(m_raw_data)
    {
        T * cpy = new T[m_width*m_height];
        std::memcpy(cpy, m_raw_data, sizeof(T)*m_width*m_height);
        m_stack.push(cpy);
    }
}

template class TextureElement2D<GLubyte>;
template class TextureElement2D<GLbyte>;
template class TextureElement2D<GLfloat>;
template class TextureElement2D<GLuint>;
