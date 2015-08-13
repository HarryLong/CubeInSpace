#include "texture_element_2d_array.h"
#include <QOpenGLContext>
#include <QWindow>
#include <cstring>
#include <assert.h>
#include <QOpenGLFunctions_4_3_Core>

template <class T> TextureElement2DArray<T>::TextureElement2DArray(int layers, QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format,
                                                     QOpenGLTexture::PixelType pixel_type) :
    TextureElement(QOpenGLTexture::Target::Target2DArray, texture_format, pixel_format, pixel_type),
    m_layers(layers)
{
    for(int l(0); l < layers; l++)
    {
        m_raw_data.push_back(nullptr);
        m_stack.push_back(FixedSizeStack<T>(10));
    }
}

template <class T> TextureElement2DArray<T>::~TextureElement2DArray()
{
    clear_texture_views();
    clear_raw_data();
    clear_stacks();
}

template <class T> void TextureElement2DArray<T>::setDimensions(int width, int height, int depth)
{
    if(m_width != width || m_height != height || m_depth != depth)
    {
        clear_stacks();
        clear_raw_data();

        m_width = width;
        m_height = height;
        m_depth = depth;

        delete_texture();
        m_texture.create(); CE();

        m_texture.bind();

        m_texture.setFormat(m_texture_format); CE();
        m_texture.setSize(m_width, m_height); CE();
        m_texture.setLayers(m_layers);
        m_texture.allocateStorage(m_pixel_format, m_pixel_type); CE();

        m_texture.setMinMagFilters(QOpenGLTexture::Filter::Linear, QOpenGLTexture::Filter::Linear);
        m_texture.setWrapMode(QOpenGLTexture::WrapMode::MirroredRepeat);CE();

        m_texture.release();

        refresh_texture_views();
    }
}

template <class T> int TextureElement2DArray<T>::layers() const
{
    return m_layers;
}

template <class T> void TextureElement2DArray<T>::refresh_texture_views()
{
    clear_texture_views();

    for(int l(0); l < m_layers; l++)
    {
        QOpenGLTexture * view_texture = m_texture.createTextureView(QOpenGLTexture::Target::Target2D,
                                                                    m_texture_format,
                                                                    0,0,
                                                                    l,l);
        view_texture->setMinMagFilters(QOpenGLTexture::Filter::Linear, QOpenGLTexture::Filter::Linear);
        view_texture->setWrapMode(QOpenGLTexture::WrapMode::MirroredRepeat);
        assert(view_texture != 0);

        m_texture_views.push_back(view_texture);
    }
}

template <class T> T TextureElement2DArray<T>::operator()(int layer, int x, int y) const
{
    return m_raw_data[layer][get_index(x,y)];
}

template <class T> QOpenGLTexture* TextureElement2DArray<T>::operator[](int layer)
{
    assert(layer < m_layers);
    return m_texture_views[layer];
}

template <class T> const T * TextureElement2DArray<T>::getRawData(int layer) const
{
    return m_raw_data[layer];
}

/***********************
 * PER LAYER FUNCTIONS *
 ***********************/
template <class T> void TextureElement2DArray<T>::bind_layer(int layer)
{
    m_texture_views[layer]->bind();
}

template <class T> void TextureElement2DArray<T>::setData(int layer, T * data, bool stack)
{
    if(stack)
        push(layer);
    if(m_raw_data[layer])
        delete m_raw_data[layer];

    m_raw_data[layer] = data;

    pushToGPU(layer);
}

template <class T> void TextureElement2DArray<T>::reset(int layer, bool stack)
{
    T * data = new T[m_width*m_height*m_color_element_count];
    std::memset(data, 0, m_width*m_height*m_color_element_count*sizeof(T));
    setData(layer, data, stack);
}

template <class T> void TextureElement2DArray<T>::syncFromGPU(int layer, bool stack)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    if(stack)
        push(layer);
    m_texture_views[layer]->bind();
    f->glGetTexImage(GL_TEXTURE_2D, 0, m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data[layer] ); CE();
}

template <class T> void TextureElement2DArray<T>::pushToGPU(int layer)
{
    assert(m_raw_data[layer] != nullptr && width() != 0 && height() != 0 && depth() != 0);

    m_texture.setData(0, layer, m_pixel_format, m_pixel_type, (GLvoid*) m_raw_data[layer]); CE();
}

template <class T> void TextureElement2DArray<T>::set(int layer, T data, int x, int y)
{
    m_raw_data[layer][get_index(x,y)] = data;
}

/*******************************
 * AGGREGATE TEXTURE FUNCTIONS *
 *******************************/
template <class T> void TextureElement2DArray<T>::setData(T * data, bool stack)
{
    if(stack)
        push_all_layers();

    clear_raw_data();

    individualize_raw_data(data);

    pushToGPU();
}

template <class T> void TextureElement2DArray<T>::individualize_raw_data(T * raw_data)
{
    clear_raw_data();
    int per_layer_elements(m_width*m_height*m_color_element_count);
    int per_layer_sz(per_layer_elements*sizeof(T));
    for(int l (0); l < m_layers ; l++)
    {
         T * layer_data = new T[per_layer_elements];
         std::memcpy(layer_data, &raw_data[per_layer_elements*l], per_layer_sz);
         m_raw_data.push_back(layer_data);
    }

    delete raw_data;
}

template <class T> void TextureElement2DArray<T>::reset(bool stack)
{
    T * data = new T[m_width*m_height*m_color_element_count*m_layers];
    std::memset(data, 0, m_width*m_height*m_color_element_count*sizeof(T)*m_layers);
    setData(data, stack);
}

template <class T> void TextureElement2DArray<T>::reset(int w, int h)
{
    setDimensions(w, h);
    reset();
}

template <class T> void TextureElement2DArray<T>::syncFromGPU(bool stack)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    if(stack)
        push_all_layers();

    T * data = new T[m_width*m_height*m_color_element_count*m_layers];

    m_texture.bind();
    f->glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, m_pixel_format, m_pixel_type, (GLvoid*) data ); CE();

    individualize_raw_data(data);
}

template <class T> void TextureElement2DArray<T>::pushToGPU()
{
    for(int l(0); l < layers(); l++)
        pushToGPU(l);
}

template <class T> void TextureElement2DArray<T>::clear_texture_views()
{
    for(QOpenGLTexture * view_texture : m_texture_views)
    {
        if(view_texture)
        {
            view_texture->destroy();
            delete view_texture;
        }
    }
    m_texture_views.clear();
}

template <class T> void TextureElement2DArray<T>::clear_raw_data()
{
    for(T * data : m_raw_data)
    {
        if(data)
            delete data;
    }
    m_raw_data.clear();
}

template <class T> void TextureElement2DArray<T>::clear_stacks()
{
    for(FixedSizeStack<T> & stack : m_stack)
        stack.clear();
}

template <class T> void TextureElement2DArray<T>::pop_all_layers()
{
    for(int l(0); l < layers(); l++)
        pop(l);
}

template <class T> void TextureElement2DArray<T>::pop()
{
    pop_all_layers();
}

template <class T> void TextureElement2DArray<T>::pop(int layer)
{
    if(!m_stack[layer].empty())
    {
        setData(layer, m_stack[layer].pop());
    }
}

template <class T> void TextureElement2DArray<T>::push()
{
    push_all_layers();
}

template <class T> void TextureElement2DArray<T>::push(int layer)
{
    if(m_raw_data[layer])
    {
        T * cpy = new T[m_width*m_height];
        std::memcpy(cpy, m_raw_data[layer], sizeof(T)*m_width*m_height);
        m_stack[layer].push(cpy);
    }
}

template <class T> void TextureElement2DArray<T>::push_all_layers()
{
    for(int l(0); l < layers(); l++)
        push(l);
}

template class TextureElement2DArray<GLubyte>;
template class TextureElement2DArray<GLbyte>;
template class TextureElement2DArray<GLfloat>;
template class TextureElement2DArray<GLuint>;
