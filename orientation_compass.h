#ifndef ORIENTATION_COMPASS_H
#define ORIENTATION_COMPASS_H

#include "gl_drawable.h"
#include "glm/glm.hpp"
#include "shape_factory.h"

/***********
 * CONTOUR *
 ***********/
class Contour : public GlCircle
{
public:
    Contour();
    virtual void render() const;
};

/***************
 * NORTH ARROW *
 ***************/
class NorthArrow : public GlArrow
{
public:
    NorthArrow();
    virtual void render() const;
};

//------------------------------

class OrientationCompass  {
public:
    OrientationCompass();
    ~OrientationCompass();

    void rotateNorth(float rotation);

    std::vector<Asset*> getAssets();

    glm::vec2 getNorthOrientation();
    glm::mat4x4 getNorthRotationMatrix();

private:
    float m_north_rotation;
    Contour * get_contour();
    NorthArrow * get_arrow();

    Contour * m_contour;
    NorthArrow * m_arrow;
};

#endif // ORIENTATION_COMPASS_H
