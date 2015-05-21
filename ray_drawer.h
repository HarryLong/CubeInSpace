#ifndef RAY_DRAWER_H
#define RAY_DRAWER_H

#include "gl_drawable.h"
#include "glm/vec3.hpp"

class RayDrawer : public GlDrawable
{
public:
    RayDrawer();
    bool bindBuffers();
    void add(const glm::vec3 & start_point, const glm::vec3 & end_point);
private:
};

#endif // RAY_DRAWER_H
