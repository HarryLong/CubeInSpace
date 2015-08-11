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
    static void perform_clustering(ResourceWrapper & resources, int k, Clusters & clusters, ClusterMembershipTexture & memberships,
                                   std::function<void(Clusters &, ResourceWrapper &, ClusterMembershipTexture &)> & cluster_membership_fn);
};

#endif // K_MEANS_CLUSTERER_H
