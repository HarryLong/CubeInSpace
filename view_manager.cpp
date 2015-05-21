#include "view_manager.h"
#include <glm/gtc/matrix_transform.hpp>

//tmp
#include "utils/utils.h"
#include "constants.h"
#include <math.h>

ViewManager::ViewManager(int z_movement_sensitivity, int x_y_movement_sensitivity, int camera_sensitivity) : m_camera_sensitivity(camera_sensitivity),
    m_z_movement_sensitivity(z_movement_sensitivity), m_x_y_movement_sensitivity(x_y_movement_sensitivity)
{
    m_transformation_matrices.projection = glm::frustum(LEFT, RIGHT, BOTTOM, TOP, Z_NEAR, Z_FAR);

    //m_transformation_matrices.view.translation = glm::mat4x4();

    //TMP
    translate_camera(0,500,5);
   // rotate(90,0);
//    m_transformation_matrices.tmp_camera = m_transformation_matrices.view;

//    m_transformation_matrices.view.rotation = glm::mat4x4();
//    m_transformation_matrices.view.translation = glm::mat4x4();
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
}

glm::vec3 ViewManager::toWorld(const glm::vec3 & camera_position, const GLint* viewport)
{
    int width = viewport[2];
    int height = viewport[3];

    // unproject screen point to derive world coordinates
    int realx = camera_position[0] ;
    int realy = height - camera_position[1] - 1;
    int realz = camera_position[2] ;
    glm::vec3 window_pos = glm::vec3((float) realx, (float) realy, (float) realz); // Actual window position

    glm::vec3 world_pos = glm::unProject(window_pos, getViewMatrix(), getProjMtx(),
                                         glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));
    return glm::vec3(world_pos.x, world_pos.y, world_pos.z);
}

void ViewManager::setNavigationProperties(int z_movement_sensitivity, int x_y_movement_sensitivity, int camera_sensitivity)
{
    this->m_camera_sensitivity = camera_sensitivity;
    this->m_z_movement_sensitivity = z_movement_sensitivity;
    this->m_x_y_movement_sensitivity = x_y_movement_sensitivity;
}

void ViewManager::pushTransforms()
{
    m_cached_transformation_matrices = m_transformation_matrices;
}

void ViewManager::popTransforms()
{
    m_transformation_matrices = m_cached_transformation_matrices;
}
