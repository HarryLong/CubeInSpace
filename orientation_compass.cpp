#include "orientation_compass.h"
#include "utils/utils.h"
#include <glm/gtc/matrix_transform.hpp>

#define RADIUS 100.0f

OrientationCompass::OrientationCompass() : m_circle(NULL), m_arrow(NULL)
{

}

OrientationCompass::~OrientationCompass()
{
    delete m_circle;
}

DrawData OrientationCompass::getContour()
{
    if(m_circle == NULL)
        m_circle = new Circle(ShapeFactory::getCircle(RADIUS));

    return m_circle->getDrawData();
}

DrawData OrientationCompass::getNorthArrow()
{
    if(m_arrow == NULL)
        m_arrow = new Arrow(ShapeFactory::getArrow(RADIUS));

    return m_arrow->getDrawData();
}


void OrientationCompass::rotateNorth(float rotation)
{
    m_north_rotation += rotation;
    Utils::normalizeAngle(m_north_rotation);
}

glm::vec2 OrientationCompass::getNorthOrientation()
{

}

glm::mat4x4 OrientationCompass::getNorthRotationMatrix()
{
    return glm::rotate(glm::mat4x4(), m_north_rotation, glm::vec3(0,1,0));
}
