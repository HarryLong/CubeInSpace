#ifndef ECOSIM_TRACKER_WIDGET_H
#define ECOSIM_TRACKER_WIDGET_H

#include <QWidgetSet>
#include "plantDB/plant_properties.h"
#include "clustering/cluster_data.h"
#include <QWidget>

class EcosimTrackerWidget : public QWidget
{
public:
    EcosimTrackerWidget(QWidget * parent);
    ~EcosimTrackerWidget();

    void track(std::vector<SpecieProperties> species, std::vector<ClusterData> cluster_data);
};

#endif
