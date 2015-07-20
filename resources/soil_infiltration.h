#ifndef SOIL_INFILTRATION_H
#define SOIL_INFILTRATION_H

#include "../gl_texture/texture_element.h"
#include "glm/common.hpp"

class SoilInfiltration : public TextureElement<GLuint> // TODO: And GL Drawable to actually draw the water bruuuu
{
public:
    SoilInfiltration();
    ~SoilInfiltration();

    void update(const glm::vec2 & center, int radius, int terrain_width, int terrain_depth, int infiltration_rate);
};

#endif // SOIL_INFILTRATION_H
