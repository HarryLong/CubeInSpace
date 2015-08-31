#ifndef K_MEANS_CLUSTERER_H
#define K_MEANS_CLUSTERER_H

#include "../resources/resource_wrapper.h"

#include <map>
#include <glm/common.hpp>
#include "cluster_membership_texture.h"
#include "cluster.h"
#include <functional>
#include <QObject>
#include "cluster_data.h"

class KMeansClusterer : public QObject
{
Q_OBJECT
public:
    KMeansClusterer();
    ~KMeansClusterer();

    void perform_clustering(int k, ResourceWrapper & resources, Clusters & clusters, ClusterMembershipTexture & memberships,
                            float monthly_temperature_change,
                            std::function<void(Clusters &, ResourceWrapper &, ClusterMembershipTexture &,int)> & clustering_fn);

signals:
    void clustering_start(QString);
    void clustering_update(int);
    void clustering_complete();

private:
    std::vector<ClusterData> recalculate_cluster_means(int n_clusters, ClusterMembershipTexture & memberships,
                                                                 TerrainTemperature & temp_data,
                                                                 TerrainDailyIllumination & illumination_data,
                                                                 Slope & slope_data,
                                                                 WeightedSoilHumidity & weighted_soil_humidity_data);

    bool containsCluster(std::vector<ClusterData> & all_clusters, ClusterData & query_cluster);
};

#endif // K_MEANS_CLUSTERER_H
