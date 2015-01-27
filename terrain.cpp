#include "terrain.h"
#include <iostream>
#include <cfloat>

/*******************
 * TERRAIN NORMALS *
 *******************/
TerrainNormals::TerrainNormals() : m_valid(false)
{
    init();
}

TerrainNormals::~TerrainNormals()
{

}

void TerrainNormals::init()
{
    m_normalsTexUnit = GL_TEXTURE1;

    // Insert a rectangle that fills the entire screen in screen space into the vertices
    for(GLfloat coordinate : m_screen_quad)
    {
        m_verticies.push_back(coordinate);
    }

    bindBuffers(); // This never changes
}

bool TerrainNormals::bindBuffers()
{
    glGenVertexArrays(1, &m_vao_constraints); CE();
    glBindVertexArray(m_vao_constraints); CE();

    // set up vertex buffer an copy in data
    glGenBuffers(1, &m_vbo_constraints); CE();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_constraints); CE();
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW); CE();

    // enable position attribute
    glEnableVertexAttribArray(0); CE();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (void*)(0)); CE();

    return true;
}

bool TerrainNormals::setTerrainDim(int width, int depth)
{
    m_valid = false;
    if( m_normalsTexture == 0 || m_width != width || m_depth != depth )
    {
        glGenFramebuffers(1, &m_fbo_normal_map); CE();
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_normal_map); CE();
        // create texture target for normal map computation


        glActiveTexture(m_normalsTexUnit); // normal map is bound to this TIU
        glGenTextures(1, &m_normalsTexture); CE();
        glBindTexture(GL_TEXTURE_2D, m_normalsTexture); CE();
        // set up texture state.

        // Give an empty image to OpenGL ( the last "0" )
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F, width, depth, 0,GL_RGBA, GL_FLOAT, 0); CE();

        // no filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CE();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CE();
        // deal with out of array access
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        // configure FBO
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_normalsTexture, 0); CE();

        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);  CE(); // "1" is the size of DrawBuffers

        // Always check that our framebuffer is ok
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Normal map FBO initialisation failed\n";
            return false;
        }

        // unbind FBO

        glBindFramebuffer(GL_FRAMEBUFFER,  0); CE();

        m_width = width;
        m_depth = depth;
    }
    return true;
}

// END OF TERRAIN NORMALS

/***********
 * TERRAIN *
 ***********/

Terrain::Terrain()
{
    init();
}

Terrain::~Terrain()
{
}

void Terrain::init()
{
    // Initialize texture units
    m_htmapTexUnit = GL_TEXTURE0;
}

bool Terrain::bindBuffers()
{
    if(m_indicies.size() > 0)
    {
        if (m_vbo_constraints != 0)
        {
            glDeleteVertexArrays(1, &m_vao_constraints);
            glDeleteBuffers(1, &m_vbo_constraints);
            glDeleteBuffers(1, &m_ibo_constraints);
            m_vao_constraints = 0;
            m_vbo_constraints = 0;
            m_ibo_constraints = 0;
        }

        glGenVertexArrays(1, &m_vao_constraints); CE();
        glBindVertexArray(m_vao_constraints); CE();

        // set up vertex buffer an copy in data
        glGenBuffers(1, &m_vbo_constraints); CE();
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_constraints); CE();
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW); CE();

        // enable position attribute
        // Vertex position
        glEnableVertexAttribArray(0); CE();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(0)); CE();
        // Texture coordinate
        glEnableVertexAttribArray(1); CE();
        const int sz = 3*sizeof(GLfloat);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(sz)); CE();

        // set up index buffer
        glGenBuffers(1, &m_ibo_constraints);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_constraints); CE();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*m_indicies.size(), &m_indicies[0], GL_STATIC_DRAW); CE();
    }
    return true;
}

bool Terrain::setTerrain(TerragenFile parsed_terrangen_file)
{
    parsed_terrangen_file.summarize();

    if(m_heightmapTexture != 0 &&
            (m_header_data.width != parsed_terrangen_file.m_header_data.width ||
             m_header_data.depth != parsed_terrangen_file.m_header_data.depth))
    {
        // std::cerr << "- Delete texture\n";
        glDeleteTextures(1, &m_heightmapTexture);  CE();
        m_heightmapTexture = 0;
    }

    if (m_heightmapTexture == 0) // create texture if it does not exist
    {
        // std::cerr << "- Create texture\n";
        glGenTextures(1, &m_heightmapTexture); CE();
        glActiveTexture(m_htmapTexUnit); CE();
        glBindTexture(GL_TEXTURE_2D, m_heightmapTexture ); CE();

        glTexImage2D(GL_TEXTURE_2D, 0,GL_R32F, parsed_terrangen_file.m_header_data.width,
                     parsed_terrangen_file.m_header_data.depth, 0,GL_RED, GL_FLOAT,  (GLfloat*)parsed_terrangen_file.m_height_data); CE();
        // no filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CE();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CE();
        // deal with out of array access
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
    else // otherwise sub in new texture data
    {
        // std::cerr << " - sub texture\n";
        glActiveTexture(m_htmapTexUnit); CE();
        glBindTexture(GL_TEXTURE_2D, m_heightmapTexture ); CE();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, parsed_terrangen_file.m_header_data.width,
                        parsed_terrangen_file.m_header_data.depth, GL_RED, GL_FLOAT, (GLfloat*)parsed_terrangen_file.m_height_data); CE();
    }

    m_header_data = parsed_terrangen_file.m_header_data;

    m_terrain_normals.setTerrainDim(m_header_data.width, m_header_data.depth);
    prepare_terrain_geometry();
    return bindBuffers();
}

#include "constants.h"
void Terrain::prepare_terrain_geometry()
{
    // Vertices
    m_verticies.clear();
    for (int z = 0; z < m_header_data.depth; z++)
    {
        for (int x = 0; x < m_header_data.width; x++)
        {
            // 3D Vertex coordinate
            m_verticies.push_back(((float) x) * m_header_data.dynamic_scale /*- (m_header_data.width/2 * m_header_data.dynamic_scale )*/); // X
            m_verticies.push_back(.0f); // Y (stored in heightmap texture)
            m_verticies.push_back(((float) z) * m_header_data.dynamic_scale /*- (m_header_data.depth/2 * m_header_data.dynamic_scale )*/); // Z

            // 2D texture coordinate
            m_verticies.push_back((float) x / (float) (m_header_data.width - 1)); // X
            m_verticies.push_back((float) z / (float) (m_header_data.depth - 1)); // Y
        }
    }

    // Indices
    m_indicies.clear();
    for (int z = 0; z < m_header_data.width - 1; z++)
    {
        if (z > 0) // Degenerate begin: repeat first vertex
            m_indicies.push_back((GLuint) (z * m_header_data.width));

        for (int x = 0; x < m_header_data.width; x++)
        {
            // One part of the strip
            m_indicies.push_back((GLuint) ((z * m_header_data.width) + x));
            m_indicies.push_back((GLuint) (((z + 1) * m_header_data.width) + x));
        }

        if (z <  m_header_data.depth - 2)   // Degenerate end: repeat last vertex
            m_indicies.push_back((GLuint) (((z + 1) * m_header_data.width) + (m_header_data.width - 1)));
    }
}
