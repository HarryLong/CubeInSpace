#ifndef COMPUTER_H
#define COMPUTER_H

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
#include "shaders/compute_shaders.h"
#include "shaders/uniforms.h"
#include "../gl_texture/counter_texture.h"
#include "resources/resource_wrapper.h"
#include "../clustering/cluster.h"
#include "../clustering/cluster_membership_texture.h"

class Computer {
public:
    class CachedWaterTexture : public TextureElement2D<GLfloat> {
    public:
        CachedWaterTexture();
        ~CachedWaterTexture();
    };
    class WaterHeightmapBorderReductionTexture : public TextureElement2D<GLfloat> {
    public:
        WaterHeightmapBorderReductionTexture();
        ~WaterHeightmapBorderReductionTexture();
    };
    class SlopeAndHumidityClusterReductionTexture : public TextureElement2DArray<GLfloat> {
    public:
        SlopeAndHumidityClusterReductionTexture();
        ~SlopeAndHumidityClusterReductionTexture();
    };
    class TemperatureClusterReductionTexture : public TextureElement2DArray<GLfloat> {
    public:
        TemperatureClusterReductionTexture();
        ~TemperatureClusterReductionTexture();
    };
    class DailyIlluminationClusterReductionTexture : public TextureElement2DArray<GLfloat> {
    public:
        DailyIlluminationClusterReductionTexture();
        ~DailyIlluminationClusterReductionTexture();
    };

    Computer();
    ~Computer();
    void compileAndLinkShaders();

    void createOverlayTexture(GLuint overlay_texture_id,
                              Terrain & terrain,
                              ResourceWrapper & resources,
                              ClusterMembershipTexture & cluster_membership_texture,
                              const char * active_overlay,
                              int month);

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

    GLuint balanceWater(PaddedTerrain & terrain,
                      TerrainWater & terrain_water,
                      int month);

    void slopeBasedInfiltrationRateFilter(Slope & terrain_slope,
                                          SoilInfiltration & soil_infiltration,
                                          int min_slope);

    void setAbsoluteAggregateHeight(Terrain & terrain,
                                    GLuint terrain_water_texture_id,
                                    float height);

    void convertNormalsToSlope(TerrainNormals & terrain_normals, Slope & slope);

    void kMeansCluster(Clusters & clusters, ResourceWrapper & resources, ClusterMembershipTexture & cluster_memberships, int clustering_iterations);

private:
    ComputeShaderProgram * get_overlay_creator_shader(const char * active_overlay);
    glm::uvec3 calculateGroupCount(int n_threads_x, int n_threads_y, int n_threads_z,
                                   int group_size_x, int group_size_y, int group_size_z);
    CounterTexture m_water_comparator_counter;
    CachedWaterTexture m_terrain_water_cache;
    WaterHeightmapBorderReductionTexture m_horizontal_overlaps;
    WaterHeightmapBorderReductionTexture m_vertical_overlaps;
    WaterHeightmapBorderReductionTexture m_corner_overlaps;
    SlopeAndHumidityClusterReductionTexture m_slope_and_humidity_cluster_reduction;
    TemperatureClusterReductionTexture m_temperature_cluster_reduction;
    DailyIlluminationClusterReductionTexture m_daily_illumination_cluster_reduction;
    ComputeShaders m_shaders;
};

#endif // RENDERER_H
