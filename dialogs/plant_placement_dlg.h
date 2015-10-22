#ifndef PLANT_PLACEMENT_DLG_H
#define PLANT_PLACEMENT_DLG_H

#include <QDialog>
#include <QLineEdit>
#include "../clustering/cluster_data.h"
#include "../ecosim_run_config.h"

class PlantSelectionWidget;
class QPushButton;
class PlantPlacementDialog : public QDialog
{
Q_OBJECT
public:
    PlantPlacementDialog(QWidget * parent = 0);
    ~PlantPlacementDialog();

    std::vector<EcoSimRunConfig> getRunConfigs();

public slots:
    void setClusters(std::vector<ClusterData> clusters);

private slots:
    void enable_simulate_button();
    void disable_simulate_button();

private:
    void init_layout();

    PlantSelectionWidget * m_plant_selection_widget;
    QPushButton * m_ok_btn;
    QPushButton * m_cancel_btn;
};

#endif // PLANT_PLACEMENT_DLG_H
