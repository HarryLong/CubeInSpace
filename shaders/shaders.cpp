#include "shaders.h"
#include <QObject>
#include <QOpenGLShaderProgram>

Shaders::Shaders()
{

}

Shaders::~Shaders()
{

}

void Shaders::compileAndLink()
{
    m_base.compileAndLink();
    m_terrain.compileAndLink();
    m_overlay.compileAndLink();
    m_terrain_elements.compileAndLink();
    m_normals_generator.compileAndLink();
    m_water_flux_generator.compileAndLink();
    m_water_comparator.compileAndLink();
    m_soil_humidity_calculator.compileAndLink();
    m_slope_based_soil_infiltration_calculator.compileAndLink();
    m_overlay_texture_creator.compileAndLink();
    m_aggregate_height.compileAndLink();
    m_border_overlap_reducer.compileAndLink();
}
