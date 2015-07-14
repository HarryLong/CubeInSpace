#ifndef PADDED_TERRAIN_H
#define PADDED_TERRAIN_H

#include "../gl_texture/texture_element.h"
#include "terrain.h"

class PaddedTerrain : public TextureElement<GLfloat> {
public:
    PaddedTerrain();
    ~PaddedTerrain();

    void refresh(Terrain & terrain);

private:

};

#endif // PADDED_TERRAIN_H
