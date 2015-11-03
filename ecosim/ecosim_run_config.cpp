#include "ecosim_run_config.h"
#include <algorithm>

EcoSimRunConfig::EcoSimRunConfig(ClusterData cluster_data) : m_cluster_data(cluster_data)
{

}
EcoSimRunConfig::~EcoSimRunConfig()
{

}

int EcoSimRunConfig::getSpecieCount()
{
    return m_species.size();
}

void EcoSimRunConfig::addSpecie(SpecieProperties specie)
{
    m_species.push_back(specie);
}

std::vector<SpecieProperties> EcoSimRunConfig::getSpecies() const
{
    return m_species;
}
