#include "orientation_compass.h"

#define GLM_FORCE_RADIANS

#include "utils/utils.h"
#include <glm/gtc/matrix_transform.hpp>

#define RADIUS 100.0f
#define ARROW_COLOR glm::vec4(1,0,0,0)
#define CONTOUR_COLOR glm::vec4(0,0,0,0)

/***********
 * CONTOUR *
 ***********/
Contour::Contour() : GlCircle(RADIUS, true, CONTOUR_COLOR)
{

}

void Contour::render() const
{
    GLfloat base_line_width;
    glGetFloatv(GL_LINE_WIDTH, &base_line_width);
    glLineWidth(5);

    GlCircle::render();

    glLineWidth(base_line_width);
}

/***************
 * NORTH ARROW *
 ***************/
NorthArrow::NorthArrow() : GlArrow(RADIUS, true, ARROW_COLOR)
{

}

void NorthArrow::render() const
{
    GLfloat base_line_width;
    glGetFloatv(GL_LINE_WIDTH, &base_line_width);
    glLineWidth(5);

    GlArrow::render();

    glLineWidth(base_line_width);
}

//------------------------------

OrientationCompass::OrientationCompass() : m_contour(NULL), m_arrow(NULL)
{

}

OrientationCompass::~OrientationCompass()
{
    if(m_contour)
        delete m_contour;
    if(m_arrow)
        delete m_arrow;
}

std::vector<Asset*> OrientationCompass::getAssets()
{
    return std::vector<Asset*>{get_contour(), get_arrow()};
}

Contour * OrientationCompass::get_contour()
{
    if(!m_contour)
        m_contour = new Contour();

    return m_contour;
}

NorthArrow * OrientationCompass::get_arrow()
{
    if(!m_arrow)
        m_arrow = new NorthArrow();

    return m_arrow;
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
    return glm::rotate(glm::mat4x4(), Utils::toRadians(m_north_rotation), glm::vec3(0,1,0));
}
