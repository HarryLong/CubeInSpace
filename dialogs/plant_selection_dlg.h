#ifndef PLANT_SELECTION_DLG_H
#define PLANT_SELECTION_DLG_H

#include <QDialog>
#include <QListWidget>
#include "plantDB/plant_db.h"
#include "../clustering/cluster_data.h"

/*****************************
 * TERRAIN SUITABILITY SCORE *
 *****************************/
class TerrainSuitabilityScore{
public:
    struct Score{
        int slope;
        int illumination[12];
        int temperature[12];
        int soil_humidities[12];
    };

    TerrainSuitabilityScore(SpecieProperties * specie_properties, std::vector<ClusterData> cluster_data);
    ~TerrainSuitabilityScore();

private:
    Score calculate_score(SpecieProperties * specie_properties, ClusterData & cluster_data);
    std::vector<Score> m_scores;
};

/*******************************
 * SPECIE PROPERTIES LIST ITEM *
 *******************************/
class SpeciePropertiesListItem : public QListWidgetItem
{
public:
    SpeciePropertiesListItem ( const SpecieProperties * specie_properties, TerrainSuitabilityScore suitability_scote);
    ~SpeciePropertiesListItem ();
    void setSpecieProperties(const SpecieProperties * specie_properties);
    const SpecieProperties * getProperties();

private:
    const SpecieProperties * m_specie_properties;
    TerrainSuitabilityScore m_suitability_score;
};

/*********************************
 * SPECIE PROPERTIES LIST WIDGET *
 *********************************/
class SpeciePropertiesListWidget : public QWidget
{
public:
    SpeciePropertiesListWidget(QString title, QWidget * parent = 0);
    ~SpeciePropertiesListWidget();

    void filter(QString filter_string);
    QListWidget * m_list;

private:
    void hide_all();
    void init_layout(QString title);
    static const QFont _LABEL_FONT;
};

/**************************
 * PLANT SELECTION DIALOG *
 **************************/
class QPushButton;
class PlantSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    PlantSelectionDialog(std::vector<ClusterData> cluster_data, QWidget * parent = 0);
    ~PlantSelectionDialog();

private slots:
    void refresh();
    void add_selected_plant();
    void remove_selected_plant();
    void refresh_filter(QString filter_text);

private:
    void init_layout();
    void init_connections();
    void remove_all_list_items();

    SpeciePropertiesListWidget * m_available_plants_widget;
    SpeciePropertiesListWidget * m_added_plants_widget;
    QPushButton * m_add_btn;
    QPushButton * m_remove_btn;
    QLineEdit * m_filter_le;
    std::vector<ClusterData> m_cluster_data;
};

#endif //PLANT_SELECTION_DLG_H
