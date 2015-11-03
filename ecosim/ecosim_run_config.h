#ifndef ECOSIM_RUN_CONFIG_H
#define ECOSIM_RUN_CONFIG_H

#include "plantDB/plant_properties.h"
#include "clustering/cluster_data.h"

class EcoSimRunConfig
{
public:
    EcoSimRunConfig(ClusterData cluster_data);
    ~EcoSimRunConfig();
    int getSpecieCount();
    void addSpecie(SpecieProperties specie);
    std::vector<SpecieProperties> getSpecies() const;
    const ClusterData m_cluster_data;

private:
    std::vector<SpecieProperties> m_species;
};

#endif // ECOSIM_RUN_CONFIG_H
