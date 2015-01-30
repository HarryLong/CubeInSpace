#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "gl_drawable.h"

enum Shape{
    Cube,
};

class CubeDrawable : public GlDrawable{
public:
    CubeDrawable();
    bool bindBuffers();
};

class ShapesHolder{
public:
    ~ShapesHolder();
    GlDrawable * get(Shape shape);
private:
    GlDrawable * m_cube;
};

#endif // SHAPEFACTORY_H
