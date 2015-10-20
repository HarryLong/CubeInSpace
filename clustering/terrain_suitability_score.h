#ifndef TERRAIN_SUITABILITY_SCORE_H
#define TERRAIN_SUITABILITY_SCORE_H

#include <vector>
#include "plantDB/plant_properties.h"
#include "cluster_data.h"

class TerrainSuitabilityScore{
public:
    class Score{
    public:
        Score();
        int slope;
        int illumination[12];
        int min_illumination;
        int max_illumination;
        float avg_illumination;

        int temperature[12];
        int min_temp;
        int max_temp;
        float avg_temp;

        int soil_humidities[12];
        int min_humidity;
        int max_humidity;
        float avg_humidity;

        float aggregate_avg;
    };

    TerrainSuitabilityScore(const SpecieProperties & specie_properties, std::vector<ClusterData> cluster_data);
    TerrainSuitabilityScore();
    ~TerrainSuitabilityScore();
    std::vector<int> getValidClusters();
    int n_clusters() const;
    float getMaxSuitabilityScore() const;
    int getMaxSuitabilityClusterIdx() const;
    float getMinSuitabilityScore() const;
    int getMinSuitabilityClusterIdx() const;
    Score operator[](int cluster_idx);

private:
    int calculate_lower_end_score(int value, int min, int prime_start);
    int calculate_upper_end_score(int value, int max, int prime_end);
    int getMin(std::vector<int> values);
    Score calculate_score(const SpecieProperties & specie_properties, const ClusterData & cluster_data);
    std::vector<Score> m_per_cluster_scores;
    std::vector<int> m_valid_clusters;
    int m_max_suitability_score;
    int m_max_suitability_cluster_idx;
    int m_min_suitability_score;
    int m_min_suitability_cluster_idx;
};

#endif
