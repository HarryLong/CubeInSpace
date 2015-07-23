#include "shader_programs.h"
#include "../gl_core/glheader.h"

/************************************
 * VERTEX & FRAGMENT SHADER PROGRAM *
 ************************************/
ShaderProgram::ShaderProgram(const QString vertex_shader_filename, const QString fragment_shader_filename) :
    m_vertex_shader_filename(vertex_shader_filename), m_fragment_shader_filename(fragment_shader_filename)
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
ComputeShaderProgram::ComputeShaderProgram(const QString compute_shader_filename) :
    m_compute_shader_filename(compute_shader_filename)
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
BaseShader::BaseShader() : ShaderProgram(":/base.vert", ":/base.frag")
{

}

BaseShader::~BaseShader()
{

}

/******************
 * TERRAIN SHADER *
 ******************/
TerrainShader::TerrainShader() : ShaderProgram(":/terrain.vert", ":/terrain.frag")
{

}

TerrainShader::~TerrainShader()
{

}

/******************
 * OVERLAY SHADER *
 ******************/
OverlayShader::OverlayShader() : ShaderProgram(":/overlay.vert", ":/overlay.frag")
{

}

OverlayShader::~OverlayShader()
{

}

/***************************
 * TERRAIN ELEMENTS SHADER *
 ***************************/
TerrainElementsShader::TerrainElementsShader() : ShaderProgram(":/terrain_elements.vert", ":/terrain_elements.frag")
{

}

TerrainElementsShader::~TerrainElementsShader()
{

}

/****************************
 * NORMALS GENERATOR SHADER *
 ****************************/
NormalsGeneratorShader::NormalsGeneratorShader() : ShaderProgram(":/normals_generator.vert", ":/normals_generator.frag")
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
WaterFluxGeneratorShader::WaterFluxGeneratorShader() : ComputeShaderProgram(":/water_flux.comp")
{

}

WaterFluxGeneratorShader::~WaterFluxGeneratorShader()
{

}


/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
const int WaterComparatorShader::_GROUP_SIZE_X = 32;
const int WaterComparatorShader::_GROUP_SIZE_Y = 32;
const int WaterComparatorShader::_GROUP_SIZE_Z = 1;
WaterComparatorShader::WaterComparatorShader() : ComputeShaderProgram(":/water_heightmap_comparator.comp")
{

}

WaterComparatorShader::~WaterComparatorShader()
{

}

/***********************************
 * SOIL HUMIDITY CALCULATOR SHADER *
 ***********************************/
const int SoilHumidityCalculatorShader::_GROUP_SIZE_X = 32;
const int SoilHumidityCalculatorShader::_GROUP_SIZE_Y = 32;
const int SoilHumidityCalculatorShader::_GROUP_SIZE_Z = 1;
SoilHumidityCalculatorShader::SoilHumidityCalculatorShader() : ComputeShaderProgram(":/soil_humidity_calculator.comp")
{

}

SoilHumidityCalculatorShader::~SoilHumidityCalculatorShader()
{

}

/****************************************
 * SLOPE BASED SOIL INFILTRATION SHADER *
 ****************************************/
const int SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_X = 32;
const int SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_Y = 32;
const int SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_Z = 1;
SlopeBasedSoilInfiltrationShader::SlopeBasedSoilInfiltrationShader() : ComputeShaderProgram(":/slope_based_soil_infiltration.comp")
{

}

SlopeBasedSoilInfiltrationShader::~SlopeBasedSoilInfiltrationShader()
{

}

/**********************************
 * OVERLAY TEXTURE CREATOR SHADER *
 **********************************/
const int OverlayTextureCreatorShader::_GROUP_SIZE_X = 32;
const int OverlayTextureCreatorShader::_GROUP_SIZE_Y = 32;
const int OverlayTextureCreatorShader::_GROUP_SIZE_Z = 1;
OverlayTextureCreatorShader::OverlayTextureCreatorShader() : ComputeShaderProgram(":/overlay_texture_creator.comp")
{

}

OverlayTextureCreatorShader::~OverlayTextureCreatorShader()
{

}

