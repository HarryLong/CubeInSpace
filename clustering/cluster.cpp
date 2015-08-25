#include "cluster.h"

Clusters::Clusters(int n_clusters) : m_n_clusters(n_clusters)
{
    m_temperature_cluster_data.reset(n_clusters, 1);

    m_slope_cluster_data.reset(n_clusters, 1);

    m_daily_illumination_cluster_data.reset(n_clusters, 1);

    m_weighted_soil_humidity_cluster_data.reset(n_clusters, 1);
}

Clusters::~Clusters()
{

}

void Clusters::push_to_gpu()
{
    m_temperature_cluster_data.pushToGPU();
    m_slope_cluster_data.pushToGPU();
    m_daily_illumination_cluster_data.pushToGPU();
    m_weighted_soil_humidity_cluster_data.pushToGPU();
}

void Clusters::sync_from_gpu()
{
    m_temperature_cluster_data.syncFromGPU();
    m_slope_cluster_data.syncFromGPU();
    m_daily_illumination_cluster_data.syncFromGPU();
    m_weighted_soil_humidity_cluster_data.syncFromGPU();
}

int Clusters::clusterCount() const
{
    return m_n_clusters;
}

void Clusters::set(int cluster, ClusterData cluster_data)
{
    m_slope_cluster_data.set(cluster_data.slope, cluster, 0);
    for(int i(0); i < 2 ;i++)
    {
        m_daily_illumination_cluster_data.set(i, cluster_data.illumination[i], cluster, 0);
        m_temperature_cluster_data.set(i, cluster_data.temperatures[i], cluster, 0);
    }
    for(int i = 0; i < 12; i++)
        m_weighted_soil_humidity_cluster_data.set(i, cluster_data.soil_humidities[i], cluster, 0);
}

ClusterData Clusters::getClusterData(int cluster_idx) const
{
    ClusterData cluster_data;

    cluster_data.slope = m_slope_cluster_data(cluster_idx,0);

    for(int i(0); i < 2; i++)
    {
        cluster_data.temperatures[i] = (GLint) m_temperature_cluster_data(i, cluster_idx,0);
        cluster_data.illumination[i] = (GLuint) m_daily_illumination_cluster_data(i, cluster_idx,0);
    }

    for(int i = 0; i < 12; i++)
    {
        cluster_data.soil_humidities[i] = m_weighted_soil_humidity_cluster_data(i,cluster_idx,0);
    }

    cluster_data.member_count = m_memberships_count.at(cluster_idx);

    return cluster_data;
}

void Clusters::summarize()
{
    qCritical() << "*** CLUSTERS ***";
    for(int i(0); i < m_n_clusters; i++)
    {
        qCritical() << "*** CLUSTER " << i << " ***";
        qCritical() << "Slope: " << m_slope_cluster_data(i,0);
        qCritical() << "Temp (Jun): " << ((GLint) m_temperature_cluster_data(0,i,0));
        qCritical() << "Temp (Dec): " << ((GLint) m_temperature_cluster_data(1,i,0));
        qCritical() << "Illumination (min): " << ((GLuint) m_daily_illumination_cluster_data(0,i,0));
        qCritical() << "Illumination (max): " << ((GLuint) m_daily_illumination_cluster_data(1,i,0));
        for(int l = 0; l < 12; l++)
        {
            qCritical() << "Soil Humidity [" << l << "]: " << m_weighted_soil_humidity_cluster_data(l,i,0);
        }
    }
}

int Clusters::getMemberCount(int cluster_idx) const
{
    return m_memberships_count.at(cluster_idx);
}

void Clusters::setMembershipCount(int cluster_idx, int member_count)
{
    m_memberships_count[cluster_idx] = member_count;
}
