#include "cluster.h"

Clusters::Clusters(int n_clusters) : m_n_clusters(n_clusters)
{
    m_temperature_cluster_data.setDimensions(n_clusters, 1);
    m_slope_cluster_data.setDimensions(n_clusters, 1);
    m_daily_illumination_cluster_data.setDimensions(n_clusters, 1);
    m_weighted_soil_humidity_cluster_data.setDimensions(n_clusters, 1);
}

Clusters::~Clusters()
{

}

void Clusters::clusters_finalised()
{
    m_temperature_cluster_data.pushToGPU();
    m_slope_cluster_data.pushToGPU();
    m_daily_illumination_cluster_data.pushToGPU();
    m_weighted_soil_humidity_cluster_data.pushToGPU();
}

int Clusters::clusterCount()
{
    return m_n_clusters;
}

void Clusters::set(int cluster, ClusterData cluster_data)
{
    m_temperature_cluster_data.set(0, cluster_data.temperatures[0], cluster, 0);
    m_temperature_cluster_data.set(1, cluster_data.temperatures[1], cluster, 0);

    m_slope_cluster_data.set(cluster_data.slope, cluster, 0);

    m_daily_illumination_cluster_data.set(0, cluster_data.illumination[0], cluster, 0);
    m_daily_illumination_cluster_data.set(1, cluster_data.illumination[1], cluster, 0);

    for(int i = 0; i < 12; i++)
        m_weighted_soil_humidity_cluster_data.set(i, cluster_data.soil_humidities[i], cluster, 0);
}
