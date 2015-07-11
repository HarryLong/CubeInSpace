#ifndef GRID_H
#define GRID_H

#include <vector>
#include "../gl_core/glheader.h"
#include "asset.h"
#include "glm/glm.hpp"

/********
 * AXES *
 ********/
class Axes : public Asset {
public:
    Axes();
    ~Axes();
    virtual void render();

protected:
    virtual void initGL();
};

/*********
 * PLANE *
 *********/
class Plane : public Asset {
public:
    Plane();
    ~Plane();
    virtual void render();

protected:
    virtual void initGL();
};

/********
 * GRID *
 ********/
class Grid{
public:
    Grid();
    ~Grid();
    std::vector<Asset*> getAssets();

private:
    Axes * get_axes();
    Plane * get_plane();

    Axes * m_axes;
    Plane * m_plane;
};

#endif // GRID_H
