#ifndef VIEW_H
#define VIEW_H

#define PI 3.14159265
#define RADIANS_TO_DEGREES 57.2

#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "utils/utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

enum TransformType{
    Translation,
    Rotation
};

struct CameraOrientation
{
    float pitch; // Around the x axis
    float yaw; // Around the y axis
    glm::mat4x4 rotation_mat;

    CameraOrientation() : pitch(.0f), yaw(.0f) {}

    void calculate_rotation_matrix()
    {
        Utils::normalizeAngle(pitch);
        Utils::normalizeAngle(yaw);

        rotation_mat = glm::rotate(glm::mat4x4(), pitch, glm::vec3(1,0,0));
        glm::fvec3 y_rotation_axis(glm::rotateY(glm::fvec3(0.f,1.f,0.f), -pitch));
        rotation_mat = glm::rotate( rotation_mat, yaw, y_rotation_axis );
    }
};

struct CameraTransformationMatrices{
    CameraOrientation rotation;
    glm::mat4x4 translation;
};

struct TransformationMatrices{
    glm::mat4x4 projection;
    CameraTransformationMatrices view;

    //CameraTransformationMatrices view;
    //CameraTransformationMatrices tmp_camera;
};


class ViewManager {
public:
    ViewManager();
    glm::mat4x4 getViewMatrix() const; // Model View Matrix
    glm::mat4x4 getProjMtx() const; // Projection Matrix
    //glm::mat4x4 getTmpCameraMtx() const; // Projection Matrix

    void sideStep(float p_amount);
    void forward(float p_amount);
    void up(float p_amount);
    void rotate(float pitch, float yaw);
    void reset_camera();

private:
    void translateCamera(float p_x, float p_y, float p_z);
    void apply(const glm::mat4x4 & p_transformation);
    TransformationMatrices m_transformation_matrices;
    CameraOrientation m_camera_orientation;
};

#endif //VIEW_H
