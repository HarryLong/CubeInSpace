#include "shader_programs.h"
#include "../gl_core/glheader.h"

/************************************
 * VERTEX & FRAGMENT SHADER PROGRAM *
 ************************************/
ShaderProgram::ShaderProgram(const QString vertex_shader_filename, const QString fragment_shader_filename, QObject * parent) :
    QOpenGLShaderProgram(parent), m_vertex_shader_filename(vertex_shader_filename), m_fragment_shader_filename(fragment_shader_filename)
{

}

ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::compileAndLink()
{
    bool succ (addShaderFromSourceFile(QOpenGLShader::Vertex, m_vertex_shader_filename)); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = addShaderFromSourceFile(QOpenGLShader::Fragment, m_fragment_shader_filename); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = link();
    if (!succ) qWarning() << "Shader compile log:" << log();
}

/**************************
 * COMPUTE SHADER PROGRAM *
 **************************/
ComputeShaderProgram::ComputeShaderProgram(const QString compute_shader_filename, QObject * parent) :
    QOpenGLShaderProgram(parent), m_compute_shader_filename(compute_shader_filename)
{

}

ComputeShaderProgram::~ComputeShaderProgram()
{

}

void ComputeShaderProgram::compileAndLink()
{
    bool succ (addShaderFromSourceFile(QOpenGLShader::Compute, m_compute_shader_filename)); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = link();
    if (!succ) qWarning() << "Shader compile log:" << log();
}

/***************
 * BASE SHADER *
 ***************/
BaseShader::BaseShader(QObject * parent) : ShaderProgram(":/base.vert", ":/base.frag", parent)
{

}

BaseShader::~BaseShader()
{

}

/******************
 * TERRAIN SHADER *
 ******************/
TerrainShader::TerrainShader(QObject * parent) : ShaderProgram(":/terrain.vert", ":/terrain.frag", parent)
{

}

TerrainShader::~TerrainShader()
{

}

/***************************
 * TERRAIN ELEMENTS SHADER *
 ***************************/
TerrainElementsShader::TerrainElementsShader(QObject * parent) : ShaderProgram(":/terrain_elements.vert", ":/terrain_elements.frag", parent)
{

}

TerrainElementsShader::~TerrainElementsShader()
{

}

/****************************
 * NORMALS GENERATOR SHADER *
 ****************************/
NormalsGeneratorShader::NormalsGeneratorShader(QObject * parent) : ShaderProgram(":/normals_generator.vert", ":/normals_generator.frag", parent)
{

}

NormalsGeneratorShader::~NormalsGeneratorShader()
{

}

/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
const int WaterFluxGeneratorShader::_GROUP_SIZE_X = 32;
const int WaterFluxGeneratorShader::_GROUP_SIZE_Y = 32;
const int WaterFluxGeneratorShader::_GROUP_SIZE_Z = 1;
WaterFluxGeneratorShader::WaterFluxGeneratorShader(QObject * parent) : ComputeShaderProgram(":/water_flux.comp")
{

}

WaterFluxGeneratorShader::~WaterFluxGeneratorShader()
{

}
