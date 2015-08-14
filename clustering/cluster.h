#ifndef CLUSTER_H
#define CLUSTER_H

#include <gl3.h>

#include "../resources/terrain_temperature.h"
#include "../resources/slope.h"
#include "../resources/weighted_soil_humidity.h"
#include "../resources/terrain_daily_illumination.h"

class Clusters {
public:
    struct ClusterData{
        GLfloat slope;
        GLint temperatures[2];
        GLuint illumination[2];
        GLfloat soil_humidities[12];
    };

    Clusters(int n_clusters);
    ~Clusters();

    int clusterCount();

    void set(int cluster_idx, ClusterData cluster_data);

    void clusters_finalised();

private:
    friend class Computer;
    TerrainTemperature m_temperature_cluster_data;
    Slope m_slope_cluster_data;
    WeightedSoilHumidity m_weighted_soil_humidity_cluster_data;
    TerrainDailyIllumination m_daily_illumination_cluster_data;

    int m_n_clusters;
};

#endif // CLUSTER_H
