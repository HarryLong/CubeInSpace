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
    m_terrain_elements.compileAndLink();
    m_normals_generator.compileAndLink();
    m_water_flux_generator.compileAndLink();
    m_water_comparator.compileAndLink();
}
