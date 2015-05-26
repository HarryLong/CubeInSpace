#include "shader_program.h"
#include <fstream>
#include <iostream>
#include "glheader.h"

#define FailGLError(X) {int err = (int)glGetError(); \
        if (err != GL_NO_ERROR) \
        {\
            std::cerr << (X); std::cerr << " error " << err << std::endl; \
            const char* err_str = reinterpret_cast<const char *>(gluErrorString(err));\
            std::cerr << " - GL Error message: " << err_str << std::endl;\
            return err;\
        }}

ShaderProgram::ShaderProgram(const std::string & p_fragment_src_file, const std::string & p_vertex_src_file, std::string p_name) :
    m_frag_shader_src_file(p_fragment_src_file), m_vertex_shader_src_file(p_vertex_src_file), m_name(p_name)
{
    initDefaults();
}

void ShaderProgram::initDefaults()
{
    m_program_id = 0;
    m_shader_ready = false;
}

GLenum ShaderProgram::linkProgram(GLuint p_program)
{
    GLint log_length = 0;
    GLint linked = 0;

    glLinkProgram(p_program);
    FailGLError("Failed glLinkProgram")

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
        FailGLError("shader did not link")

    return GL_NO_ERROR;
}

GLenum ShaderProgram::compileShader(GLenum p_target, GLchar* p_source_code, GLuint & p_shader)
{
    GLint   log_length = 0;
    GLint   compiled  = 0;

    if (p_source_code != 0)
    {
        p_shader = glCreateShader(p_target);
        FailGLError("Failed to create fragment shader");
        glShaderSource(p_shader,1,(const GLchar **)&p_source_code,0);
        FailGLError("Failed glShaderSource")
        glCompileShader(p_shader);
        FailGLError("Failed glCompileShader")

        glGetShaderiv(p_shader,GL_COMPILE_STATUS,&compiled);
        glGetShaderiv(p_shader,GL_INFO_LOG_LENGTH,&log_length);

        if (log_length > 1)
        {
            GLint charsWritten;
            GLchar *log = new char [log_length+128];
            glGetShaderInfoLog(p_shader, log_length, &charsWritten, log);
            std::cerr << "Compilation log: nchars=(" << log_length << "): "<< (char*)log << std::endl;
            delete [] log;
        }

        if (compiled == 0)
            FailGLError("shader could not compile")
    }
    return GL_NO_ERROR;
}

bool ShaderProgram::compileAndLink(void)
{
    if (m_shader_ready)
        return true;

    // READ IN INPUT FILES
    std::ifstream vertex_shader_input_stream(m_vertex_shader_src_file.c_str(), std::ios::binary);
    std::ifstream fragment_shader_input_stream(m_frag_shader_src_file.c_str(), std::ios::binary);

    if (!vertex_shader_input_stream)
    {
        std::cerr << "could not open shader source: " << m_vertex_shader_src_file << std::endl;
        return false;
    }

    if (!fragment_shader_input_stream)
    {
        std::cerr << "could not open shader source: " << m_frag_shader_src_file << std::endl;
        return false;
    }

    std::string vertex_src_str, fragment_src_str;

    std::string tmp_str;

    while (std::getline(vertex_shader_input_stream, tmp_str))
    {
        vertex_src_str += tmp_str;
        vertex_src_str += "\n";
    }

    while (std::getline(fragment_shader_input_stream, tmp_str))
    {
        fragment_src_str += tmp_str;
        fragment_src_str += "\n";
    }

    vertex_shader_input_stream.close();
    fragment_shader_input_stream.close();

    // COMPILE SHADERS
    GLuint fragment_shader_Id;
    GLuint vertex_shader_id;
    GLuint err = compileShader(GL_VERTEX_SHADER, const_cast<GLchar*>(vertex_src_str.c_str()), vertex_shader_id);
    if (0 != err)
    {
        std::cerr << "Vertex Shader could not compile\n";
        return false;
    }
    err = compileShader(GL_FRAGMENT_SHADER, const_cast<GLchar*>(fragment_src_str.c_str()), fragment_shader_Id);
    if (0 != err)
    {
        std::cerr << "Fragment Shader could not compile\n";
        return false;
    }

    m_program_id = glCreateProgram(); CE()
    glAttachShader(m_program_id, vertex_shader_id);CE()
    glAttachShader(m_program_id, fragment_shader_Id);CE()

    err = linkProgram(m_program_id); CE()
    if (GL_NO_ERROR != err)
    {
        std::cerr << "Program could not link\n";
                return false;
    }

    // detach and delete shader objects, we don't need them anymore (NB: may cause issues with dodgy drivers)
    if (fragment_shader_Id != 0)
    {
        glDetachShader(m_program_id, fragment_shader_Id);CE()
        glDeleteShader(fragment_shader_Id);CE()
    }
    if (vertex_shader_id != 0)
    {
        glDetachShader(m_program_id, vertex_shader_id);CE()
        glDeleteShader(vertex_shader_id);CE()
    }

    m_shader_ready = true;
    return true;
}
