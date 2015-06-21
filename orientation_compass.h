#ifndef ORIENTATION_COMPASS_H
#define ORIENTATION_COMPASS_H

#include "gl_drawable.h"
#include "glm/glm.hpp"
#include "shape_factory.h"
#include "controllers.h"

/***********
 * CONTOUR *
 ***********/
class Contour : public GlCircle
{
public:
    Contour();
    virtual void render();
};

/***************
 * NORTH ARROW *
 ***************/
class NorthArrow : public GlArrow
{
public:
    NorthArrow();
    virtual void render();
};

/********************
 * TRUE NORTH ARROW *
 *******************/
class TrueNorthArrow : public GlArrow
{
public:
    TrueNorthArrow();
    virtual void render();
};

//------------------------------

class OrientationCompass : public QObject{
Q_OBJECT
public:
    OrientationCompass(PositionControllers & position_controllers);
    ~OrientationCompass();

    void rotateNorth(float rotation);

    std::vector<Asset*> getAssets();

    glm::vec3 getNorthOrientation() const;
    glm::vec3 getTrueNorthOrientation() const;
    glm::vec3 getEastOrientation() const;

signals:
    void orientationChanged(float north_x, float north_y, float north_z,
                            float true_north_x, float true_north_y, float true_north_z,
                            float east_x, float east_y, float east_z);

public slots:
    void setLatitude(int latitude);
    void setTerrainDimensions(int width, int depth, int base_height, int max_height);

private:
    float m_north_rotation;

    Contour * get_contour();
    NorthArrow * get_north_arrow();
    TrueNorthArrow * get_true_north_arrow();

    void refresh_true_north();
    void refresh_contour();
    void refresh_north();
    void emit_orientation_changed();

    Contour * m_contour;
    NorthArrow * m_north_arrow;
    TrueNorthArrow * m_true_north_arrow;

    int m_latitude;

    glm::vec3 m_north_orientation;
    glm::vec3 m_east_orientation;
    glm::vec3 m_true_north_orientation;
    glm::mat4x4 m_north_rotation_mat;
    glm::mat4x4 m_true_north_rotation_mat;
    glm::mat4x4 m_center_translation_mat;
};

#endif // ORIENTATION_COMPASS_H
