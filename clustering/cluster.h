#ifndef CLUSTER_H
#define CLUSTER_H

#include "cluster_data.h"
#include "../resources/terrain_temperature.h"
#include "../resources/slope.h"
#include "../resources/weighted_soil_humidity.h"
#include "../resources/terrain_daily_illumination.h"

class Clusters {
public:
    Clusters(int n_clusters);
    ~Clusters();

    int clusterCount() const;

    void set(int cluster_idx, ClusterData cluster_data);

    void push_to_gpu();
    void sync_from_gpu();

    void summarize();

    void setMembershipCount(int cluster_idx, int member_count);

    int getMemberCount(int cluster_idx) const;

    ClusterData getClusterData(int cluster_idx) const;
private:
    friend class Computer;
    TerrainTemperature m_temperature_cluster_data;
    Slope m_slope_cluster_data;
    WeightedSoilHumidity m_weighted_soil_humidity_cluster_data;
    TerrainDailyIllumination m_daily_illumination_cluster_data;

    int m_n_clusters;
    std::map<int, int> m_memberships_count;
};

#endif // CLUSTER_H
