#ifndef ORIENTATION_COMPASS_H
#define ORIENTATION_COMPASS_H

#include "gl_drawable.h"
#include "glm/glm.hpp"
#include "shape_factory.h"

class OrientationCompass {
public:
    OrientationCompass();
    ~OrientationCompass();

    DrawData getContour();
    DrawData getNorthArrow();

    void rotateNorth(float rotation);
    glm::vec2 getNorthOrientation();
    glm::mat4x4 getNorthRotationMatrix();

private:
    float m_north_rotation;

    Circle * m_circle;
    Arrow * m_arrow;
};

#endif // ORIENTATION_COMPASS_H
