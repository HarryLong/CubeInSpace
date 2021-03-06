#include "plant_placement_dlg.h"
#include <QGridLayout>
#include "../widgets/plant_selection_widget.h"
#include <QPushButton>
#include <QCloseEvent>
#include <QKeyEvent>

PlantPlacementDialog::PlantPlacementDialog(QWidget *parent) : QDialog(parent), m_plant_selection_widget(new PlantSelectionWidget(this)),
    m_ok_btn(new QPushButton("OK", this))
{
    init_layout();

    connect(m_ok_btn, SIGNAL(clicked(bool)), this, SLOT(emit_refresh_plants()));

    connect(m_plant_selection_widget, SIGNAL(plantAdded()), this, SLOT(enable_simulate_button()));
    connect(m_plant_selection_widget, SIGNAL(noPlantsSelected()), this, SLOT(disable_simulate_button()));
}

PlantPlacementDialog::~PlantPlacementDialog()
{

}

std::vector<EcoSimRunConfig> PlantPlacementDialog::getRunConfigs()
{
    return m_plant_selection_widget->getRunConfigs();
}

void PlantPlacementDialog::setClusters(std::vector<ClusterData> clusters)
{
    m_plant_selection_widget->setClusters(clusters);
}

void PlantPlacementDialog::enable_simulate_button()
{
    m_ok_btn->setEnabled(true);
}

void PlantPlacementDialog::disable_simulate_button()
{
    m_ok_btn->setEnabled(false);
}

void PlantPlacementDialog::init_layout()
{
    QVBoxLayout * layout (new QVBoxLayout);
    layout->addWidget(m_plant_selection_widget, 1);
    layout->addWidget(m_ok_btn, 0 , Qt::AlignCenter);

    setLayout(layout);
}

void PlantPlacementDialog::keyPressEvent(QKeyEvent * event)
{
    if(event->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(event);
    else
        event->ignore();
}

void PlantPlacementDialog::closeEvent(QCloseEvent * event)
{
    event->ignore();
}

void PlantPlacementDialog::emit_refresh_plants()
{
    emit refreshPlants();
}
