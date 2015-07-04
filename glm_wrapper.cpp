#include "glm_wrapper.h"

glm::mat4x4 GLM_UTILS::fromEigen(const Eigen::Affine3f & eigen_matrix)
{
    const float * raw_mat = eigen_matrix.data();
//    return glm::mat4x4(
//            raw_mat[0], raw_mat[4], raw_mat[8] , raw_mat[12],
//            raw_mat[1], raw_mat[5], raw_mat[9] , raw_mat[13],
//            raw_mat[2], raw_mat[6], raw_mat[10], raw_mat[14],
//            raw_mat[3], raw_mat[7], raw_mat[11], raw_mat[15]);

    return glm::mat4x4(
            raw_mat[0], raw_mat[1], raw_mat[2] , raw_mat[3],
            raw_mat[4], raw_mat[5], raw_mat[6] , raw_mat[7],
            raw_mat[8], raw_mat[9], raw_mat[10], raw_mat[11],
            raw_mat[12], raw_mat[13], raw_mat[14], raw_mat[15]);
}
