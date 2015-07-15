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
#include "shaders/shaders.h"
#include "shaders/uniforms.h"
#include "../gl_texture/counter_texture.h"
#include "resources/resource_wrapper.h"

class Renderer {
public:
    Renderer();
    ~Renderer();
    void compileAndLinkShaders();

    void calculateNormals(Terrain & terrain);

    void renderTerrain(Terrain & terrain, ResourceWrapper & resources, const Transform & transforms,
                       const LightProperties & sunlight_properties, const char * overlay, int month);

    void renderTerrainElements(Terrain & padded_terrain, Transform & transforms, const std::vector<Asset*> & p_assets);

    void renderAssets(const Transform & transforms, const std::vector<Asset*> & p_assets);

    void balanceWater(PaddedTerrain & terrain, TerrainWaterHeightmap * terrain_water, bool one_step = false);

private:
    void reset_overlays(TerrainShader & shader);

    CounterTexture m_water_comparator_counter;
    TerrainWaterHeightmap m_terrain_water_cache;
    Shaders m_shaders;
};

#endif // RENDERER_H
