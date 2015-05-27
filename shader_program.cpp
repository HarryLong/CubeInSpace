#include "shader_program.h"
#include <fstream>
#include <iostream>
#include "glheader.h"

ShaderProgram::ShaderProgram(QString p_frag_shader_src_file, QString p_vertex_shader_src_file, QString p_name) :
    m_program_id(0), m_shader_ready(false), m_frag_shader(p_frag_shader_src_file), m_vertex_shader(p_vertex_shader_src_file), m_destription(p_name)
{

}

bool ShaderProgram::compileAndLink()
{
    if (m_shader_ready)
        return true;

    bool success(true);

    m_program_id = glCreateProgram(); CE()

    success &= compile_and_attach(m_frag_shader, GL_FRAGMENT_SHADER, m_fragment_shader_id);
    success &= compile_and_attach(m_vertex_shader, GL_VERTEX_SHADER, m_vertex_shader_id);

    success &= ( link(m_program_id) == GL_NO_ERROR);

    m_shader_ready = success;

    // Program is linked - we can detach and delete shader objects
    if (m_fragment_shader_id != 0)
    {
        glDetachShader(m_program_id, m_fragment_shader_id);CE()
        glDeleteShader(m_fragment_shader_id);CE()
    }
    if (m_vertex_shader_id != 0)
    {
        glDetachShader(m_program_id, m_vertex_shader_id);CE()
        glDeleteShader(m_vertex_shader_id);CE()
    }

    return success;
}

bool ShaderProgram::compile_and_attach(QFile & shader_file, GLenum shader_type, GLuint & shader_id)
{
    if(!shader_file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Failed to open shader file: " << shader_file.fileName().toStdString() << std::endl;
        return false;
    }

    QByteArray shader_src(shader_file.readAll());

    shader_file.close();

    // COMPILE SHADER
    GLuint err ( compile(shader_type, shader_src, shader_id) );
    if (0 != err)
    {
        std::cerr << "Failed to compile shader: " << shader_file.fileName().toStdString() << std::endl;
        return false;
    }

    // ATTACH SHADER
    glAttachShader(m_program_id, shader_id);CE()

    return true;
}


GLenum ShaderProgram::link(GLuint p_program)
{
    GLint log_length = 0;
    GLint linked = 0;

    glLinkProgram(p_program); CE();

    glGetProgramiv(p_program,GL_LINK_STATUS ,&linked);
    glGetProgramiv(p_program,GL_INFO_LOG_LENGTH,&log_length);

    if (log_length > 1)
    {
        GLint   chars_written;
        GLchar *log = new char [log_length+128];

        glGetProgramInfoLog(p_program, log_length, &chars_written, log);
        std::cerr << "Link GetProgramInfoLog: nchars=(" << chars_written << "): " << (char*)log << std::endl;
        delete [] log;
    }

    if (linked == 0)
        std::cerr << "Shader " << m_destription.toStdString() << " did not link!" << std::endl;

    return GL_NO_ERROR;
}

GLenum ShaderProgram::compile(GLenum p_shader_type, QByteArray & p_shader_src, GLuint & p_shader_id)
{
    GLint   log_length = 0;
    GLint   compiled  = 0;

    const GLchar * src(p_shader_src.constData());

    if (p_shader_src != 0)
    {
        p_shader_id = glCreateShader(p_shader_type); CE();
        glShaderSource(p_shader_id,1,&src,0); CE();
        glCompileShader(p_shader_id); CE();

        glGetShaderiv(p_shader_id,GL_COMPILE_STATUS,&compiled); CE();
        glGetShaderiv(p_shader_id,GL_INFO_LOG_LENGTH,&log_length); CE();

        if (log_length > 1)
        {
            GLint charsWritten;
            GLchar *log = new char [log_length+128];
            glGetShaderInfoLog(p_shader_id, log_length, &charsWritten, log);
            std::cerr << "Compilation log: nchars=(" << log_length << "): "<< (char*)log << std::endl;
            delete [] log;
        }

        if (compiled == 0)
            std::cerr << "Shader " << m_destription.toStdString() << " did not compile!" << std::endl;
    }
    return GL_NO_ERROR;
}
