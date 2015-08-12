#include "rendering_shader_programs.h"
#include "../gl_core/glheader.h"

/************************************
 * VERTEX & FRAGMENT SHADER PROGRAM *
 ************************************/
RenderingShaderProgram::RenderingShaderProgram(const QString vertex_shader_filename, const QString fragment_shader_filename) :
    m_vertex_shader_filename(vertex_shader_filename), m_fragment_shader_filename(fragment_shader_filename)
{

}

RenderingShaderProgram::~RenderingShaderProgram()
{

}

void RenderingShaderProgram::compileAndLink()
{
    bool succ (addShaderFromSourceFile(QOpenGLShader::Vertex, m_vertex_shader_filename)); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = addShaderFromSourceFile(QOpenGLShader::Fragment, m_fragment_shader_filename); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = link();
    if (!succ) qWarning() << "Shader compile log:" << log();
}

/***************
 * BASE SHADER *
 ***************/
BaseShader::BaseShader() : RenderingShaderProgram(":/base.vert", ":/base.frag")
{

}

BaseShader::~BaseShader()
{

}

/******************
 * TERRAIN SHADER *
 ******************/
TerrainShader::TerrainShader() : RenderingShaderProgram(":/terrain.vert", ":/terrain.frag")
{

}

TerrainShader::~TerrainShader()
{

}

/***************************
 * TERRAIN ELEMENTS SHADER *
 ***************************/
TerrainElementsShader::TerrainElementsShader() : RenderingShaderProgram(":/terrain_elements.vert", ":/terrain_elements.frag")
{

}

TerrainElementsShader::~TerrainElementsShader()
{

}

/****************************
 * NORMALS GENERATOR SHADER *
 ****************************/
NormalsGeneratorShader::NormalsGeneratorShader() : RenderingShaderProgram(":/normals_generator.vert", ":/normals_generator.frag")
{

}

NormalsGeneratorShader::~NormalsGeneratorShader()
{

}
