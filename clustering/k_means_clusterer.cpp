#include "k_means_clusterer.h"
#include <algorithm>
#include "../gl_core/renderer.h"

void KMeansClusterer::perform_clustering(ResourceWrapper & resources, int k, Clusters & clusters, ClusterMembershipTexture & memberships,
                                         std::function<void(Clusters &, ResourceWrapper &, ClusterMembershipTexture &)> & cluster_membership_fn)
{
//    TerrainTemperatureTexture & jun_temp(resources.getTerrainTemp().getJun());
//    TerrainTemperatureTexture & dec_temp(resources.getTerrainTemp().getDec());

//    TerrainDailyIlluminationTexture & min_daily_illumination(resources.getDailyIllumination().getMin());
//    TerrainDailyIlluminationTexture & max_daily_illumination(resources.getDailyIllumination().getMax());

//    Slope & slope(resources.getSlope());

//    WeightedSoilHumidityHeightmap * soil_humidities[12];
//    for(int i = 0; i < 12; i++)
//    {
//        soil_humidities[i] = &resources.getWeightedSoilHumidity()[i+1];
//    }

//    int width(min_daily_illumination.width());
//    int height(min_daily_illumination.height());

//    // First generate the random points
//    int i(0);
//    {
//        int increments((int) std::sqrt(std::min(width,height)));
//        for(int x = 0; x < width && i < k; x+= increments)
//        {
//            for(int y = 0; y < height && i < k; y += increments, i++)
//            {
//                GLint temp_jun ( (GLint) jun_temp(x,y) );
//                GLint temp_dec ( (GLint) dec_temp(x,y) );

//                GLuint illumination_min ( (GLuint) min_daily_illumination(x,y) );
//                GLuint illumination_max ( (GLuint) max_daily_illumination(x,y) );

//                GLfloat position_slope (slope(x,y));

//                GLfloat humidities[12];
//                for(int i = 0; i < 12; i++)
//                {
//                    humidities[i] = (*soil_humidities[i])(x,y);
//                }

//                clusters.set(i, temp_jun, temp_dec, illumination_min, illumination_max, position_slope, humidities);
//            }
//        }
//    }



    // First get k points

}

