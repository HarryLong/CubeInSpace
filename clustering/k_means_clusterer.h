#ifndef K_MEANS_CLUSTERER_H
#define K_MEANS_CLUSTERER_H

#include "../resources/resource_wrapper.h"

#include <map>
#include <glm/common.hpp>
#include "cluster_membership_texture.h"
#include "cluster.h"
#include <functional>


class KMeansClusterer
{
public:
    static void perform_clustering(int k, ResourceWrapper & resources, Clusters & clusters, ClusterMembershipTexture & memberships,
                                   std::function<void(Clusters &, ResourceWrapper &, ClusterMembershipTexture &)> & cluster_membership_fn);

private:
    static std::vector<Clusters::ClusterData> recalculate_cluster_means(int n_clusters, ClusterMembershipTexture & memberships,
                                                                        TerrainTemperature & temp_data,
                                                                        TerrainDailyIllumination & illumination_data,
                                                                        Slope & slope_data,
                                                                        WeightedSoilHumidity & weighted_soil_humidity_data);
};

#endif // K_MEANS_CLUSTERER_H
