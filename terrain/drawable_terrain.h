#ifndef DRAWABLE_TERRAIN_H
#define DRAWABLE_TERRAIN_H

#include "../gl_core/gl_drawable.h"
#include "../gl_texture/texture_element_2d.h"
#include "terragen_file_manager/terragen_file.h"

class DrawableTerrain : public GlDrawable, public TextureElement2D<GLfloat>
{
public:
    DrawableTerrain();
    ~DrawableTerrain();

    virtual void render();
    bool prepareTerrainGeometry(const TerragenFile & terragen_file);

protected:
    virtual void initGL();

private:
    void refreshHeightmapTexture(const TerragenFile & terragen_file);
};

#endif // DRAWABLE_TERRAIN_H
