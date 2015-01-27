#ifndef GRID_MANAGER_H
#define GRID_MANAGER_H

#include <vector>
#include "glheader.h"
#include "gl_drawable.h"
#include "glm/glm.hpp"

#define BASE_GRID_COLOR .2f, .2f, .2f, 1.f
#define POSITIVE_AXIS_COLOR .8f, .8f, .8f, 1.f
#define NEGATIVE_AXIS_COLOR .8f, .8f, .8f, 1.f

class GridHolder : public GlDrawable{
public:
    GridHolder();
    void genGrid();
    bool bindBuffers();

private:
    void insertGridColor(std::vector<GLfloat> & p_vector, float p_red, float p_green, float p_blue, float p_alpha);
};

#endif //GRID_MANAGER_H
