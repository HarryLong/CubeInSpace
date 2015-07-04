#ifndef GLM_WRAPPER_H
#define GLM_WRAPPER_H

#define GLM_FORCE_RADIANS

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <Eigen/Geometry>


using Eigen::Vector3f;

class GLM_UTILS {
public:
    static glm::mat4x4 fromEigen(const Eigen::Affine3f & eigen_matrix);
};

#endif // GLM_WRAPPER_H
