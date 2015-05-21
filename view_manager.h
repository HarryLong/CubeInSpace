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
};


class ViewManager {
public:
    ViewManager(int z_movement_sensitivity, int x_y_movement_sensitivity, int camera_sensitivity);
    glm::mat4x4 getViewMatrix() const; // Model View Matrix
    glm::mat4x4 getProjMtx() const; // Projection Matrix

    void sideStep(float p_amount, bool ignore_sensitivity = false);
    void forward(float p_amount, bool ignore_sensitivity = false);
    void up(float p_amount, bool ignore_sensitivity = false);
    void rotate(float pitch, float yaw, bool ignore_sensitivity = false);
    void reset_camera();
    void setNavigationProperties(int z_movement_sensitivity, int x_y_movement_sensitivity, int camera_sensitivity);
    glm::vec3 toWorld(const glm::vec3 & camera_position, const GLint * viewport);

    void pushTransforms();
    void popTransforms();

private:
    void translate_camera(float p_x, float p_y, float p_z);
    TransformationMatrices m_transformation_matrices;
    TransformationMatrices m_cached_transformation_matrices;
    int m_z_movement_sensitivity, m_x_y_movement_sensitivity, m_camera_sensitivity;
};

#endif //VIEW_H
