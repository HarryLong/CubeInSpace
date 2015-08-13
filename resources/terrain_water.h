#ifndef TERRAIN_WATER_H
#define TERRAIN_WATER_H

#include "../gl_texture/texture_element_2d_array.h"
#include "glm/common.hpp"
#include <atomic>
#include <mutex>

/*******************
 * BALANCE CHECKER *
 *******************/
class BalanceChecker{
public:
    BalanceChecker();
    ~BalanceChecker();

    void reset();
    bool isBalanced();
    bool incrementBalancingIteration(int changes);
private:
    int m_balance_iterations_changes_total;
    std::vector<int> m_balance_iterations_changes;
};

/*****************
 * TERRAIN WATER *
 *****************/
class TerrainWater : public TextureElement2DArray<GLfloat>{
public:
    struct MaterialProperties{
        static const glm::vec4 _DIFFUSE;
        static const glm::vec4 _SPECULAR;
        static const glm::vec4 _AMBIENT;
    };

    TerrainWater();
    ~TerrainWater();

    bool incrementBalancingIteration(int month, int changes);
    bool balanced();
    bool balanced(int month);
    void setUnbalanced();

private:
    std::vector<BalanceChecker> m_balance_checkers;
};

#endif // TERRAIN_WATER_H
