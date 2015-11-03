#include "simulator.h"
#include "ecotracker/tracker.h"
#include <QDebug>

EcoSimulator::EcoSimulator() : m_thread(nullptr)
{

}

EcoSimulator::~EcoSimulator()
{
    delete_thread();
}

bool EcoSimulator::requires_simulation(const SimulationConfiguration & config)
{
    std::set<int> species;
    for(auto it(config.m_plants_to_generate.begin()); it != config.m_plants_to_generate.end(); it++)
        species.insert(it->first);

    EntryData entry_data(config.m_humidity,
                         config.m_illumination,
                         config.m_temperature,
                         species,
                         config.m_duration);

    return !Tracker::contains(entry_data);
}

void EcoSimulator::start(const std::vector<EcoSimRunConfig> & configs)
{
    reset();
    for(const EcoSimRunConfig & config : configs)
    {
        SimulationConfiguration simulation_config(to_simulation_config(config));
        if(requires_simulation(simulation_config))
            m_run_configs.push_back(simulation_config);
    }
    if(m_run_configs.size() > 0)
    {
        emit processing("Running simulation");
        start_next_simulation();
    }
}

void EcoSimulator::reset()
{
    m_run_configs.clear();
    m_current_simulation_idx = 0;
}

void EcoSimulator::delete_thread()
{
    if(m_thread)
    {
        m_thread->join();
        delete m_thread;
    }
}

void EcoSimulator::start_next_simulation()
{
    emit processDescriptionUpdate("Running simulation " + QString::number(m_current_simulation_idx+1) + "/" + QString::number(m_run_configs.size()));

    delete_thread();

    m_thread = new std::thread(&EcoSimulator::run_simulation, this);
}

SimulationConfiguration EcoSimulator::to_simulation_config(const EcoSimRunConfig & run_config) const
{
    SimulationConfiguration simulation_config;

    simulation_config.m_duration = 100*12;
    simulation_config.m_seeding_enabled = true;
    // Plants to generate
    for(SpecieProperties & specie : run_config.getSpecies())
        simulation_config.m_plants_to_generate.emplace(specie.specie_id, -1); // -1 forces to spawn base seed count
    for(int i(0); i < 12; i++)
    {
        simulation_config.m_humidity.push_back((int) run_config.m_cluster_data.soil_humidities[i]);
        simulation_config.m_temperature.push_back((int) run_config.m_cluster_data.temperatures[i]);
        simulation_config.m_illumination.push_back((int) run_config.m_cluster_data.illumination[i]);
    }
    simulation_config.m_slope = run_config.m_cluster_data.slope;

    return simulation_config;
}

void EcoSimulator::progressUpdate(float percent)
{
    float real_percentage(percent * (((float)m_current_simulation_idx+1)/m_run_configs.size()));
    emit processingUpdate((int) real_percentage);
}

void EcoSimulator::complete()
{
    if(++m_current_simulation_idx == m_run_configs.size())
        emit processingComplete();
    else
        start_next_simulation();
}

void EcoSimulator::progressUpdate(QString info)
{
    emit processDescriptionUpdate(info);
}

void EcoSimulator::run_simulation()
{
    SimulatorManager::start(m_run_configs.at(m_current_simulation_idx), this);
}
