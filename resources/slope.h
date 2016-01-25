#ifndef SLOPE_H
#define SLOPE_H

#include "../gl_texture/texture_element_2d.h"

class Slope : public TextureElement2D<GLfloat>
{
public:
    Slope();
    ~Slope();

    void setMin(float min);
    void setMax(float max);

    float getMin() const;
    float getMax() const;

private:
    float min_;
    float max_;
};

#endif // SLOPE_H
