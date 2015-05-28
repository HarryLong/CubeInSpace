#ifndef GLM_UTILS
#define GLM_UTILS

#include "glm/glm.hpp"

#include <iostream>
namespace glmutils{
    template <typename T> inline glm::vec3 toVec3(const T & vec)
    {
        return glm::vec3(vec[0], vec[1], vec[2]);
    }
}

#endif // GLM_UTILS
