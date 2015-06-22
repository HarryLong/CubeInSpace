#include "shader_programs.h"
#include "glheader.h"

ShaderProgram::ShaderProgram(const QString & vertex_shader_filename, const QString & fragment_shader_filename,
                             QObject * parent) : QGLShaderProgram(parent)
{
    bool succ (addShaderFromSourceFile(QGLShader::Vertex, vertex_shader_filename)); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = addShaderFromSourceFile(QGLShader::Fragment, fragment_shader_filename);
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = link();
    if (!succ) qWarning() << "Shader compile log:" << log();
}

ShaderProgram::~ShaderProgram()
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
