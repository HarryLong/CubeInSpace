#ifndef WATER_FLOW_ANALYSER_H
#define WATER_FLOW_ANALYSER_H

#include "asset.h"
#include <glm/common.hpp>
#include "terragen_file_manager/terragen_file.h"

/********************
 * WATER FLOW ASSET *
 ********************/
class WaterFlowAsset : public Asset
{
public:
    WaterFlowAsset();
    ~WaterFlowAsset();
    virtual void render();

    void add(const glm::vec3 & start_point, const glm::vec3 & end_point);

protected:
    virtual void initGL();

private:
};


/***********************
 * WATER FLOW ANALYZER *
 ***********************/
class WaterFlowAnalyzer{
public:
    WaterFlowAnalyzer();
    ~WaterFlowAnalyzer();

    void generateWaterFlow(const TerragenFile & height_map, glm::vec2 starting_point);

    Asset * asAsset();
private:
    bool get_lowest_neighbour(const TerragenFile & height_map, glm::vec2 & reference_point, glm::vec2 & lowest_neighbour);
    WaterFlowAsset * m_water_flow;
};

#endif // WATER_FLOW_ANALYSER_H
