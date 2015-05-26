#ifndef GRID_MANAGER_H
#define GRID_MANAGER_H

#include <vector>
#include "glheader.h"
#include "asset.h"
#include "glm/glm.hpp"

class Grid : public Asset{
public:
    Grid();
    void init();
    bool bindBuffers();
    void render() const;
};
#endif //GRID_MANAGER_H
