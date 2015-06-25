#include "water_flow_analyser.h"
#include <algorithm>
#include <climits>

/********************
 * WATER FLOW ASSET *
 ********************/
WaterFlowAsset::WaterFlowAsset() : Asset(true, glm::vec4(.0f, .0f, 1.f, 1.f))
{

}

WaterFlowAsset::~WaterFlowAsset()
{

}

void WaterFlowAsset::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

void WaterFlowAsset::render()
{
    if(!initalised())
        initGL();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    m_vao_constraints.bind();

    f->glDrawArrays(GL_LINES, 0, m_verticies.size()); CE();

    m_vao_constraints.release();
}

void WaterFlowAsset::add(const glm::vec3 &start_point, const glm::vec3 &end_point)
{
    m_verticies.push_back(start_point[0]);
    m_verticies.push_back(start_point[1]);
    m_verticies.push_back(start_point[2]);

    m_verticies.push_back(end_point[0]);
    m_verticies.push_back(end_point[1]);
    m_verticies.push_back(end_point[2]);

    fillBuffers(); // Update buffers
}


/***********************
 * WATER FLOW ANALYZER *
 ***********************/
WaterFlowAnalyzer::WaterFlowAnalyzer() : m_water_flow(new WaterFlowAsset)
{

}

WaterFlowAnalyzer::~WaterFlowAnalyzer()
{
    delete m_water_flow;
}

void WaterFlowAnalyzer:: generateWaterFlow(const TerragenFile & height_map, glm::vec2 starting_point)
{
    glm::vec2 p1(starting_point);
    glm::vec2 p2;
    while(get_lowest_neighbour(height_map, p1, p2))
    {
        qCritical() << "Looking for closest neighbour to: " << p1[0] << ", " << p1[1];
        m_water_flow->add(glm::vec3(p1[0], height_map(p1[0],p1[1])+1, p1[1]), glm::vec3(p2[0], height_map(p2[0],p2[1])+1, p2[1]));
        p1 = p2;
        qCritical() << "Nearest neighbour: " << p1[0] << ", " << p1[1];
    }
}

bool WaterFlowAnalyzer::get_lowest_neighbour(const TerragenFile & height_map, glm::vec2 & reference_point, glm::vec2 & lowest_neighbour)
{
    float min(height_map(reference_point[0], reference_point[1]));
    bool found(false);
    for(int x = std::max(0, (int)reference_point[0]-1); x < std::min(height_map.m_header_data.width, (int) reference_point[0]+1); x++)
    {
        for(int z = std::max(0, (int)reference_point[1]-1); z < std::min(height_map.m_header_data.depth, (int)reference_point[1]+1); z++)
        {
            if((x != reference_point[0] || z != reference_point[1]) && height_map(x,z) < min )
            {
                found = true;
                lowest_neighbour = glm::vec2(x,z);
            }
        }
    }

    return found;
}

Asset * WaterFlowAnalyzer::asAsset()
{
    return m_water_flow;
}
