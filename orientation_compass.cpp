#include "orientation_compass.h"
#include "glm_rotations.h"
#include "geom.h"

#define RADIUS 100.0f
#define NORTH_ARROW_COLOR glm::vec4(1,0,0,0)
#define TRUE_NORTH_ARROW_COLOR glm::vec4(0,1,0,0)
#define CONTOUR_COLOR glm::vec4(0,0,0,0)
/***********
 * CONTOUR *
 ***********/
Contour::Contour() : GlCircle(RADIUS, CONTOUR_COLOR)
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
NorthArrow::NorthArrow() : GlArrow(RADIUS-3, NORTH_ARROW_COLOR)
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

/********************
 * TRUE NORTH ARROW *
 *******************/
TrueNorthArrow::TrueNorthArrow() : GlArrow(RADIUS-3, TRUE_NORTH_ARROW_COLOR)
{

}

void TrueNorthArrow::render() const
{
    GLfloat base_line_width;
    glGetFloatv(GL_LINE_WIDTH, &base_line_width);
    glLineWidth(5);

    GlArrow::render();

    glLineWidth(base_line_width);
}

//------------------------------

OrientationCompass::OrientationCompass() : m_contour(NULL), m_true_north_arrow(NULL), m_north_orientation(GlArrow::_base_orientation),
    m_latitude(0), m_east_orientation(glm::rotateY(m_north_orientation, (float) M_PI_2)), m_true_north_orientation(m_north_orientation)
{
}

OrientationCompass::~OrientationCompass()
{
    if(m_contour)
        delete m_contour;
    if(m_true_north_arrow)
        delete m_true_north_arrow;
    if(m_north_arrow)
        delete m_north_arrow;
}

std::vector<Asset*> OrientationCompass::getAssets()
{
    return std::vector<Asset*>{get_contour(), get_north_arrow(), get_true_north_arrow()};
}

Contour * OrientationCompass::get_contour()
{
    if(!m_contour)
        m_contour = new Contour();

    return m_contour;
}

NorthArrow * OrientationCompass::get_north_arrow()
{
    if(!m_north_arrow)
    {
        m_north_arrow = new NorthArrow();
    }

    return m_north_arrow;
}

TrueNorthArrow * OrientationCompass::get_true_north_arrow()
{
    if(!m_true_north_arrow)
    {
        m_true_north_arrow = new TrueNorthArrow();
    }

    return m_true_north_arrow;
}

void OrientationCompass::rotateNorth(float rotation)
{
    m_north_rotation += rotation;
    Geom::normalizeDegrees(m_north_rotation);

    refresh_north();
    refresh_true_north();

    emit_orientation_changed();
}

glm::vec3 OrientationCompass::getNorthOrientation() const
{
    return m_north_orientation;
}

glm::vec3 OrientationCompass::getTrueNorthOrientation() const
{
    return m_true_north_orientation;
}

glm::vec3 OrientationCompass::getEastOrientation() const
{
    return m_east_orientation;
}

void OrientationCompass::setLatitude(int latitude)
{
    m_latitude = latitude;
    refresh_true_north();
    emit_orientation_changed();
}

void OrientationCompass::refresh_true_north()
{
    m_true_north_orientation = glm::rotate(m_north_orientation, Geom::toRadians(-m_latitude), m_east_orientation);
    m_true_north_rotation_mat = glm::rotate(glm::mat4x4(), Geom::toRadians(-m_latitude), m_east_orientation);
    get_true_north_arrow()->setMtwMat(m_center_translation_mat * m_true_north_rotation_mat * m_north_rotation_mat);
}

void OrientationCompass::refresh_contour()
{
    get_contour()->setMtwMat(m_center_translation_mat);
}

void OrientationCompass::refresh_north()
{
    m_north_rotation_mat = glm::rotate(glm::mat4x4(), Geom::toRadians(m_north_rotation), glm::vec3(0,1,0));
    m_north_orientation = glm::rotateY(m_north_arrow->_base_orientation, Geom::toRadians(m_north_rotation));
    m_east_orientation = glm::rotateY(m_north_orientation, (float) M_PI_2);
    get_north_arrow()->setMtwMat(m_center_translation_mat * m_north_rotation_mat);
}

#include <iostream>
void OrientationCompass::setCenterPosition(glm::vec3 center)
{
    std::cout << "Center : " << center[0] << ", " << center[1] << ", " << center[2] << ", " << std::endl;
    m_center_translation_mat = glm::translate(glm::mat4x4(), center);
    refresh_contour();
    refresh_north();
    refresh_true_north();
    emit_orientation_changed();
}

void OrientationCompass::emit_orientation_changed()
{
    emit orientationChanged(m_north_orientation[0], m_north_orientation[1], m_north_orientation[2],
                            m_true_north_orientation[0], m_true_north_orientation[1], m_true_north_orientation[2],
                            m_east_orientation[0], m_east_orientation[1], m_east_orientation[2]);
}
