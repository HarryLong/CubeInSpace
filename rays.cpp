#include "rays.h"
#include <vector>

RayDrawer::RayDrawer() : Asset(true, glm::vec4(.294f, .0f, .51f, 1.f))
{

}

RayDrawer::~RayDrawer()
{

}

void RayDrawer::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

void RayDrawer::render()
{
    if(!initalised())
        initGL();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    m_vao_constraints.bind();

    f->glDrawArrays(GL_LINES, 0, m_verticies.size()); CE();

    m_vao_constraints.release();
}

void RayDrawer::add(const glm::vec3 &start_point, const glm::vec3 &end_point)
{
    m_verticies.push_back(start_point[0]);
    m_verticies.push_back(start_point[1]);
    m_verticies.push_back(start_point[2]);

    m_verticies.push_back(end_point[0]);
    m_verticies.push_back(end_point[1]);
    m_verticies.push_back(end_point[2]);

    fillBuffers(); // Update buffers
}
