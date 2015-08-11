#ifndef CLUSTER_H
#define CLUSTER_H

#include <gl3.h>

class Clusters {
public:
    Clusters(int n_clusters);
    ~Clusters();

    int clusterCount();

    void set(int cluster, GLint jun_temperature, GLint dec_temperature, GLuint min_illumination, GLuint max_illumination,
              GLfloat slope, GLfloat soil_humidities[12]);

    static const int _MAX_CLUSTERS = 50;

    GLint m_jun_temperature[Clusters::_MAX_CLUSTERS];
    GLint m_dec_temperature[Clusters::_MAX_CLUSTERS];

    GLuint m_min_illuminations[Clusters::_MAX_CLUSTERS];
    GLuint m_max_illuminations[Clusters::_MAX_CLUSTERS];

    GLfloat m_slopes[Clusters::_MAX_CLUSTERS];

    GLfloat m_soil_humidities[12][Clusters::_MAX_CLUSTERS];

private:
    int m_n_clusters;
};

#endif // CLUSTER_H
