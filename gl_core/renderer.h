#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <map>
#include <vector>
#include <QDir>
#include <QMatrix>
#include "terrain/terrain.h"
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
    void init();

    void calculateNormals(NormalsGeneratorShader & shader, Terrain & terrain);

    void renderTerrain(TerrainShader & shader, Terrain & terrain, ResourceWrapper & resources, const Transform & transforms,
                       const LightProperties & sunlight_properties, const char * overlay, int month);

    void renderTerrainElements(TerrainElementsShader & shader, Terrain & terrain, Transform & transforms, const std::vector<Asset*> & p_assets);

    void renderAssets(BaseShader & shader, const Transform & transforms, const std::vector<Asset*> & p_assets);

    void balanceWater(WaterFluxGeneratorShader & shader, Terrain & terrain, GLuint terrain_water_heightmap_texture_id);

private:
    void reset_overlays(TerrainShader & shader);

    CounterTexture m_counter_texture;
};

#endif // RENDERER_H
