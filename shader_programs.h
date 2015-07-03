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
    ShaderProgram(const QString & vertex_shader_filename, const QString & fragment_shader_filename, QObject * parent = 0);
    ~ShaderProgram();
};

/**************************
 * COMPUTE SHADER PROGRAM *
 **************************/
class ComputeShaderProgram : public QOpenGLShaderProgram
{
    Q_OBJECT
public:
    ComputeShaderProgram(const QString & computer_shader_filename, QObject * parent = 0);
    ~ComputeShaderProgram();
};

/***************
 * BASE SHADER *
 ***************/
class BaseShader : public ShaderProgram
{
public:
    BaseShader(QObject * parent = 0);
    ~BaseShader();
};

/******************
 * TERRAIN SHADER *
 ******************/
class TerrainShader : public ShaderProgram
{
public:
    TerrainShader(QObject * parent = 0);
    ~TerrainShader();
};

/***************************
 * TERRAIN ELEMENTS SHADER *
 ***************************/
class TerrainElementsShader : public ShaderProgram
{
public:
    TerrainElementsShader(QObject * parent = 0);
    ~TerrainElementsShader();
};

/****************************
 * NORMALS GENERATOR SHADER *
 ****************************/
class NormalsGeneratorShader : public ShaderProgram
{
public:
    NormalsGeneratorShader(QObject * parent = 0);
    ~NormalsGeneratorShader();
};

/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
class WaterFluxGeneratorShader : public ComputeShaderProgram
{
public:
    WaterFluxGeneratorShader(QObject * parent = 0);
    ~WaterFluxGeneratorShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;

    struct Uniforms{
        static const QString _TERRAIN_HEIGHTMAP;
        static const QString _WATER_HEIGHTMAP;
    };
};

#endif //SHADER_PROGRAMS
