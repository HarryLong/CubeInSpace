#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm_wrapper.h"

class Transform {
public:
    Transform(const glm::mat4x4 & mv, const glm::mat4x4 & proj, float scale = 1.0f) :
        _MV(mv), _Proj(proj), _scale(scale) {}

    const glm::mat4x4 _MV;
    const glm::mat4x4 _Proj;
    const float _scale;
};

#endif
