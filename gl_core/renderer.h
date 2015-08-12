#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <map>
#include <vector>
#include <QDir>
#include <QMatrix>
#include "terrain/terrain.h"
#include "terrain/padded_terrain.h"
#include "lighting/light_properties.h"
#include "../gl_assets/grid.h"
#include "transform.h"
#include "shaders/rendering_shaders.h"
#include "shaders/uniforms.h"
#include "../gl_texture/counter_texture.h"
#include "resources/resource_wrapper.h"
#include "../clustering/cluster.h"
#include "../clustering/cluster_membership_texture.h"

class Renderer {
public:
    Renderer();
    ~Renderer();
    void compileAndLinkShaders();

    void calculateNormals(Terrain & terrain);

    void renderTerrain(Terrain & terrain,
                       GLuint water_heightmap_texture_id,
                       bool render_water,
                       const Transform & transforms,
                       const LightProperties & sunlight_properties,
                       GLuint overlay_texture_id,
                       bool overlay_active);

    void renderTerrainElements(Terrain & padded_terrain, Transform & transforms, const std::vector<Asset*> & p_assets);

    void renderAssets(const Transform & transforms, const std::vector<Asset*> & p_assets);

private:
    RenderingShaders m_shaders;
};

#endif // RENDERER_H
