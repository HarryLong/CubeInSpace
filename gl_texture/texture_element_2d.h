#ifndef TEXTURE_ELEMENT_2D_H
#define TEXTURE_ELEMENT_2D_H

#include "texture_element.h"
#include "../fixed_size_stack.h"

template <class T> class TextureElement2D : public TextureElement{
public:
    TextureElement2D(QOpenGLTexture::TextureFormat texture_format, QOpenGLTexture::PixelFormat pixel_format, QOpenGLTexture::PixelType pixel_type);
    virtual ~TextureElement2D();
    // Terrain shade functions
    virtual void setData(T * data, bool stack = false);
    virtual void reset(int width, int height);
    virtual T operator()(int x, int y) const;
    void set(T data, int x, int y);
    virtual void setDimensions(int width, int height, int depth = 1);

    virtual void syncFromGPU(bool stack = false);
    virtual void pushToGPU();
    const T * getRawData() const;

    void pop();
    void push();

protected:

private:
    T * m_raw_data;

    FixedSizeStack<T> m_stack;
};

#endif // TEXTURE_ELEMENT_2D_H
