#ifndef GRID_H
#define GRID_H

#include <vector>
#include "glheader.h"
#include "asset.h"
#include "glm/glm.hpp"

/********
 * AXES *
 ********/
class Axes : public Asset {
public:
    Axes();
    ~Axes();
    virtual bool bindBuffers();
    virtual void render() const;

private:
    void init();
};

/*********
 * PLANE *
 *********/
class Plane : public Asset {
public:
    Plane();
    ~Plane();
    virtual bool bindBuffers();
    virtual void render() const;

private:
    void init();
};

/********
 * GRID *
 ********/
class Grid{
public:
    Grid();
    ~Grid();
    std::vector<const Asset*> getAssets();

private:
    Axes * get_axes();
    Plane * get_plane();

    Axes * m_axes;
    Plane * m_plane;
};

#endif // GRID_H
