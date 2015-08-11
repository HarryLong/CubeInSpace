#include "padded_terrain.h"
#include <cstring>

PaddedTerrain::PaddedTerrain() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

PaddedTerrain::~PaddedTerrain()
{

}

void PaddedTerrain::refresh(Terrain & terrain)
{
    int terrain_width(terrain.getWidth());
    int terrain_depth(terrain.getDepth());

    int padded_terrain_width(terrain_width+2);
    int padded_terrain_depth(terrain_depth+2);

    int sz (sizeof(GLfloat)*padded_terrain_width*padded_terrain_depth);
    GLfloat * padded_data = (GLfloat*) std::malloc(sz);

    const GLfloat * base_data = terrain.getDrawableTerrain().getRawData();

#pragma omp parallel for
    for(int z = 0; z < padded_terrain_depth; z++)
    {
        for(int x = 0; x < padded_terrain_width; x++)
        {
            padded_data[z*padded_terrain_width+x] = 1;
        }
    }

//    std::memset(padded_data,0, sz);

//     First copy across the data
#pragma omp parallel for
    for(int z = 0; z < terrain_depth; z++)
    {
        int padded_start_index (((z+1) * padded_terrain_width) + 1);
        std::memcpy(&padded_data[padded_start_index], &base_data[z*terrain_width], terrain_width * sizeof(GLfloat));
    }

    // Now deal with the borders

    /***************
     * X=0 BORDER *
     ***************/
    TerrainNormals & terrain_normals (terrain.getNormals());
    glm::vec3 minus_x_vector(-1,0,0);
#pragma omp parallel for
    for(int z = 0; z < terrain_depth; z++)
    {
        int padded_index ((z+1) * padded_terrain_width);
        glm::vec3 normal (terrain_normals(0,z));
        float dot_product (glm::dot(minus_x_vector, normal));

        if(dot_product > 0) // Water runs off
        {
            padded_data[padded_index] = -1;
        }
    }
    /**************************
     * X=TERRAIN_WIDTH BORDER *
     **************************/
    glm::vec3 plus_x_vector(1,0,0);
#pragma omp parallel for
    for(int z = 0; z < terrain_depth; z++)
    {
        int padded_index ((z+2) * padded_terrain_width - 1);
        glm::vec3 normal (terrain_normals(terrain_width-1,z));
        float dot_product (glm::dot(plus_x_vector, normal));

        if(dot_product > 0) // Water runs off
        {
            padded_data[padded_index] = -1;
        }
    }

    /***************
     * Z=0 BORDER*
     ***************/
    glm::vec3 minus_z_vector(0,0,-1);
#pragma omp parallel for
    for(int x = 0; x < terrain_width; x++)
    {
        int padded_index (x+1);
        glm::vec3 normal (terrain_normals(x,0));
        float dot_product (glm::dot(minus_z_vector, normal));

        if(dot_product > 0) // Water runs off
        {
            padded_data[padded_index] = -1;
        }
    }

    glm::vec3 plus_z_vector(0,0,1);
#pragma omp parallel for
    for(int x = 0; x < terrain_width; x++)
    {
        int padded_index (padded_terrain_width*(padded_terrain_depth-1)+x);
        glm::vec3 normal (terrain_normals(x,terrain_depth-1));
        float dot_product (glm::dot(plus_z_vector, normal));

        if(dot_product > 0) // Water runs off
        {
            padded_data[padded_index] = -1;
        }
    }

    setDimensions(padded_terrain_width, padded_terrain_depth);
    setData(padded_data);
}
