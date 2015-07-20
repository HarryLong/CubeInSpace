#ifndef SHADER_PROGRAMS
#define SHADER_PROGRAMS

#include <QOpenGLShaderProgram>

/************************************
 * VERTEX & FRAGMENT SHADER PROGRAM *
 ************************************/
class ShaderProgram : public QOpenGLShaderProgram
{
    Q_OBJECT
public:
    ShaderProgram(const QString vertex_shader_filename, const QString fragment_shader_filename);
    ~ShaderProgram();
    void compileAndLink();

private:
    const QString m_vertex_shader_filename, m_fragment_shader_filename;
};

/**************************
 * COMPUTE SHADER PROGRAM *
 **************************/
class ComputeShaderProgram : public QOpenGLShaderProgram
{
    Q_OBJECT
public:
    ComputeShaderProgram(const QString compute_shader_filename);
    ~ComputeShaderProgram();
    void compileAndLink();

private:
    const QString m_compute_shader_filename;
};


/***************
 * BASE SHADER *
 ***************/
class BaseShader : public ShaderProgram
{
public:
    BaseShader();
    ~BaseShader();
};

/******************
 * TERRAIN SHADER *
 ******************/
class TerrainShader : public ShaderProgram
{
public:
    TerrainShader();
    ~TerrainShader();
};

/******************
 * OVERLAY SHADER *
 ******************/
class OverlayShader : public ShaderProgram
{
public:
    OverlayShader();
    ~OverlayShader();
};

/***************************
 * TERRAIN ELEMENTS SHADER *
 ***************************/
class TerrainElementsShader : public ShaderProgram
{
public:
    TerrainElementsShader();
    ~TerrainElementsShader();
};

/****************************
 * NORMALS GENERATOR SHADER *
 ****************************/
class NormalsGeneratorShader : public ShaderProgram
{
public:
    NormalsGeneratorShader();
    ~NormalsGeneratorShader();

    struct Uniforms{
        static const char * _TERRAIN_HEIGHTMAP;
    };
};

/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
class WaterFluxGeneratorShader : public ComputeShaderProgram
{
public:
    WaterFluxGeneratorShader();
    ~WaterFluxGeneratorShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
class WaterComparatorShader : public ComputeShaderProgram
{
public:
    WaterComparatorShader();
    ~WaterComparatorShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/***********************************
 * SOIL HUMIDITY CALCULATOR SHADER *
 ***********************************/
class SoilHumidityCalculatorShader : public ComputeShaderProgram
{
public:
    SoilHumidityCalculatorShader();
    ~SoilHumidityCalculatorShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/****************************************
 * SLOPE BASED SOIL INFILTRATION SHADER *
 ****************************************/
class SlopeBasedSoilInfiltrationShader : public ComputeShaderProgram
{
public:
    SlopeBasedSoilInfiltrationShader();
    ~SlopeBasedSoilInfiltrationShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};



#endif //SHADER_PROGRAMS
