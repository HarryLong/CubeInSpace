#ifndef ECOSIM_TRACKER_WIDGET_H
#define ECOSIM_TRACKER_WIDGET_H

#include <QWidgetSet>
#include "../ecosim_run_config.h"
#include <QWidget>
#include <map>
#include "clustering/k_means_clusterer.h"

class QProgressBar;
class QBoxLayout;
class SingleSimTrackerWidget : public QWidget
{
    Q_OBJECT
public:
    SingleSimTrackerWidget(int cluster_idx, QWidget * parent = 0);
    ~SingleSimTrackerWidget();
    void reset();

private:
    void init_layout();

    QProgressBar * m_progress_bar;
    int m_cluster_idx;
};

class EcosimTrackerWidget : public QWidget
{
public:
    EcosimTrackerWidget(QWidget * parent = 0);
    ~EcosimTrackerWidget();

    void track(std::vector<EcoSimRunConfig> run_configs);

private:
    void init_layout();
    void reset();
    SingleSimTrackerWidget * m_trackers[KMeansClusterer::_MAX_CLUSTERS];
};

#endif
