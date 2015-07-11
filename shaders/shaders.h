#ifndef SHADERS_H
#define SHADERS_H

#include "shader_programs.h"

class QObject;
class Shaders {
public:
    Shaders(QObject * parent);
    ~Shaders();
    void compileAndLink();

    BaseShader m_base;
    TerrainShader m_terrain;
    TerrainElementsShader m_terrain_elements;
    NormalsGeneratorShader m_normals_generator;
    WaterFluxGeneratorShader m_water_flux_generator;
};

#endif // SHADERS_H
