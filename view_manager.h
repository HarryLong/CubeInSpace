#ifndef VIEW_H
#define VIEW_H

#define GLM_FORCE_RADIANS

#include "glheader.h"
#include "glm/matrix.hpp"
#include "controllers.h"

enum TransformType{
    Translation,
    Rotation
};

class CameraOrientation
{
public:
    CameraOrientation();
    void calculate_rotation_matrix();

    float pitch; // Around the x axis
    float yaw; // Around the y axis
    glm::mat4x4 rotation_mat;
};

struct CameraTransformationMatrices{
    CameraOrientation rotation;
    glm::mat4x4 translation;
};

struct TransformationMatrices{
    glm::mat4x4 projection;
    CameraTransformationMatrices view;
};

/****************
 * VIEW MANAGER *
 ****************/
class ViewManager : public QObject{
    Q_OBJECT
public:
    ViewManager(ViewControllers & view_controllers);
    glm::mat4x4 getViewMatrix() const; // Model View Matrix
    glm::mat4x4 getProjMtx() const; // Projection Matrix

    void sideStep(float p_amount, bool ignore_sensitivity = false);
    void forward(float p_amount, bool ignore_sensitivity = false);
    void up(float p_amount, bool ignore_sensitivity = false);
    void rotate(float pitch, float yaw, bool ignore_sensitivity = false);
    void reset_camera();
    glm::vec3 toWorld(const glm::vec3 & camera_position, const GLint * viewport);

    void pushTransforms();
    void popTransforms();

    float getCameraYaw();
    float getCameraPitch();

signals:
    void cameraOrientationChanged(float pitch, float yaw);

private slots:
    void set_camera_sensitivity(int camera_sensitivity);
    void set_z_movements_sensitivity(int z_movement_sensitivity);
    void set_x_y_movements_sensitivity(int x_y_movement_sensitivity);

private:
    void translate_camera(float p_x, float p_y, float p_z);
    void emit_camera_orientation_changed_signal();

    TransformationMatrices m_transformation_matrices;
    TransformationMatrices m_cached_transformation_matrices;
    int m_z_movement_sensitivity, m_x_y_movement_sensitivity, m_camera_sensitivity;
};

#endif //VIEW_H
