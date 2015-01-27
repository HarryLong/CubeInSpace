#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include "glheader.h"

class ShaderProgram
{
private:
    GLuint m_program_id;
    bool m_shader_ready;
    std::string m_frag_shader_src_file, m_vertex_shader_src_file, m_name;

    GLenum compileShader(GLenum p_target, GLchar* p_src_code, GLuint & p_shader);
    GLenum linkProgram(GLuint p_program);
    void initDefaults();
public:
    ShaderProgram(const std::string& p_frag_shader_src_file, const std::string& p_vertex_shader_src_file, std::string p_name = NULL); // construct from file
    ~ShaderProgram() {}

    bool compileAndLink(void); //compile and link shaders
    std::string getName() { return m_name; }
    GLuint getProgramID(void) const { return m_program_id; }
    bool initialised(void) const {return m_shader_ready; }
};

#endif
