#ifndef TERRAIN_WATER_H
#define TERRAIN_WATER_H

#include <gl_drawable.h>
#include "texture_element.h"
#include "glm/common.hpp"

class TerrainWater : public TextureElement<GLuint> // TODO: And GL Drawable to actually draw the water bruuuu
{
public:
    TerrainWater();
    ~TerrainWater();

    void tmp_calculateTotalWater();

    struct MaterialProperties{
        static const glm::vec4 _DIFFUSE;
        static const glm::vec4 _SPECULAR;
        static const glm::vec4 _AMBIENT;
    };
};

#endif // TERRAIN_WATER_H
