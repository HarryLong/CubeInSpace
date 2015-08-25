#include "cluster_data.h"

bool ClusterData::operator==(const ClusterData & other)
{
    if(slope != other.slope)
        return false;

    for(int i(0); i < 2; i++)
    {
        if(temperatures[i] != other.temperatures[i])
            return false;

        if(illumination[i] != other.illumination[i])
            return false;
    }

    for(int i(0); i < 12; i++)
    {
        if(soil_humidities[i] != other.soil_humidities[i])
            return false;
    }

    return true;
}
