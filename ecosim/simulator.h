#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "ecosim_run_config.h"
#include "ecosimulator/simulator/core/simulator_manager.h"
#include "ecosimulator/simulator/core/simulation_configuration.h"
#include <QObject>
#include <thread>
#include "../worker.h"

class EcoSimulator : public Worker, public SimulatorManager::ProgressListener{
    Q_OBJECT
public:
    EcoSimulator();
    ~EcoSimulator();

    void start(const std::vector<EcoSimRunConfig> & configs);

    virtual void progressUpdate(float percent);
    virtual void progressUpdate(QString info);
    virtual void complete();

private:
    SimulationConfiguration to_simulation_config(const EcoSimRunConfig & run_config) const;
    void delete_thread();
    void start_next_simulation();
    void run_simulation();
    void reset();
    bool requires_simulation(const SimulationConfiguration & config);
    int m_current_simulation_idx;
//    SimulationConfiguration m_active_simulation_config;
//    std::vector<EcoSimRunConfig> m_run_configs;
    std::vector<SimulationConfiguration> m_run_configs;
    std::thread * m_thread;
};

#endif // SIMULATOR_H
