#ifndef TERRAIN_SHADE_H
#define TERRAIN_SHADE_H

#include "texture_element.h"

/*****************
 * TERRAIN SHADE *
 *****************/
class TerrainShade : public TextureElement<GLubyte> {
public:
    TerrainShade();
    ~TerrainShade();
};

#endif // TERRAIN_SHADE_H
