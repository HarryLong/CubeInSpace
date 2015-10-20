#ifndef PLANT_PLACEMENT_DLG_H
#define PLANT_PLACEMENT_DLG_H

#include <QDialog>
#include <QLineEdit>
#include "../clustering/cluster_data.h"

class PlantSelectionWidget;
class EcosimTrackerWidget;
class QPushButton;
class PlantPlacementDialog : public QDialog
{
Q_OBJECT
public:
    PlantPlacementDialog(QWidget * parent = 0);
    ~PlantPlacementDialog();

public slots:
    void setClusters(std::vector<ClusterData> clusters);

private slots:
    void startEcoSimRuns();

private:
    void init_layout();

    PlantSelectionWidget * m_plant_selection_widget;
    EcosimTrackerWidget * m_ecosim_tracker_widget;
    QPushButton * m_ok_btn;
    QPushButton * m_cancel_btn;
};

#endif // PLANT_PLACEMENT_DLG_H
