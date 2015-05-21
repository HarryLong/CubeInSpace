#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include "gl_drawable.h"

/**********
 * CIRCLE *
 **********/
class Circle : public GlDrawable {
public:
    Circle(float radius);
    ~Circle();
    virtual bool bindBuffers();

private:
    void init();
    float m_radius;
};

/*********
 * ARROW *
 *********/
class Arrow : public GlDrawable {
public:
    Arrow(float length);
    ~Arrow();
    virtual bool bindBuffers();

private:
    void init();
    float m_length;
};

/*****************
 * SHAPE FACTORY *
 *****************/
class ShapeFactory {
public:
    static Circle getCircle(float radius);
    static Arrow getArrow(float length);
};

#endif // SHAPE_FACTORY
