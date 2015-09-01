#include "plant_selection_dlg.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>

/*****************************
 * TERRAIN SUITABILITY SCORE *
 *****************************/
TerrainSuitabilityScore::TerrainSuitabilityScore(SpecieProperties * specie_properties, std::vector<ClusterData> cluster_data)
{
    for(ClusterData & cluster : cluster_data)
    {
        m_scores.push_back(calculate_score(specie_properties, cluster));
    }
}

TerrainSuitabilityScore::~TerrainSuitabilityScore()
{

}

TerrainSuitabilityScore::Score calculate_score(SpecieProperties * specie_properties, ClusterData & cluster_data)
{
    TerrainSuitabilityScore::Score cluster_score;

    for(int i = 0; i < 12; i++)
    {
        int illumination(cluster_data.illumination[i]);
        int soil_humidity(cluster_data.soil_humidities[i]);
        int temperature(cluster_data.temperatures[i]);

        // Illumination
        {
            IlluminationProperties * properties(specie_properties->illumination_properties);
            int score = 0;
            if(illumination >= properties->prime_illumination.first &&
                    illumination <= properties->prime_illumination.second)
            {
                score = 100;
            }
            else if(illumination < properties->prime_illumination.first &&
                    illumination >= properties->min_illumination)
            {
                score = (((float)illumination)/(properties->prime_illumination.first-properties->min_illumination));
            }
            else if(illumination > properties->prime_illumination.second &&
                    illumination <= properties->max_illumination)
            {
                score = (((float)illumination)/(properties->max_illumination - properties->prime_illumination.second));
            }
            cluster_score.illumination[i] = score;
        }

        // Soil Humidity
        {
            SoilHumidityProperties * properties(specie_properties->soil_humidity_properties);
            int score = 0;
            if(soil_humidity >= properties->prime_soil_humidity.first &&
                    soil_humidity <= properties->prime_soil_humidity.second)
            {
                score = 100;
            }
            else if(soil_humidity < properties->prime_soil_humidity.first &&
                    soil_humidity >= properties->min_soil_humidity)
            {
                score = (((float)soil_humidity)/(properties->prime_soil_humidity.first-properties->min_soil_humidity));
            }
            else if(soil_humidity > properties->prime_soil_humidity.second &&
                    soil_humidity <= properties->max_soil_humidity)
            {
                score = (((float)soil_humidity)/(properties->max_soil_humidity - properties->prime_soil_humidity.second));
            }
            cluster_score.soil_humidities[i] = score;
        }

        // Temperature
        {
            TemperatureProperties * properties(specie_properties->temperature_properties);
            int score = 0;
            if(temperature >= properties->prime_temp.first &&
                    temperature <= properties->prime_temp.second)
            {
                score = 100;
            }
            else if(temperature < properties->prime_temp.first &&
                    temperature >= properties->min_temp)
            {
                score = (((float)temperature)/(properties->prime_temp.first-properties->min_temp));
            }
            else if(temperature > properties->prime_temp.second &&
                    temperature <= properties->max_temp)
            {
                score = (((float)temperature)/(properties->max_temp - properties->prime_temp.second));
            }
            cluster_score.temperature[i] = score;
        }

        // TODO: Slope
        {
            cluster_score.slope = 100;
        }

        return cluster_score;
    }
}

/*******************************
 * SPECIE PROPERTIES LIST ITEM *
 *******************************/
SpeciePropertiesListItem::SpeciePropertiesListItem ( const SpecieProperties * specie_properties, TerrainSuitabilityScore suitability_score) :
    QListWidgetItem(), m_specie_properties(nullptr), m_suitability_score(suitability_score)
{
    setSpecieProperties(specie_properties);
}

SpeciePropertiesListItem::~SpeciePropertiesListItem()
{
    delete m_specie_properties;
}

void SpeciePropertiesListItem::setSpecieProperties( const SpecieProperties * specie_properties)
{
    if(m_specie_properties)
        delete m_specie_properties;
    m_specie_properties = specie_properties;
    setText(specie_properties->specie_name);
}

const SpecieProperties * SpeciePropertiesListItem::getProperties()
{
    return m_specie_properties;
}

/*********************************
 * SPECIE PROPERTIES LIST WIDGET *
 *********************************/
const QFont SpeciePropertiesListWidget::_LABEL_FONT("Times", 15, QFont::Weight::Bold);
SpeciePropertiesListWidget::SpeciePropertiesListWidget(QString title, QWidget * parent) : QWidget(parent)
{
    init_layout(title);
}

SpeciePropertiesListWidget::~SpeciePropertiesListWidget()
{

}

void SpeciePropertiesListWidget::init_layout(QString title)
{
    m_list = new QListWidget(this);

    QVBoxLayout * main_layout = new QVBoxLayout;

    QLabel * lbl = new QLabel(title);
    lbl->setFont(SpeciePropertiesListWidget::_LABEL_FONT);

    {
        QHBoxLayout * layout = new QHBoxLayout;
        layout->addWidget(lbl, 1, Qt::AlignCenter);
        main_layout->addLayout(layout);
    }
    main_layout->addWidget(m_list);

    setLayout(main_layout);
}

void SpeciePropertiesListWidget::filter(QString filter_string)
{
    hide_all();

    QList<QListWidgetItem*> matches ( m_list->findItems(filter_string, Qt::MatchFlag::MatchContains) );
    for(QListWidgetItem* item : matches)
        item->setHidden(false);
}

void SpeciePropertiesListWidget::hide_all()
{
    for(int row(0); row < m_list->count(); row++ )
        m_list->item(row)->setHidden(true);
}

/**************************
 * PLANT SELECTION DIALOG *
 **************************/
PlantSelectionDialog::PlantSelectionDialog( std::vector<ClusterData> cluster_data, QWidget * parent ) : QDialog(parent),
  m_available_plants_widget ( new SpeciePropertiesListWidget("Available", this) ),
  m_added_plants_widget ( new SpeciePropertiesListWidget("Added", this) ),
  m_add_btn ( new QPushButton(">>", this) ),
  m_remove_btn ( new QPushButton("<<", this) ),
  m_filter_le (new QLineEdit(this) ),
  m_cluster_data(cluster_data)
{
    setWindowTitle("Plant Selection");
    setFixedSize(1200,800);
    init_layout();
    init_connections();

    refresh();
}

PlantSelectionDialog::~PlantSelectionDialog()
{
    remove_all_list_items();
}

void PlantSelectionDialog::refresh()
{
    //TODO: CLEAR BOTH LIST WIDGETS
    remove_all_list_items();

    PlantDB::SpeciePropertiesHolder all_species(PlantDB().getAllPlantData());
    for(auto it(all_species.begin()); it != all_species.end(); it++)
    {
        TerrainSuitabilityScore suitability_score(it->second, m_cluster_data);
        m_available_plants_widget->m_list->addItem( new SpeciePropertiesListItem(it->second, suitability_score) );
    }

    m_available_plants_widget->m_list->sortItems();
}

void PlantSelectionDialog::remove_all_list_items()
{
    m_available_plants_widget->m_list->clear();
    m_added_plants_widget->m_list->clear();
}

void PlantSelectionDialog::add_selected_plant()
{
    QList<QListWidgetItem*> selected_items (m_available_plants_widget->m_list->selectedItems());

    bool count(selected_items.size());

    for(QListWidgetItem * item : selected_items)
        m_added_plants_widget->m_list->addItem(m_available_plants_widget->m_list->takeItem(m_available_plants_widget->m_list->row(item)));

    if(count > 0)
        m_added_plants_widget->m_list->sortItems();
}

void PlantSelectionDialog::remove_selected_plant()
{
    QList<QListWidgetItem*> selected_items (m_added_plants_widget->m_list->selectedItems());
    bool count(selected_items.size());

    for(QListWidgetItem * item : selected_items)
        m_available_plants_widget->m_list->addItem(m_added_plants_widget->m_list->takeItem(m_added_plants_widget->m_list->row(item)));


    if(count > 0)
        m_available_plants_widget->m_list->sortItems();
}

void PlantSelectionDialog::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;

    // Available plants & add/remove buttons & Added plants
    {
        QHBoxLayout * h_layout = new QHBoxLayout;

        // Available plants
        {
            h_layout->addWidget(m_available_plants_widget, 1);
        }
        // Add/Remove button
        {
            QVBoxLayout * layout = new QVBoxLayout;
            layout->addWidget(m_add_btn);
            layout->addWidget(m_remove_btn);
            h_layout->addLayout(layout);
        }
        // Added plants layout
        {
            h_layout->addWidget(m_added_plants_widget, 1);
        }

        main_layout->addLayout(h_layout);
    }

    // Filter
    {
        m_filter_le->setMinimumWidth(20);
        QHBoxLayout * layout = new QHBoxLayout;

        layout->addWidget(new QLabel,1); //left-padding

        layout->addWidget(new QLabel("Search: "), 0); // Left-Padding
        layout->addWidget(m_filter_le, 0);

        layout->addWidget(new QLabel,1); // Right-padding

        main_layout->addLayout(layout);
    }

    setLayout(main_layout);
}

void PlantSelectionDialog::refresh_filter(QString filter_text)
{
    m_available_plants_widget->filter(filter_text);
    m_added_plants_widget->filter(filter_text);
}

void PlantSelectionDialog::init_connections()
{
    connect(m_add_btn, SIGNAL(clicked(bool)), this, SLOT(add_selected_plant()));
    connect(m_remove_btn, SIGNAL(clicked(bool)), this, SLOT(remove_selected_plant()));
    connect(m_filter_le, SIGNAL(textEdited(QString)), this, SLOT(refresh_filter(QString)));
}

/*
 * QListWidgets
 * Refresh button
 * Add button
 * Remove button
 *
 */
