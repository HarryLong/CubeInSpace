#ifndef SHADERS_H
#define SHADERS_H

#include "shader_programs.h"

class QObject;
class Shaders {
public:
    Shaders();
    ~Shaders();
    void compileAndLink();

    BaseShader m_base;
    TerrainShader m_terrain;
    OverlayShader m_overlay;
    TerrainElementsShader m_terrain_elements;
    NormalsGeneratorShader m_normals_generator;
    WaterFluxGeneratorShader m_water_flux_generator;
    WaterComparatorShader m_water_comparator;
    SoilHumidityCalculatorShader m_soil_humidity_calculator;
    SlopeBasedSoilInfiltrationShader m_slope_based_soil_infiltration_calculator;
};

#endif // SHADERS_H
