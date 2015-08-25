#ifndef COMPUTE_SHADERS_H
#define COMPUTE_SHADERS_H

#include "compute_shader_programs.h"

class QObject;
class ComputeShaders {
public:
    struct OverlayCreatorShaders{
        OverlayTextureCreatorShaders::Slope m_slope;
        OverlayTextureCreatorShaders::Altitude m_altitude;
        OverlayTextureCreatorShaders::Shade m_shade;
        OverlayTextureCreatorShaders::Temperature m_temperature;
        OverlayTextureCreatorShaders::DailyIllumination m_daily_illumination;
        OverlayTextureCreatorShaders::SoilInfiltrationRate m_soil_infiltration_rate;
        OverlayTextureCreatorShaders::MonthlySoilHumidity m_soil_humidity;
        OverlayTextureCreatorShaders::WeightedAvgSoilHumidity m_weigted_avg_soil_humidity;
        OverlayTextureCreatorShaders::Clusters m_clusters;
    };
    struct KMeansClusteringShaders{
        KMeansClustererShaders::ClosestClusterFinder m_closest_cluster_finder;
        KMeansClustererShaders::MeansRecalculator m_means_recalculator;
    };

    ComputeShaders();
    ~ComputeShaders();
    void compileAndLink();

    WaterFluxGeneratorShader m_water_flux_generator;
    WaterComparatorShader m_water_comparator;
    SoilHumidityCalculatorShader m_soil_humidity_calculator;
    StandingWaterCalculator m_standing_water_calculator;
    SlopeBasedSoilInfiltrationShader m_slope_based_soil_infiltration_calculator;
    AggregateHeightShader m_aggregate_height;
    BorderOverlapReducer m_border_overlap_reducer;
    WeightedAverageCalculator m_weighted_avg_calculator;
    NormalsToSlopeConverter m_normals_to_slope_converter;
    KMeansClusteringShaders m_k_means_clusterer;
    OverlayCreatorShaders m_overlay_creator_shaders;
};

#endif // COMPUTE_SHADERS_H
