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

    void renderTerrain(Terrain & terrain,
                       TerrainWaterHeightmap & water_heightmap,
                       bool render_water,
                       const Transform & transforms,
                       const LightProperties & sunlight_properties,
                       GLuint overlay_texture_id,
                       bool overlay_active);

//    void renderOverlay(Terrain & terrain, ResourceWrapper & resources, const Transform & transforms,
//                       const char * overlay, int month);

    void renderTerrainElements(Terrain & padded_terrain, Transform & transforms, const std::vector<Asset*> & p_assets);

    void renderAssets(const Transform & transforms, const std::vector<Asset*> & p_assets);

    void createOverlayTexture(GLuint overlay_texture_id, Terrain & terrain, ResourceWrapper & resources, const char * active_overlay, int month);

    void calculateSoilHumidityAndStandingWater(GLuint soil_infiltration_texture_id,
                                               GLuint resulting_soil_humidity_texture_id,
                                               GLuint resulting_standing_water_texture_id,
                                               int rainfall,
                                               int rainfall_intensity,
                                               int terrain_width,
                                               int terrain_depth,
                                               float terrain_scale);

    void calculateWeightedSoilHumidity(SoilHumidity & soil_humdities,
                                       WeightedSoilHumidity & weighted_soil_humidities);

    void balanceWater(PaddedTerrain & terrain, TerrainWaterHeightmap * terrain_water, float terrain_scale,
                      bool one_step = false);

    void slopeBasedInfiltrationRateFilter(Terrain & terrain,
                                          GLuint soil_infiltration_texture_id,
                                          int min_slope);

    void setAbsoluteAggregateHeight(Terrain & terrain,
                                    TerrainWaterHeightmap & terrain_water,
                                    float height);

private:
    void reset_overlays(OverlayTextureCreatorShader & shader);
    glm::uvec3 calculateGroupCount(int n_threads_x, int n_threads_y, int n_threads_z,
                                   int group_size_x, int group_size_y, int group_size_z);
    CounterTexture m_water_comparator_counter;
    TerrainWaterHeightmap m_terrain_water_cache;
    TerrainWaterHeightmap m_horizontal_overlaps;
    TerrainWaterHeightmap m_vertical_overlaps;
    TerrainWaterHeightmap m_corner_overlaps;
    Shaders m_shaders;
};

#endif // RENDERER_H
