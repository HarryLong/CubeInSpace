#include "terrain_water.h"
#include <cstring>

/*******************
 * BALANCE CHECKER *
 *******************/
BalanceChecker::BalanceChecker()
{

}

BalanceChecker::~BalanceChecker()
{

}

void BalanceChecker::reset()
{
    m_balance_iterations_changes_total = 0;
    m_balance_iterations_changes.clear();
}

bool BalanceChecker::isBalanced()
{
    int iteration_idx(m_balance_iterations_changes.size()-1);

    if(iteration_idx > 0 && m_balance_iterations_changes.at(iteration_idx) <= (m_balance_iterations_changes_total/1000.0f))
    {
        return true;
    }

    return false;
}

bool BalanceChecker::incrementBalancingIteration(int changes)
{
    m_balance_iterations_changes.push_back(changes);
    m_balance_iterations_changes_total += changes;

    return isBalanced();
}

/*****************
 * TERRAIN WATER *
 *****************/
const glm::vec4 TerrainWater::MaterialProperties::_DIFFUSE = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_SPECULAR = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_AMBIENT = glm::vec4(0.f, 0.f, 1.f, 1.0f);
TerrainWater::TerrainWater() :
    TextureElement2DArray<GLfloat>(12, QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{
    for(int i = 0; i < 12; i++)
        m_balance_checkers.push_back(BalanceChecker());
}

TerrainWater::~TerrainWater()
{

}

bool TerrainWater::incrementBalancingIteration(int month, int changes)
{
    return m_balance_checkers[month-1].incrementBalancingIteration(changes);
}

bool TerrainWater::balanced()
{
    for(BalanceChecker & balance_checker: m_balance_checkers)
    {
        if(!balance_checker.isBalanced())
            return false;
    }

    return true;
}

bool TerrainWater::balanced(int month)
{
    return m_balance_checkers[month-1].isBalanced();
}

void TerrainWater::setUnbalanced()
{
    for(BalanceChecker & balance_checker: m_balance_checkers)
    {
        balance_checker.reset();
    }
}

