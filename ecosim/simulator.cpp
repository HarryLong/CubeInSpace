#include "simulator.h"

EcoSimulator::EcoSimulator(std::vector<EcoSimRunConfig> configs) : m_run_configs(configs), m_current_simulation_idx(0), m_thread(nullptr)
{
    void processing("Running simulation");
    start_next_simulation();
}

EcoSimulator::~EcoSimulator()
{
    delete_thread();
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
    m_current_simulation_idx++;
    emit processDescriptionUpdate("Running simulation " + QString::number(m_current_simulation_idx) + "/" + QString::number(m_run_configs.size()));

    build_simulation_config(m_run_configs.at(m_current_simulation_idx-1));

    delete_thread();
    m_thread = new std::thread(&EcoSimulator::run_simulation, this);
}

SimulationConfiguration EcoSimulator::build_simulation_config(EcoSimRunConfig & run_config)
{
    m_active_simulation_config.m_duration = 100*12; // 100 years
    m_active_simulation_config.m_slope = run_config.m_cluster_data.slope;
    for(int i(0); i < 12; i++)
    {
        m_active_simulation_config.m_humidity.push_back((int) run_config.m_cluster_data.soil_humidities[i]);
        m_active_simulation_config.m_temperature.push_back((int) run_config.m_cluster_data.temperatures[i]);
        m_active_simulation_config.m_illumination.push_back((int) run_config.m_cluster_data.illumination[i]);
    }
    m_active_simulation_config.m_seeding_enabled = true;

    for(SpecieProperties & specie : run_config.getSpecies())
    {
        m_active_simulation_config.m_plants_to_generate.emplace(specie.specie_id, -1); // -1 forces to spawn base seed count
    }
}

void EcoSimulator::progressUpdate(float percent)
{
    float real_percentage(percent * (((float)m_current_simulation_idx)/m_run_configs.size()));
    emit processingUpdate((int) real_percentage);
}

void EcoSimulator::complete()
{
    if(m_current_simulation_idx == m_run_configs.size())
        emit processingComplete();
    else
        start_next_simulation();
}

void EcoSimulator::run_simulation()
{
    SimulatorManager::start(m_active_simulation_config, this);
}

