#ifndef GRID_MANAGER_H
#define GRID_MANAGER_H

#include <vector>
#include "glheader.h"
#include "gl_drawable.h"
#include "glm/glm.hpp"

class GridHolder : public GlDrawable{
public:
    GridHolder();
    void genGrid();
    bool bindBuffers();
    bool binded() { return m_binded;}

private:
    bool m_binded; // Hack: For some reason, it won't work if binded on construction!!!
};

#endif //GRID_MANAGER_H
