#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "gl_drawable.h"

class ShapesHolder : public GlDrawable{
public:
    void genCube();
    bool bindBuffers();
};

#endif // SHAPEFACTORY_H
