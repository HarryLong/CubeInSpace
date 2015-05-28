#ifndef ASSET_H
#define ASSET_H

#include "gl_drawable.h"
#include "glm/glm.hpp"
#include "glm/matrix.hpp"

class Asset : public GlDrawable
{
public:
    Asset(bool uniform_color, glm::vec4 color = glm::vec4(), glm::mat4x4 mtw_mat = glm::mat4x4(), float scale = 1.0f);
    virtual ~Asset();

    void setMtwMat(glm::mat4x4 mtw_mat);
    glm::mat4x4 getMtwMat() const;
    float getScale() const;
    void setScale(float scale);

    const glm::vec4 m_color;
    const bool m_uniform_color;

private:
    glm::mat4x4 m_mtw_mat;
    float m_scale;
};

#endif // ASSET_H
