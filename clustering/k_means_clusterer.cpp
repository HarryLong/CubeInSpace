#include "k_means_clusterer.h"
#include <algorithm>
#include "../gl_core/renderer.h"

void KMeansClusterer::perform_clustering(int k, ResourceWrapper & resources, Clusters & clusters, ClusterMembershipTexture & memberships,
                                         std::function<void(Clusters &, ResourceWrapper &, ClusterMembershipTexture &)> & cluster_membership_fn)
{
    TerrainTemperature & temp_data(resources.getTerrainTemp());
    TerrainDailyIllumination & illumination_data(resources.getDailyIllumination());
    Slope & slope_data(resources.getSlope());
    WeightedSoilHumidity & weighted_soil_humidity_data(resources.getWeightedSoilHumidity());

    int width(temp_data.width());
    int height(temp_data.height());

    // First generate k random clusters
    int cluster_idx(0);
    {
        int increments((int) std::sqrt(std::min(width,height)));
        for(int x = 0; x < width && cluster_idx < k; x+= increments)
        {
            for(int y = 0; y < height && cluster_idx < k; y += increments, cluster_idx++)
            {
                Clusters::ClusterData cluster_data;
                for(int i = 0; i < 2; i++)
                {
                    cluster_data.temperatures[i] = temp_data(i,x,y);
                    cluster_data.illumination[i] = illumination_data(i, x, y);
                }
                cluster_data.slope = slope_data(x,y) ;
                for(int i = 0; i < 12; i++)
                {
                    cluster_data.soil_humidities[i] = weighted_soil_humidity_data(i, x, y);
                }
                clusters.set(cluster_idx, cluster_data);
            }
        }
        clusters.clusters_finalised(); // Pushes data to the GPU
    }

    for(int i = 0; i < 100; i++)
    {
        cluster_membership_fn(clusters, resources, memberships);
        // Sync memberships from GPU
        memberships.syncFromGPU();

        std::vector<Clusters::ClusterData> cluster_data(recalculate_cluster_means(k, memberships, temp_data, illumination_data,
                                                                                  slope_data, weighted_soil_humidity_data));

        // Update cluster data on GPU
        for(int cluster_idx(0); cluster_idx < k; cluster_idx++)
        {
            clusters.set(cluster_idx, cluster_data[cluster_idx]);
        }
        clusters.clusters_finalised(); // Pushes data to GPU
    }

}

std::vector<Clusters::ClusterData> KMeansClusterer::recalculate_cluster_means(int n_clusters, ClusterMembershipTexture & memberships,
                                                                              TerrainTemperature & temp_data,
                                                                              TerrainDailyIllumination & illumination_data,
                                                                              Slope & slope_data,
                                                                              WeightedSoilHumidity & weighted_soil_humidity_data)
{
    std::vector<int> n_members;
    std::vector<Clusters::ClusterData> cluster_means;

    for(int i(0); i < n_clusters; i++)
    {
        cluster_means.push_back(Clusters::ClusterData());
        n_members.push_back(0);
    }

    for(int x = 0; x < memberships.width(); x++)
    {
        for(int y = 0; y < memberships.height(); y++)
        {
            int cluster_idx(memberships(x,y));
            n_members[cluster_idx]++;

            Clusters::ClusterData & cluster_data(cluster_means[cluster_idx]);
            for(int i = 0; i < 2; i++)
            {
                cluster_data.temperatures[i] += temp_data(i, x, y);
                cluster_data.illumination[i] += illumination_data(i, x, y);
            }
            cluster_data.slope += slope_data(x, y);
            for(int i = 0; i < 12; i++)
            {
                cluster_data.soil_humidities[i] += weighted_soil_humidity_data(i, x, y);
            }
        }
    }

    // Now average out
    for(int cluster_idx (0); cluster_idx < n_clusters; cluster_idx++)
    {
        Clusters::ClusterData & cluster_data(cluster_means[cluster_idx]);
        int dividor(n_members[cluster_idx]);
        for(int i = 0; i < 2; i++)
        {
            cluster_data.temperatures[i] /= dividor;
            cluster_data.illumination[i] /= dividor;
        }
        cluster_data.slope /= dividor;
        for(int i = 0; i < 12; i++)
        {
            cluster_data.soil_humidities[i] /= dividor;
        }
    }
}
