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
    ShaderProgram(const QString vertex_shader_filename, const QString fragment_shader_filename, QObject * parent = 0);
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
    ComputeShaderProgram(const QString compute_shader_filename, QObject * parent = 0);
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
    WaterFluxGeneratorShader(QObject * parent = 0);
    ~WaterFluxGeneratorShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

#endif //SHADER_PROGRAMS
