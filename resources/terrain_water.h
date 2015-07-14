#ifndef TERRAIN_WATER_H
#define TERRAIN_WATER_H

#include "../gl_texture/texture_element.h"
#include "glm/common.hpp"
#include <atomic>
#include <mutex>

class TerrainWaterHeightmap : public TextureElement<GLuint> // TODO: And GL Drawable to actually draw the water bruuuu
{
public:
    TerrainWaterHeightmap();
    ~TerrainWaterHeightmap();
};

class TerrainWater{
public:
    struct MaterialProperties{
        static const glm::vec4 _DIFFUSE;
        static const glm::vec4 _SPECULAR;
        static const glm::vec4 _AMBIENT;
    };
    TerrainWater();
    ~TerrainWater();
    void bindDec();
    void bindJun();
    GLuint getDecTextureId();
    GLuint getJunTextureId();
    void pushToGPU();
    void setData(GLuint * jun_data, GLuint * dec_data, int width, int height);
    void syncFromGPU();
    void getWaterData(int x, int z, GLuint & jun, GLuint & dec);
    const GLuint * getJunRawData();
    const GLuint * getDecRawData();

    bool balanced();
    bool isBalancing();
    void setBalancing(bool balancing);
    void incrementBalancingIteration(int changes);

private:
    TerrainWaterHeightmap m_terrain_water_jun;
    TerrainWaterHeightmap m_terrain_water_dec;

    void perform_balancing_check();
    void set_balanced(bool balanced);
    std::atomic<bool> m_balanced;
    std::mutex m_balancing_mutex;
    std::atomic<bool> m_balancing;
    int m_balancing_iterations;
    std::vector<int> m_balance_iterations_changes;
    int m_balance_iterations_changes_total;
};

#endif // TERRAIN_WATER_H
