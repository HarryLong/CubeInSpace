#include "k_means_clusterer.h"
#include <algorithm>
#include "../gl_core/renderer.h"

KMeansClusterer::KMeansClusterer()
{

}

KMeansClusterer::~KMeansClusterer()
{

}

void KMeansClusterer::perform_clustering(int k, ResourceWrapper & resources, Clusters & clusters, ClusterMembershipTexture & memberships,
                                         float monthly_temperature_change,
                                         std::function<void(Clusters &, ResourceWrapper &, ClusterMembershipTexture &,int)> & clustering_fn)
{
    QString msg("Performing K Means Clustering...");
    msg.append("[ k=").append(QString::number(k)).append("]");
    emit clustering_start(msg);

    TerrainTemperature & temp_data(resources.getTerrainTemp());
    TerrainDailyIllumination & illumination_data(resources.getDailyIllumination());
    Slope & slope_data(resources.getSlope());
    WeightedSoilHumidity & weighted_soil_humidity_data(resources.getWeightedSoilHumidity());

    int width(temp_data.width());
    int height(temp_data.height());

    // First generate k random clusters
    {
        std::vector<ClusterData> all_clusters;
        int min_side(std::min(width, height));
        float diagonal_length (std::sqrt(2*min_side*min_side));
        float diagonal_increments(diagonal_length/k);
        int x_y_increments(std::sqrt((diagonal_increments*diagonal_increments)/2));

        int x_y (0);
        do{
            ClusterData cluster_data;
            cluster_data.slope = slope_data(x_y,x_y) ;
            for(int i = 0; i < 12; i++)
            {
                cluster_data.illumination[i] = (GLuint) illumination_data(i, x_y, x_y);
                cluster_data.soil_humidities[i] = weighted_soil_humidity_data(i, x_y, x_y);
                cluster_data.temperatures[i] = (GLint) temp_data(i,x_y,x_y);
            }

            int increment(1);
            while(containsCluster(all_clusters, cluster_data))
            {
                qCritical() << "Contains cluster!";
                int multiplier(all_clusters.size() % 2 == 0 ? 1 : -1);

                cluster_data.slope += (multiplier * increment);
                cluster_data.slope = std::min(90.f, std::max(0.f, cluster_data.slope));

                for(int i = 0; i < 12; i++)
                {
                    int tmp_illumination = std::min(24, std::max(0, ((int)cluster_data.illumination[i]) + (multiplier * increment)));
                    cluster_data.illumination[i] = (GLuint)tmp_illumination;
                    cluster_data.soil_humidities[i] += (multiplier * increment);
                    cluster_data.soil_humidities[i] = std::max(0.f, cluster_data.soil_humidities[i]);
                    cluster_data.temperatures[i] = std::max(0, cluster_data.temperatures[i] + (multiplier*increment));
                }
                increment++;
            }
            all_clusters.push_back(cluster_data);
            x_y += x_y_increments;
        }while(all_clusters.size() < k);
        for(int cluster_idx (0); cluster_idx < k; cluster_idx++)
        {
            clusters.set(cluster_idx, all_clusters.at(cluster_idx));
        }
        clusters.push_to_gpu(); // Pushes data to the GPU
    }

    clusters.summarize();

    clustering_fn(clusters, resources, memberships, 50); // Perform clustering
    memberships.syncFromGPU();
    clusters.sync_from_gpu();

//    qCritical() << "Memberships [" << memberships.width() << "," << memberships.height() << "]";

//    int total_iterations(100);

//    for(int i = 0; i < total_iterations; i++)
//    {
//        emit clustering_update((((float)i)/total_iterations) * 100);
//        cluster_membership_fn(clusters, resources, memberships);
        // Sync memberships from GPU
//        memberships.syncFromGPU();
//        std::vector<ClusterData> cluster_data(recalculate_cluster_means(k, memberships, temp_data, illumination_data,
//                                                                                  slope_data, weighted_soil_humidity_data));
//        // Update cluster data on GPU
//        for(int cluster_idx(0); cluster_idx < k; cluster_idx++)
//        {
//            clusters.set(cluster_idx, cluster_data[cluster_idx]);
//        }
//        clusters.clusters_finalised(); // Pushes data to GPU

//        clusters.summarize();
//    }

    std::vector<int> n_members;
    for(int i(0); i < k; i++)
    {
        n_members.push_back(0);
    }
    for(int x = 0; x < memberships.width(); x++)
    {
        for(int y = 0; y < memberships.height(); y++)
        {
            int cluster_idx(memberships(x,y));
            n_members[cluster_idx]++;
        }
    }
    for(int i(0); i < k; i++)
    {
        clusters.setMembershipCount(i, n_members[i]);
    }

    // Set the remaining temperatures
    for(int cluster_idx(0); cluster_idx < k; cluster_idx++)
    {
        ClusterData cluster_data(clusters.getClusterData(cluster_idx));
        int jun_temp(cluster_data.temperatures[5]);
        for(int month_diff(1); month_diff < 7; month_diff++)
        {
            float temp(jun_temp+(month_diff*monthly_temperature_change));

            if(month_diff < 6)
                cluster_data.temperatures[5-month_diff] = temp;

            cluster_data.temperatures[5+month_diff] = temp;
        }
        clusters.set(cluster_idx, cluster_data);
    }

//    qCritical() << "*** FINAL CLUSTERS ***";
//    clusters.summarize();
//    qCritical() << "*** MEMBERSHIP COUNT ***";
//    for(int i(0); i < k; i++)
//    {
//        qCritical() << i << " --> " << n_members[i];
//    }

    emit clustering_complete();
}

std::vector<ClusterData> KMeansClusterer::recalculate_cluster_means(int n_clusters, ClusterMembershipTexture & memberships,
                                                                              TerrainTemperature & temp_data,
                                                                              TerrainDailyIllumination & illumination_data,
                                                                              Slope & slope_data,
                                                                              WeightedSoilHumidity & weighted_soil_humidity_data)
{
    std::vector<int> n_members;
    std::vector<ClusterData> cluster_means;

    for(int i(0); i < n_clusters; i++)
    {
        cluster_means.push_back(ClusterData());
        n_members.push_back(0);
    }

    for(int x = 0; x < memberships.width(); x++)
    {
        for(int y = 0; y < memberships.height(); y++)
        {
            int cluster_idx(memberships(x,y));
            n_members[cluster_idx]++;

            ClusterData & cluster_data(cluster_means[cluster_idx]);
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
        int cluster_n_members(n_members[cluster_idx]);
        ClusterData & cluster_data(cluster_means[cluster_idx]);

        if(cluster_n_members > 0)
        {
            for(int i = 0; i < 2; i++)
            {
                cluster_data.temperatures[i] /= cluster_n_members;
                cluster_data.illumination[i] /= cluster_n_members;
            }
            cluster_data.slope /= cluster_n_members;
            for(int i = 0; i < 12; i++)
            {
                cluster_data.soil_humidities[i] /= cluster_n_members;
            }
        }
        else
        {
            for(int i = 0; i < 2; i++)
            {
                cluster_data.temperatures[i] = 0;
                cluster_data.illumination[i] = 0;
            }
            cluster_data.slope = 0;
            for(int i = 0; i < 12; i++)
            {
                cluster_data.soil_humidities[i] = 0;
            }
        }
    }

    return cluster_means;
}

bool KMeansClusterer::containsCluster(std::vector<ClusterData> & all_clusters, ClusterData & query_cluster)
{
    bool found_equal(false);

    for(int i(0); i < all_clusters.size() && !found_equal; i++)
    {
        found_equal = (all_clusters.at(i) == query_cluster);
    }

    return found_equal;
}
