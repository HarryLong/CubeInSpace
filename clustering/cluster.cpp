#include "cluster.h"

Clusters::Clusters(int n_clusters) : m_n_clusters(n_clusters)
{
}

Clusters::~Clusters()
{

}

int Clusters::clusterCount()
{
    return m_n_clusters;
}

void Clusters::set(int cluster, GLint jun_temperature, GLint dec_temperature, GLuint min_illumination, GLuint max_illumination,
         GLfloat slope, GLfloat soil_humidities[12])
{
    m_jun_temperature[cluster] = jun_temperature;
    m_dec_temperature[cluster] = dec_temperature;

    m_slopes[cluster] = slope;

    m_min_illuminations[cluster] = min_illumination;
    m_max_illuminations[cluster] = max_illumination;

    for(int i = 0; i < 12; i++)
        m_soil_humidities[i][cluster] = soil_humidities[i];
}
