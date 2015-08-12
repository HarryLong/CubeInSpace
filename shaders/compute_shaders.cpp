#include "compute_shaders.h"
#include <QObject>
#include <QOpenGLShaderProgram>

ComputeShaders::ComputeShaders()
{

}

ComputeShaders::~ComputeShaders()
{

}

void ComputeShaders::compileAndLink()
{
    m_water_flux_generator.compileAndLink();
    m_water_comparator.compileAndLink();
    m_soil_humidity_calculator.compileAndLink();
    m_slope_based_soil_infiltration_calculator.compileAndLink();
    m_aggregate_height.compileAndLink();
    m_border_overlap_reducer.compileAndLink();
    m_weighted_avg_calculator.compileAndLink();
    m_normals_to_slope_converter.compileAndLink();
    m_closest_cluster_finder.compileAndLink();

    m_overlay_creator_shaders.m_slope.compileAndLink();
    m_overlay_creator_shaders.m_altitude.compileAndLink();
    m_overlay_creator_shaders.m_shade.compileAndLink();
    m_overlay_creator_shaders.m_temperature.compileAndLink();
    m_overlay_creator_shaders.m_daily_illumination.compileAndLink();
    m_overlay_creator_shaders.m_soil_infiltration_rate.compileAndLink();
    m_overlay_creator_shaders.m_soil_humidity.compileAndLink();
    m_overlay_creator_shaders.m_weigted_avg_soil_humidity.compileAndLink();
}
