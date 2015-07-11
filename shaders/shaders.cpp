#include "shaders.h"
#include <QObject>
#include <QOpenGLShaderProgram>

Shaders::Shaders(QObject * parent) :
    m_base(parent),
    m_terrain(parent),
    m_terrain_elements(parent),
    m_normals_generator(parent),
    m_water_flux_generator(parent)
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
}
