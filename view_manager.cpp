#include "view_manager.h"
#include <glm/gtc/matrix_transform.hpp>

//tmp
#include "utils/utils.h"
#include "constants.h"
#include <math.h>

ViewManager::ViewManager()
{
    m_transformation_matrices.projection = glm::frustum(LEFT, RIGHT, BOTTOM, TOP, Z_NEAR, Z_FAR);

    //m_transformation_matrices.view.translation = glm::mat4x4();

    //TMP
    translateCamera(0,500,5);
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

//glm::mat4x4 ViewManager::getTmpCameraMtx() const
//{
 //   return m_transformation_matrices.tmp_camera.rotation * m_transformation_matrices.tmp_camera.translation ;
//}

void ViewManager::sideStep(float p_amount)
{
    translateCamera(-p_amount, 0, 0);
}

void ViewManager::up(float p_amount)
{
    translateCamera(0, p_amount, 0);
}

void ViewManager::forward(float p_amount)
{
    translateCamera(0, 0, -p_amount);
}

//void ViewManager::transform(float p_diff_x, float p_diff_y, float p_diff_z, bool rotation)
//{
//    if(rotation) // i.e. rotation
//    {
//        rotate(-p_diff_y, -p_diff_x);
//    }
//    else // i.e translation
//    {
//        translateCamera(-p_diff_x, p_diff_y, p_diff_z);
//    }
//}

void ViewManager::translateCamera(float p_x, float p_y, float p_z)
{
    glm::vec3 rotated_translation_vector;
    {
        glm::vec4 translation_4_point_vector = glm::vec4(-p_x, -p_y, -p_z,0);
        glm::vec4 rotated_4_point_translation_vector = (translation_4_point_vector * m_transformation_matrices.view.rotation.rotation_mat);
        rotated_translation_vector = glm::vec3(rotated_4_point_translation_vector[0],rotated_4_point_translation_vector[1],rotated_4_point_translation_vector[2]);
    }

    m_transformation_matrices.view.translation = glm::translate(m_transformation_matrices.view.translation, rotated_translation_vector);
}

void ViewManager::rotate(float pitch, float yaw)
{
    m_transformation_matrices.view.rotation.pitch += pitch;
    m_transformation_matrices.view.rotation.yaw += yaw;

    m_transformation_matrices.view.rotation.calculate_rotation_matrix();
}

void ViewManager::apply(const glm::mat4x4 & p_transformation)
{
    //m_transformation_matrices.view *= glm::inverse(m_transformation_matrices.view) * p_transformation * m_transformation_matrices.view;
}

void ViewManager::reset_camera()
{
    m_transformation_matrices.view.rotation.pitch = 0.f;
    m_transformation_matrices.view.rotation.yaw = 0.f;

    m_transformation_matrices.view.rotation.rotation_mat = glm::mat4x4();
    m_transformation_matrices.view.translation = glm::mat4x4();
}
