#include "terrain_suitability_score.h"

TerrainSuitabilityScore::Score::Score() :
    slope(0),
    min_illumination(0),
    max_illumination(0),
    avg_illumination(0),
    min_temp(0),
    max_temp(0),
    avg_temp(0),
    min_humidity(0),
    max_humidity(0),
    avg_humidity(0),
    valid(false)
{}

TerrainSuitabilityScore::TerrainSuitabilityScore(const SpecieProperties & specie_properties, std::vector<ClusterData> cluster_data)
{
    for(int cluster_idx(0); cluster_idx < cluster_data.size(); cluster_idx++)
    {
        ClusterData & cluster(cluster_data.at(cluster_idx));
        TerrainSuitabilityScore::Score cluster_score( calculate_score(specie_properties, cluster) );
        m_per_cluster_scores.push_back(cluster_score);

        if(cluster_score.valid)
        {
            m_valid_clusters.push_back(cluster_idx);
        }

        if(cluster_idx == 0 || cluster_score.aggregate_avg < m_min_suitability_score)
        {
            m_min_suitability_score = cluster_score.aggregate_avg;
            m_min_suitability_cluster_idx = cluster_idx;
        }
        if(cluster_idx == 0 || cluster_score.aggregate_avg > m_max_suitability_score)
        {
            m_max_suitability_score = cluster_score.aggregate_avg;
            m_max_suitability_cluster_idx = cluster_idx;
        }
    }
}

TerrainSuitabilityScore::TerrainSuitabilityScore()
{

}

TerrainSuitabilityScore::~TerrainSuitabilityScore()
{

}
int TerrainSuitabilityScore::getMin(std::vector<int> values)
{
    int min(values.at(0));

    for(int i(1); i < values.size(); i++)
        min = std::min(min, values.at(i));

    return min;
}

int TerrainSuitabilityScore::calculate_lower_end_score(int value, int min, int prime_start)
{
    return std::round(( ((float)value-min) / (prime_start-min)) * 100 );
}

int TerrainSuitabilityScore::calculate_upper_end_score(int value, int max, int prime_end)
{
    return std::round( (1 - ( ((float)value-prime_end) / (max-prime_end))) * 100 );
}

TerrainSuitabilityScore::Score TerrainSuitabilityScore::calculate_score(const SpecieProperties & specie_properties, const ClusterData & cluster_data)
{
    TerrainSuitabilityScore::Score cluster_score;

    for(int i = 0; i < 12; i++)
    {
        int illumination(cluster_data.illumination[i]);
        int soil_humidity(cluster_data.soil_humidities[i]);
        int temperature(cluster_data.temperatures[i]);

        // Illumination
        {
            const IlluminationProperties & properties(specie_properties.illumination_properties);
            int score = 0;
            if(illumination >= properties.prime_illumination.first &&
                    illumination <= properties.prime_illumination.second)
            {
                score = 100;
            }
            else if(illumination < properties.prime_illumination.first &&
                    illumination >= properties.min_illumination)
            {
                score = calculate_lower_end_score(illumination, properties.min_illumination, properties.prime_illumination.first);
            }
            else if(illumination > properties.prime_illumination.second &&
                    illumination <= properties.max_illumination)
            {
                score = calculate_upper_end_score(illumination, properties.max_illumination, properties.prime_illumination.second);
            }
            cluster_score.illumination[i] = score;
        }

        // Soil Humidity
        {
            const SoilHumidityProperties & properties(specie_properties.soil_humidity_properties);
            int score = 0;
            if(soil_humidity >= properties.prime_soil_humidity.first &&
                    soil_humidity <= properties.prime_soil_humidity.second)
            {
                score = 100;
            }
            else if(soil_humidity < properties.prime_soil_humidity.first &&
                    soil_humidity >= properties.min_soil_humidity)
            {
                score = calculate_lower_end_score(soil_humidity, properties.min_soil_humidity, properties.prime_soil_humidity.first);
            }
            else if(soil_humidity > properties.prime_soil_humidity.second &&
                    soil_humidity <= properties.max_soil_humidity)
            {
                score = calculate_upper_end_score(soil_humidity, properties.max_soil_humidity, properties.prime_soil_humidity.second);
            }
            cluster_score.soil_humidities[i] = score;
        }

        // Temperature
        {
            const TemperatureProperties & properties(specie_properties.temperature_properties);
            int score = 0;
            if(temperature >= properties.prime_temp.first &&
                    temperature <= properties.prime_temp.second)
            {
                score = 100;
            }
            else if(temperature < properties.prime_temp.first &&
                    temperature >= properties.min_temp)
            {
                score = calculate_lower_end_score(temperature, properties.min_temp, properties.prime_temp.first);
            }
            else if(temperature > properties.prime_temp.second &&
                    temperature <= properties.max_temp)
            {
                score = calculate_upper_end_score(temperature, properties.max_temp, properties.prime_temp.second);
            }
            cluster_score.temperature[i] = score;
        }

        // Aggregates
        cluster_score.avg_temp += cluster_score.temperature[i];
        if(i == 0 || cluster_score.temperature[i] < cluster_score.min_temp)
            cluster_score.min_temp = cluster_score.temperature[i];
        if(i == 0 || cluster_score.temperature[i] > cluster_score.max_temp)
            cluster_score.max_temp = cluster_score.temperature[i];

        cluster_score.avg_illumination += cluster_score.illumination[i];
        if(i == 0 || cluster_score.illumination[i] < cluster_score.min_illumination)
            cluster_score.min_illumination = cluster_score.illumination[i];
        if(i == 0 || cluster_score.illumination[i] > cluster_score.max_illumination)
            cluster_score.max_illumination = cluster_score.illumination[i];

        cluster_score.avg_humidity += cluster_score.soil_humidities[i];
        if(i == 0 || cluster_score.soil_humidities[i] < cluster_score.min_humidity)
            cluster_score.min_humidity = cluster_score.soil_humidities[i];
        if(i == 0 || cluster_score.soil_humidities[i] > cluster_score.max_humidity)
            cluster_score.max_humidity = cluster_score.soil_humidities[i];
    }
    // Avg aggregates
    cluster_score.avg_temp = cluster_score.avg_temp/12.f;
    cluster_score.avg_illumination = cluster_score.avg_illumination/12.f;
    cluster_score.avg_humidity = cluster_score.avg_humidity/12.f;



    // TODO: Slope
    {
        cluster_score.slope = 100;
    }

    {
        // Check if highest scoring
        std::vector<int> aggregates;
        aggregates.push_back(cluster_score.min_humidity);
        aggregates.push_back(cluster_score.min_illumination);
        aggregates.push_back(cluster_score.min_temp);
        aggregates.push_back(cluster_score.slope);
        cluster_score.valid = (getMin(aggregates) > 0);
    }

    // Aggregate avg
    cluster_score.aggregate_avg = (cluster_score.avg_temp + cluster_score.avg_illumination + cluster_score.avg_humidity + cluster_score.slope) / 4.f;

//    qCritical() << "<-- SCORE SUMMARY [" << specie_properties->specie_name << "] --> ";
//    qCritical() << "--Illumination--";
//    for(int i(0); i < 12; i++)
//        qCritical() << "[" << i << " --> " << cluster_score.illumination[i];
//    qCritical() << "MIN --> " << cluster_score.min_illumination;
//    qCritical() << "--Temperature--";
//    for(int i(0); i < 12; i++)
//        qCritical() << "[" << i << " --> " << cluster_score.temperature[i];
//    qCritical() << "MIN --> " << cluster_score.min_temp;
//    qCritical() << "--Soil Humidity--";
//    for(int i(0); i < 12; i++)
//        qCritical() << "[" << i << " --> " << cluster_score.soil_humidities[i];
//    qCritical() << "MIN --> " << cluster_score.min_humidity;
//    qCritical() << "--Slope --> " << cluster_score.slope;

    return cluster_score;
}

std::vector<int> TerrainSuitabilityScore::getValidClusters()
{
    return m_valid_clusters;
}

TerrainSuitabilityScore::Score TerrainSuitabilityScore::operator[](int cluster_idx)
{
    return m_per_cluster_scores.at(cluster_idx);
}

int TerrainSuitabilityScore::n_clusters() const
{
    return m_per_cluster_scores.size();
}

float TerrainSuitabilityScore::getMaxSuitabilityScore() const
{
    return m_max_suitability_score;
}

int TerrainSuitabilityScore::getMaxSuitabilityClusterIdx() const
{
    return m_max_suitability_cluster_idx;
}

float TerrainSuitabilityScore::getMinSuitabilityScore() const
{
    return m_min_suitability_score;
}

int TerrainSuitabilityScore::getMinSuitabilityClusterIdx() const
{
    return m_min_suitability_cluster_idx;
}
