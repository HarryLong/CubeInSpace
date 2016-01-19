#ifndef TERRAIN_SHADE_H
#define TERRAIN_SHADE_H

#include "../gl_texture/texture_element_2d.h"
#include <glm/common.hpp>

class TerrainNormals;
class Terrain;
/*****************
 * TERRAIN SHADE *
 *****************/
class TerrainShade : public TextureElement2D<GLubyte> {
public:
    TerrainShade();
    ~TerrainShade();

};

#endif // TERRAIN_SHADE_H
