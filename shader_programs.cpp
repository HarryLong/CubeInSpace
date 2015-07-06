#include "shader_programs.h"
#include "glheader.h"

/************************************
 * VERTEX & FRAGMENT SHADER PROGRAM *
 ************************************/
ShaderProgram::ShaderProgram(const QString & vertex_shader_filename, const QString & fragment_shader_filename,
                             QObject * parent) : QOpenGLShaderProgram(parent)
{
    bool succ (addShaderFromSourceFile(QOpenGLShader::Vertex, vertex_shader_filename)); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = addShaderFromSourceFile(QOpenGLShader::Fragment, fragment_shader_filename); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = link();
    if (!succ) qWarning() << "Shader compile log:" << log();
}

ShaderProgram::~ShaderProgram()
{

}

/**************************
 * COMPUTE SHADER PROGRAM *
 **************************/
ComputeShaderProgram::ComputeShaderProgram(const QString & computer_shader_filename, QObject * parent) :  QOpenGLShaderProgram(parent)
{
    bool succ (addShaderFromSourceFile(QOpenGLShader::Compute, computer_shader_filename)); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = link();
    if (!succ) qWarning() << "Shader compile log:" << log();
}

ComputeShaderProgram::~ComputeShaderProgram()
{

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
const int WaterFluxGeneratorShader::_GROUP_SIZE_X = 16;
const int WaterFluxGeneratorShader::_GROUP_SIZE_Y = 16;
const int WaterFluxGeneratorShader::_GROUP_SIZE_Z = 1;
WaterFluxGeneratorShader::WaterFluxGeneratorShader(QObject * parent) : ComputeShaderProgram(":/water_flux.comp")
{

}

WaterFluxGeneratorShader::~WaterFluxGeneratorShader()
{

}
