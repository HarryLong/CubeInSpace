#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "ecosim_run_config.h"
#include "ecosimulator/simulator/core/simulator_manager.h"
#include "ecosimulator/simulator/core/simulation_configuration.h"
#include <QObject>
#include <thread>

class EcoSimulator : public QObject, public SimulatorManager::ProgressListener{
    Q_OBJECT
public:
    EcoSimulator(std::vector<EcoSimRunConfig> configs);
    ~EcoSimulator();

    virtual void progressUpdate(float percent);
    virtual void complete();

signals:
    void processing(QString description);
    void processingUpdate(int percent_complete);
    void processingComplete();
    void processDescriptionUpdate(QString description);

private:
    SimulationConfiguration build_simulation_config(EcoSimRunConfig & run_config);
    void delete_thread();
    void start_next_simulation();
    void start_thread();
    int m_current_simulation_idx;
    SimulationConfiguration m_active_simulation_config;
    std::vector<EcoSimRunConfig> m_run_configs;
    std::thread * m_thread;
};

#endif // SIMULATOR_H