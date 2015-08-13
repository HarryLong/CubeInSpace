#ifndef TEXTURE_ELEMENT_2D_ARRAY_H
#define TEXTURE_ELEMENT_2D_ARRAY_H

#include "texture_element.h"
#include "../fixed_size_stack.h"

template <class T> class TextureElement2DArray : public TextureElement{
public:
    TextureElement2DArray(int layers, QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format, QOpenGLTexture::PixelType pixel_type);
    virtual ~TextureElement2DArray();

    QOpenGLTexture* operator[](int layer);
    virtual T operator()(int layer, int x, int y) const;
    virtual void setDimensions(int width, int height, int depth = 1);

    int layers() const;

    /***********************
     * PER LAYER FUNCTIONS *
     ***********************/
    virtual void setData(int layer, T * data, bool stack = false);
    virtual void reset(int layer, bool stack = false);
    void set(int layer, T data, int x, int y);
    virtual void syncFromGPU(int layer, bool stack = false);
    virtual void pushToGPU(int layer);
    void bind_layer(int layer);

    /*******************************
     * AGGREGATE TEXTURE FUNCTIONS *
     *******************************/
    virtual void setData(T * data, bool stack = false);
    virtual void reset(bool stack = false);
    virtual void reset(int w, int h);
    virtual void syncFromGPU(bool stack = false);
    virtual void pushToGPU();

    const T * getRawData(int layer) const;

    void push();
    void push(int layer);
    void pop(int layer);
    void pop();

protected:

private:
    void refresh_texture_views();
    void individualize_raw_data(T * raw_data);
    void push_all_layers();
    void pop_all_layers();

    void clear_stacks();
    void clear_texture_views();
    void clear_raw_data();
    std::vector<QOpenGLTexture*> m_texture_views;
    std::vector<T*> m_raw_data;
    std::vector<FixedSizeStack<T> > m_stack;
    int m_layers;
};

#endif // TEXTURE_ELEMENT_2D_H
