#ifndef TERRAIN_WATER_H
#define TERRAIN_WATER_H

#include "../gl_texture/texture_element.h"
#include "glm/common.hpp"
#include <atomic>
#include <mutex>

/***************************
 * TERRAIN WATER HEIGHTMAP *
 ***************************/
class TerrainWaterHeightmap : public TextureElement<GLfloat> // TODO: And GL Drawable to actually draw the water bruuuu
{
public:
    TerrainWaterHeightmap();
    ~TerrainWaterHeightmap();

    void incrementBalancingIteration(int changes);
    bool balanced();
    bool isBalancing();
    void setBalancing(bool balancing);
    void setBalanced(bool balanced);

private:
    void perform_balancing_check();

    std::atomic<bool> m_balanced;
    std::atomic<bool> m_balancing;
    int m_balancing_iterations;
    std::vector<int> m_balance_iterations_changes;
    int m_balance_iterations_changes_total;
};

/*****************
 * TERRAIN WATER *
 *****************/
class TerrainWater{
public:
    struct MaterialProperties{
        static const glm::vec4 _DIFFUSE;
        static const glm::vec4 _SPECULAR;
        static const glm::vec4 _AMBIENT;
    };
    TerrainWater();
    ~TerrainWater();
    bool balanced();
    void bind(int);
    TerrainWaterHeightmap& operator[](int);
    void setData(GLfloat * data[12], int width, int height);
    std::vector<TerrainWaterHeightmap*> getUnbalanced();
    void reset(int width, int height);

private:
    TerrainWaterHeightmap m_terrain_water[12];
};

#endif // TERRAIN_WATER_H
