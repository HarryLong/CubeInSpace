#ifndef RAY_DRAWER_H
#define RAY_DRAWER_H

#include "asset.h"
#include "glm/vec3.hpp"

class RayDrawer : public Asset
{
public:
    RayDrawer();
    ~RayDrawer();
    virtual void render();

    void add(const glm::vec3 & start_point, const glm::vec3 & end_point);

protected:
    virtual void initGL();

private:
};

#endif // RAY_DRAWER_H
