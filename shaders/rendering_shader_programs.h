#ifndef RENDERING_SHADER_PROGRAMS
#define RENDERING_SHADER_PROGRAMS

#include <QOpenGLShaderProgram>

/************************************
 * VERTEX & FRAGMENT SHADER PROGRAM *
 ************************************/
class RenderingShaderProgram : public QOpenGLShaderProgram
{
    Q_OBJECT
public:
    RenderingShaderProgram(const QString vertex_shader_filename, const QString fragment_shader_filename);
    ~RenderingShaderProgram();
    void compileAndLink();

private:
    const QString m_vertex_shader_filename, m_fragment_shader_filename;
};


/***************
 * BASE SHADER *
 ***************/
class BaseShader : public RenderingShaderProgram
{
public:
    BaseShader();
    ~BaseShader();
};

/******************
 * TERRAIN SHADER *
 ******************/
class TerrainShader : public RenderingShaderProgram
{
public:
    TerrainShader();
    ~TerrainShader();
};

/***************************
 * TERRAIN ELEMENTS SHADER *
 ***************************/
class TerrainElementsShader : public RenderingShaderProgram
{
public:
    TerrainElementsShader();
    ~TerrainElementsShader();
};

/****************************
 * NORMALS GENERATOR SHADER *
 ****************************/
class NormalsGeneratorShader : public RenderingShaderProgram
{
public:
    NormalsGeneratorShader();
    ~NormalsGeneratorShader();

    struct Uniforms{
        static const char * _TERRAIN_HEIGHTMAP;
    };
};

#endif //RENDERING_SHADER_PROGRAMS
