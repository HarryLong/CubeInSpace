#include "plant_placement_dlg.h"
#include <QGridLayout>
#include "../widgets/plant_selection_widget.h"
#include "../widgets/ecosim_tracker_widget.h"
#include <QPushButton>

PlantPlacementDialog::PlantPlacementDialog(QWidget *parent) : QDialog(parent), m_plant_selection_widget(new PlantSelectionWidget(this)),
    m_ecosim_tracker_widget(new EcosimTrackerWidget(this)), m_ok_btn(new QPushButton("OK", this)), m_cancel_btn(new QPushButton("Cancel", this))
{
    init_layout();

    connect(m_ok_btn, SIGNAL(clicked(bool)), this, SLOT(startEcoSimRuns()));
    connect(m_cancel_btn, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

PlantPlacementDialog::~PlantPlacementDialog()
{

}

void PlantPlacementDialog::startEcoSimRuns()
{

}


void PlantPlacementDialog::setClusters(std::vector<ClusterData> clusters)
{
    m_plant_selection_widget->setClusters(clusters);
}

void PlantPlacementDialog::init_layout()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(m_plant_selection_widget,0,0,1,1,Qt::AlignCenter);
    layout->addWidget(m_ecosim_tracker_widget,0,0,1,1,Qt::AlignCenter);



    QHBoxLayout * button_layout = new QHBoxLayout();
    button_layout->addWidget(m_cancel_btn, 0 , Qt::AlignCenter);
    button_layout->addWidget(m_ok_btn, 0 , Qt::AlignCenter);
    layout->addLayout(button_layout,1,0,1,1,Qt::AlignCenter);

    setLayout(layout);
}
