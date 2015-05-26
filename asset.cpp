#include "asset.h"

Asset::Asset(bool uniform_color, glm::vec4 color, glm::mat4x4 mtw_mat, float scale) :
    m_uniform_color(uniform_color), m_color(color), m_mtw_mat(mtw_mat), m_scale(scale)
{

}

Asset::~Asset()
{

}

void Asset::setMtwMat(glm::mat4x4 mtw_mat)
{
    m_mtw_mat = mtw_mat;
}

glm::mat4x4 Asset::getMtwMat() const
{
    return m_mtw_mat;
}

float Asset::getScale() const
{
    return m_scale;
}

void Asset::setScale(float scale)
{
    m_scale = scale;
}
