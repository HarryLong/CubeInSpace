#include "cluster.h"

bool Clusters::ClusterData::operator==(const ClusterData & other)
{
    if(slope != other.slope)
        return false;

    if(temperatures != other.temperatures)
        return false;

    for(int i(0); i < 2; i++)
    {
        if(illumination[i] != other.illumination[i])
            return false;
    }

    for(int i(0); i < 12; i++)
    {
        if(soil_humidities[i] != other.soil_humidities[i])
            return false;
    }

    return true;
}

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

int Clusters::clusterCount()
{
    return m_n_clusters;
}

void Clusters::set(int cluster, ClusterData cluster_data)
{
    m_temperature_cluster_data.set(0, cluster_data.temperatures, cluster, 0);

    m_slope_cluster_data.set(cluster_data.slope, cluster, 0);

//    m_daily_illumination_cluster_data.set(0, cluster_data.illumination[0], cluster, 0);
//    m_daily_illumination_cluster_data.set(1, cluster_data.illumination[1], cluster, 0);

    for(int i = 0; i < 12; i++)
        m_weighted_soil_humidity_cluster_data.set(i, cluster_data.soil_humidities[i], cluster, 0);
}

void Clusters::summarize()
{
    qCritical() << "*** CLUSTERS ***";
    for(int i(0); i < m_n_clusters; i++)
    {
        qCritical() << "*** CLUSTER " << i << " ***";
        qCritical() << "Slope: " << m_slope_cluster_data(i,0);
        qCritical() << "Temp: " << ((GLint) m_temperature_cluster_data(0,i,0));
//        qCritical() << "Illumination (min): " << ((GLuint) m_daily_illumination_cluster_data(0,i,0));
//        qCritical() << "Illumination (max): " << ((GLuint) m_daily_illumination_cluster_data(1,i,0));
        for(int l = 0; l < 12; l++)
        {
            qCritical() << "Soil Humidity [" << l << "]: " << m_weighted_soil_humidity_cluster_data(l,i,0);
        }
    }
}
