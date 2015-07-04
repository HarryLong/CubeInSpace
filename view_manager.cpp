#include "view_manager.h"

#include "constants.h"
#include <math.h>
#include "geom.h"
#include "glm_wrapper.h"
#include <QSlider>

/**********************
 * CAMERA ORIENTATION *
 **********************/
CameraOrientation::CameraOrientation() : pitch(.0f), yaw(.0f)
{

}

void CameraOrientation::calculate_rotation_matrix()
{
    Geom::normalizeDegrees(pitch);
    Geom::normalizeDegrees(yaw);

    float pitch_radians(Geom::toRadians(pitch));
    float yaw_radians(Geom::toRadians(yaw));

    rotation_mat = glm::rotate(glm::mat4x4(), pitch_radians, glm::vec3(1,0,0));
    glm::fvec3 y_rotation_axis(glm::rotateY(glm::fvec3(0.f,1.f,0.f), -pitch_radians));
    rotation_mat = glm::rotate( rotation_mat, yaw_radians, y_rotation_axis );
}

//-------------------------------------------

/****************
 * VIEW MANAGER *
 ****************/
ViewManager::ViewManager(ViewControllers & view_controllers) : m_camera_sensitivity(view_controllers.camera_sensitivity_slider->value()),
    m_z_movement_sensitivity(view_controllers.z_movement_sensitivity_slider->value()),
    m_x_y_movement_sensitivity(view_controllers.x_y_movement_sensitivity_slider->value())
{
//    m_transformation_matrices.projection = glm::frustum(LEFT, RIGHT, BOTTOM, TOP, Z_NEAR, Z_FAR);

    connect(view_controllers.camera_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(set_camera_sensitivity(int)));
    connect(view_controllers.x_y_movement_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(set_x_y_movements_sensitivity(int)));
    connect(view_controllers.z_movement_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(set_z_movements_sensitivity(int)));

    //TMP
    translate_camera(0,500,5);
}

glm::mat4x4 ViewManager::getViewMatrix() const
{
    // Calculate the rotation matrix based on the individual rotations
    return  m_transformation_matrices.view.rotation.rotation_mat * m_transformation_matrices.view.translation;
}

glm::mat4x4 ViewManager::getProjMtx() const
{
    return m_transformation_matrices.projection;
}

void ViewManager::sideStep(float p_amount, bool ignore_sensitivity)
{
    translate_camera((ignore_sensitivity ? 1 : m_x_y_movement_sensitivity) * p_amount, 0, 0);
}

void ViewManager::up(float p_amount, bool ignore_sensitivity)
{
    translate_camera(0, (ignore_sensitivity ? 1 : m_x_y_movement_sensitivity) * p_amount, 0);
}

void ViewManager::forward(float p_amount, bool ignore_sensitivity)
{
    translate_camera(0, 0, (ignore_sensitivity ? 1 : m_z_movement_sensitivity) * p_amount);
}

void ViewManager::rotate(float pitch, float yaw, bool ignore_sensitivity)
{
    m_transformation_matrices.view.rotation.pitch += (ignore_sensitivity ? 1 : m_camera_sensitivity) * pitch;
    m_transformation_matrices.view.rotation.yaw += (ignore_sensitivity ? 1 : m_camera_sensitivity) * yaw;

    m_transformation_matrices.view.rotation.calculate_rotation_matrix();

    emit_camera_orientation_changed_signal();
}

void ViewManager::translate_camera(float p_x, float p_y, float p_z)
{
    glm::vec3 rotated_translation_vector;
    {
        glm::vec4 translation_4_point_vector = glm::vec4(p_x, -p_y, -p_z,0);
        glm::vec4 rotated_4_point_translation_vector = (translation_4_point_vector * m_transformation_matrices.view.rotation.rotation_mat);
        rotated_translation_vector = glm::vec3(rotated_4_point_translation_vector[0],rotated_4_point_translation_vector[1],rotated_4_point_translation_vector[2]);
    }

    m_transformation_matrices.view.translation = glm::translate(m_transformation_matrices.view.translation, rotated_translation_vector);
}

void ViewManager::reset_camera()
{
    m_transformation_matrices.view.rotation.pitch = 0.f;
    m_transformation_matrices.view.rotation.yaw = 0.f;

    m_transformation_matrices.view.rotation.rotation_mat = glm::mat4x4();
    m_transformation_matrices.view.translation = glm::mat4x4();

    emit_camera_orientation_changed_signal();
}

glm::vec3 ViewManager::toWorld(const glm::vec3 & camera_position, const GLint* viewport, int widget_width, int widget_height)
{
    // unproject screen point to derive world coordinates
    int realx = camera_position[0]-1;

    int realy = widget_height - camera_position[1];
    int realz = camera_position[2] ;

    glm::vec3 window_pos = glm::vec3((float) realx, (float) realy, (float) realz); // Actual window position

    glm::vec3 world_pos = glm::unProject(window_pos, getViewMatrix(), getProjMtx(),
                                         glm::vec4(viewport[0], viewport[1], widget_width, widget_height));
    return glm::vec3(world_pos.x, world_pos.y, world_pos.z);
}

void ViewManager::set_camera_sensitivity(int camera_sensitivity)
{
    m_camera_sensitivity = camera_sensitivity;
}

void ViewManager::set_z_movements_sensitivity(int z_movement_sensitivity)
{
    m_z_movement_sensitivity = z_movement_sensitivity;
}

void ViewManager::set_x_y_movements_sensitivity(int x_y_movement_sensitivity)
{
    m_x_y_movement_sensitivity = x_y_movement_sensitivity;
}

void ViewManager::pushTransforms()
{
    m_cached_transformation_matrices = m_transformation_matrices;
}

void ViewManager::popTransforms()
{
    m_transformation_matrices = m_cached_transformation_matrices;
}

void ViewManager::emit_camera_orientation_changed_signal()
{
    emit cameraOrientationChanged(m_transformation_matrices.view.rotation.pitch, m_transformation_matrices.view.rotation.yaw);
}

float ViewManager::getCameraYaw()
{
    return m_transformation_matrices.view.rotation.pitch;
}

float ViewManager::getCameraPitch()
{
    return m_transformation_matrices.view.rotation.yaw;
}

