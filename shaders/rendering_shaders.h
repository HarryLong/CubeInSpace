#ifndef RENDERING_SHADERS_H
#define RENDERING_SHADERS_H

#include "rendering_shader_programs.h"

class QObject;
class RenderingShaders {
public:
    RenderingShaders();
    ~RenderingShaders();
    void compileAndLink();

    BaseShader m_base;
    TerrainShader m_terrain;
    TerrainElementsShader m_terrain_elements;
    NormalsGeneratorShader m_normals_generator;
};

#endif // RENDERING_SHADERS_H
