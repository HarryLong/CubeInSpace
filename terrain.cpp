﻿#include "terrain.h"
#include <iostream>
#include <cfloat>
#include "geom.h"
#include <math.h>

#include "utils/utils.h"
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

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

bool TerrainNormals::setTerrainDim(int width, int depth)
{
    if( m_normalsTexture == 0 || m_width != width || m_depth != depth )
    {
//        delete_buffers();

//        if (m_normalsTexture != 0)
//            glDeleteTextures(1, &m_normalsTexture);  CE();

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
    return bindBuffers();
}

void TerrainNormals::delete_buffers()
{
    if (m_fbo_normal_map != 0) glDeleteFramebuffers(1, &m_fbo_normal_map);  CE();
    GlDrawable::delete_buffers();
}

void TerrainNormals::render() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_normal_map); CE();

    // set shader program to normal map gen
    glBindVertexArray(m_vao_constraints); CE();

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);  CE();

    // unbind everthing
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  CE();
    glBindVertexArray(0);  CE();
}

/*********************
 * TERRAIN RECTANGLE *
 *********************/
TerrainRect::TerrainRect(glm::vec3 min, glm::vec3 max, int terrain_width, int terrain_depth) : Asset(true, glm::vec4(0.3, 0.3, 0.3, 0.5))
{
    int rect_width(max[0] - min[0]);
    int rect_depth(max[2] - min[2]);

    // Vertices
    for (int z (min[2]); z < max[2]; z++)
    {
        for (int x (min[0]); x < max[0]; x++)
        {
            // 3D Vertex coordinate
            m_verticies.push_back((float) x);
            m_verticies.push_back(.0f); // Y (stored in heightmap texture)
            m_verticies.push_back((float) z);

            // 2D texture coordinate
            m_verticies.push_back((float) x / (float) (terrain_width - 1)); // X
            m_verticies.push_back((float) z / (float) (terrain_depth - 1)); // Y
        }
    }

    // Indices
    for (int z(0); z < rect_depth - 1; z++)
    {
        if (z > 0) // Degenerate begin: repeat first vertex
            m_indicies.push_back((GLuint) (z * rect_width));

        for (int x = 0; x < rect_width; x++)
        {
            // One part of the strip
            m_indicies.push_back((GLuint) ((z * rect_width) + x));
            m_indicies.push_back((GLuint) (((z + 1) * rect_width) + x));
        }

        if (z <  rect_depth - 2)   // Degenerate end: repeat last vertex
            m_indicies.push_back((GLuint) (((z + 1) * rect_width) + (rect_width - 1)));
    }

    bindBuffers();
}

TerrainRect::~TerrainRect()
{

}

bool TerrainRect::bindBuffers()
{
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

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

void TerrainRect::render() const
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(m_vao_constraints); CE();
    glDrawElements(GL_TRIANGLE_STRIP, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    glDisable(GL_BLEND);

    glBindVertexArray(0); // Unbind
}

/***********
 * TERRAIN *
 ***********/
Terrain::Terrain() : m_heightmapTexture(0), m_htmapTexUnit(GL_TEXTURE0)
{
    init();
}

Terrain::~Terrain()
{
    clearTerrainElements();
}

void Terrain::init()
{
}

void Terrain::render() const
{
    glBindVertexArray(m_vao_constraints); CE();

    glDrawElements(GL_TRIANGLE_STRIP, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    glBindVertexArray(0); // Unbind
}

bool Terrain::bindBuffers()
{
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

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

bool Terrain::setTerrain(TerragenFile parsed_terrangen_file)
{
    parsed_terrangen_file.summarize();
    delete_buffers();

    refresh_heightmap_texture(parsed_terrangen_file);

    if(m_terragen_file.m_height_data)
        free(m_terragen_file.m_height_data);
    m_terragen_file = parsed_terrangen_file;

    m_terrain_normals.setTerrainDim(m_terragen_file.m_header_data.width, m_terragen_file.m_header_data.depth);
    m_terrain_normals.setValid(false);

    prepare_terrain_geometry();

    build_sphere_acceleration_structure();

    // Recalculate center
    m_center = glm::vec2(m_terragen_file.m_header_data.width/2.0f, m_terragen_file.m_header_data.depth/2.0f);

    return bindBuffers();
}

#include "constants.h"
void Terrain::prepare_terrain_geometry()
{
    // Vertices
    m_verticies.clear();
    for (int z = 0; z < m_terragen_file.m_header_data.depth; z++)
    {
        for (int x = 0; x < m_terragen_file.m_header_data.width; x++)
        {
            // 3D Vertex coordinate
            m_verticies.push_back((float) x);/*- (m_header_data.width/2 * m_header_data.dynamic_scale )*/ // X
            m_verticies.push_back(.0f); // Y (stored in heightmap texture)
            m_verticies.push_back((float) z);/*- (m_header_data.depth/2 * m_header_data.dynamic_scale )*/ // Z

            // 2D texture coordinate
            m_verticies.push_back((float) x / (float) (m_terragen_file.m_header_data.width - 1)); // X
            m_verticies.push_back((float) z / (float) (m_terragen_file.m_header_data.depth - 1)); // Y
        }
    }

    // Indices
    m_indicies.clear();
    for (int z = 0; z < m_terragen_file.m_header_data.width - 1; z++)
    {
        if (z > 0) // Degenerate begin: repeat first vertex
            m_indicies.push_back((GLuint) (z * m_terragen_file.m_header_data.width));

        for (int x = 0; x < m_terragen_file.m_header_data.width; x++)
        {
            // One part of the strip
            m_indicies.push_back((GLuint) ((z * m_terragen_file.m_header_data.width) + x));
            m_indicies.push_back((GLuint) (((z + 1) * m_terragen_file.m_header_data.width) + x));
        }

        if (z <  m_terragen_file.m_header_data.depth - 2)   // Degenerate end: repeat last vertex
            m_indicies.push_back((GLuint) (((z + 1) * m_terragen_file.m_header_data.width) + (m_terragen_file.m_header_data.width - 1)));
    }

    std::cout << "Indicies : " << m_indicies.size() << std::endl;
}

void Terrain::refresh_heightmap_texture(TerragenFile & parsed_terrangen_file)
{
    if(m_heightmapTexture != 0 &&
            (m_terragen_file.m_header_data.width != parsed_terrangen_file.m_header_data.width ||
             m_terragen_file.m_header_data.depth != parsed_terrangen_file.m_header_data.depth))
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

    m_terrain_normals.setValid(false);
}

bool Terrain::ray_intersect(const glm::vec3 & start, const glm::vec3 & direction, glm::vec3 & intersection_point)
{
    // bounding sphere accel structure
    float scaler(.0f), distance(.0f);
    float tolerance (((float) getWidth()) / (getDepth()-1));
    bool found(false);

    float min_distance(FLT_MAX);

    for(int x (0); x < m_sphere_acceleration_structure.n_spheres_x; x++)
        for(int z (0); z < m_sphere_acceleration_structure.n_spheres_z; z++)
        {
            SphereAccelerationStructure::Sphere & sphere(m_sphere_acceleration_structure.m_spheres[x][z]);
            Geom::rayPointDist(start, direction, sphere.center, scaler, distance);
            if(distance <= sphere.radius) // intersects enclosing sphere so test enclosed points
            {
                int x_min(x * m_sphere_acceleration_structure.step_size);
                int x_max(std::min(getWidth()-1, x_min + m_sphere_acceleration_structure.step_size));

                int z_min(z * m_sphere_acceleration_structure.step_size);
                int z_max(std::min(getDepth()-1, z_min + m_sphere_acceleration_structure.step_size));

                for(int point_in_sphere_x = x_min; point_in_sphere_x < x_max; point_in_sphere_x++)
                    for(int point_in_sphere_z = z_min; point_in_sphere_z < z_max; point_in_sphere_z++)
                    {
                        glm::vec3 point_in_sphere(point_in_sphere_x, m_terragen_file(point_in_sphere_x,point_in_sphere_z), point_in_sphere_z);
                        Geom::rayPointDist(start, direction, point_in_sphere, scaler, distance);
                        if(distance < tolerance && distance < min_distance)
                        {
                            found = true;
                            min_distance = distance;
                            intersection_point = point_in_sphere;
                        }
                }
            }
        }
    return found;
}

void Terrain::build_sphere_acceleration_structure()
{
    m_sphere_acceleration_structure.clear();

    m_sphere_acceleration_structure.n_spheres_x = (getWidth()-1) / m_sphere_acceleration_structure.step_size + 1;
    m_sphere_acceleration_structure.n_spheres_z = (getDepth()-1) / m_sphere_acceleration_structure.step_size + 1;

    for(int x(0); x < getWidth(); x += m_sphere_acceleration_structure.step_size )
    {
        std::vector<SphereAccelerationStructure::Sphere > column_of_spheres;
        for(int z(0); z < getDepth(); z += m_sphere_acceleration_structure.step_size)
        {
            int x_max(std::min(getWidth()-1, x + m_sphere_acceleration_structure.step_size));
            int z_max(std::min(getDepth()-1, z + m_sphere_acceleration_structure.step_size));

            glm::vec3 p_min(x, m_terragen_file(x,z), z);
            glm::vec3 p_max(x_max, m_terragen_file(x_max,z_max), z_max);

            glm::vec3 sphere_center(Geom::affinecombine(0.5f, p_min, 0.5f, p_max));

//            std::cout << "p_min: "; Utils::print(p_min); std::cout << std::endl;
//            std::cout << "p_max: "; Utils::print(p_max); std::cout << std::endl;
//            std::cout << "sphere_center: "; Utils::print(sphere_center); std::cout << std::endl;

            // Calculate the maximum radius
            float squared_radius(.0f);
            for( int point_in_sphere_x(x); point_in_sphere_x < x_max; point_in_sphere_x++ )
                for( int point_in_sphere_z(z); point_in_sphere_z < z_max; point_in_sphere_z++)
                {
                    glm::vec3 point_in_sphere(point_in_sphere_x, m_terragen_file(point_in_sphere_x, point_in_sphere_z), point_in_sphere_z);
//                    std::cout << "point_in_sphere: "; Utils::print(point_in_sphere); std::cout << std::endl;

                    // calculate the distance from the center
                    glm::vec3 center_to_point(Geom::diff(sphere_center, point_in_sphere));
//                    std::cout << "center_to_point: "; Utils::print(center_to_point); std::cout << std::endl;

                    float squared_length(Geom::squaredLength(center_to_point));

                    if(squared_length > squared_radius)
                        squared_radius = squared_length;
                }

            // Scale & Add sphere to structure
            float radius(sqrt(squared_radius));

////            std::cout << "scaled_sphere_center: "; Utils::print(sphere_center); std::cout << std::endl;
//            std::cout << "Sphere radius: " << radius << std::endl;

            SphereAccelerationStructure::Sphere  sphere(sphere_center, radius);
////            std::cout << "Going to add sphere with center: "; Utils::print(sphere.center);
////            std::cout << " and radius " << radius << std::endl;
            column_of_spheres.push_back(sphere);
        }
        m_sphere_acceleration_structure.m_spheres.push_back(column_of_spheres);

////    std::cout << "*** SPHERES ADDED ***" << std::endl;
////    for(std::vector<Sphere> spheres : m_sphere_acceleration_structure.m_spheres)
////    {
////        for(Sphere sphere : spheres)
////        {
////            std::cout << "Center: "; Utils::print(sphere.center); std::couawt << std::endl;
////        }
////    }
////    std::cout << "***********************" << std::endl;
}
}

void Terrain::incrementHeights(const std::vector<glm::vec3> & points, int increment)
{
    for(glm::vec3 point : points)
    {
        m_terragen_file(point[0], point[2]) += increment;
    }
    refresh_heightmap_texture(m_terragen_file);
    build_sphere_acceleration_structure();
}

void Terrain::addTerrainRect(glm::vec3 min, glm::vec3 max)
{
    m_terrain_rectangles.push_back(new TerrainRect(min, max, m_terragen_file.m_header_data.width,
                                               m_terragen_file.m_header_data.depth));
}

void Terrain::clearTerrainElements()
{
    clear_terrain_rectangles();
}

bool Terrain::traceRay(glm::vec3 start_point, glm::vec3 direction, glm::vec3 & intersection_point)
{
    return ray_intersect(start_point, direction, intersection_point);
}

glm::vec2 Terrain::getCenter()
{
    return m_center;
}

void Terrain::clear_terrain_rectangles()
{
    for(TerrainRect * rect : m_terrain_rectangles)
        delete rect;

    m_terrain_rectangles.clear();
}

const std::vector<const Asset*> Terrain::getTerrainElements()
{
    return std::vector<const Asset*>(m_terrain_rectangles.begin(), m_terrain_rectangles.end());
//    return m_terrain_rectangles;
}

