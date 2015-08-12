#include "rendering_shaders.h"
#include <QObject>
#include <QOpenGLShaderProgram>

RenderingShaders::RenderingShaders()
{

}

RenderingShaders::~RenderingShaders()
{

}

void RenderingShaders::compileAndLink()
{
    m_base.compileAndLink();
    m_terrain.compileAndLink();
    m_terrain_elements.compileAndLink();
    m_normals_generator.compileAndLink();
}
