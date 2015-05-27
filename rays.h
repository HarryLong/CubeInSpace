#ifndef RAY_DRAWER_H
#define RAY_DRAWER_H

#include "asset.h"
#include "glm/vec3.hpp"

class RayDrawer : public Asset
{
public:
    RayDrawer();
    ~RayDrawer();
    virtual bool bindBuffers();
    virtual void render() const;

    void add(const glm::vec3 & start_point, const glm::vec3 & end_point);
private:
};

#endif // RAY_DRAWER_H
