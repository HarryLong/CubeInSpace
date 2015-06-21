#ifndef ASSET_H
#define ASSET_H

#include "gl_drawable.h"
#include "glm/glm.hpp"
#include "glm/matrix.hpp"

#include <vector>

class Asset : public GlDrawable
{
public:
    struct AssetTransformations{
    public:
        AssetTransformations(glm::mat4x4 mtw, float scale) : mtw(mtw), scale(scale) {}
        float scale;
        glm::mat4x4 mtw;
    };

    Asset(bool uniform_color, glm::vec4 color = glm::vec4(), glm::mat4x4 mtw_mat = glm::mat4x4(), float scale = 1.0f);
    virtual ~Asset();

    void addTransformation(glm::mat4x4 mtw, float scale = 1.0f);
    void setTranformation(glm::mat4x4 mtw, float scale = 1.0f);
    const std::vector<Asset::AssetTransformations> & getTransformations();
    void clearTransformations();

    const glm::vec4 m_color;
    const bool m_uniform_color;

private:
    std::vector<Asset::AssetTransformations> m_asset_transformations;
};

#endif // ASSET_H
