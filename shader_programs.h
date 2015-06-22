#ifndef SHADER_PROGRAMS
#define SHADER_PROGRAMS

#include <QGLShaderProgram>

class ShaderProgram : public QGLShaderProgram
{
    Q_OBJECT
public:
    ShaderProgram(const QString & vertex_shader_filename, const QString & fragment_shader_filename, QObject * parent = 0);
    ~ShaderProgram();
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

#endif //SHADER_PROGRAMS
