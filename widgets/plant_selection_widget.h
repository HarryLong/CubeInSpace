#ifndef PLANT_SELECTION_WIDGET_H
#define PLANT_SELECTION_WIDGET_H

#include <QWidget>
#include "plantDB/plant_db.h"
#include "../clustering/cluster_data.h"
#include "../widgets/histogram_widget.h"
#include "../clustering/terrain_suitability_score.h"
#include "../widgets/specie_suitability_list_widget.h"
#include "../ecosim/ecosim_run_config.h"

#include <QLineEdit>

/*****************************
 * HISTOGRAM WRAPPER WIDGETS *
 *****************************/
class HistogramWrapperWidget : public QWidget
{
Q_OBJECT
public:
    HistogramWrapperWidget(std::vector<QString> titles, QWidget * parent = 0);
    ~HistogramWrapperWidget();
    HistogramWidget* operator[](int index);
    virtual void setScores(int * scores);
    void clear();

    static std::vector<QString> getMonthTitles();
private:
    void init_layout(std::vector<QString> & titles);
    std::vector<HistogramWidget*> m_histograms;
};

class AggregateHistogramWrapperWidget : public HistogramWrapperWidget
{
public:
    static const int _SLOPE = 0;
    static const int _TEMP= 1;
    static const int _ILLUMINATION = 2;
    static const int _HUMIDITY = 3;
    AggregateHistogramWrapperWidget(QWidget * parent = 0);
    ~AggregateHistogramWrapperWidget();

private:
    static std::vector<QString> getTitles();
};

/************************
 * SCORE SUMMARY WIDGET *
 ************************/
class QComboBox;
class ScoreSummaryWidget : public QWidget
{
Q_OBJECT
public:
    enum Type{
        _AVG = 0,
        _MIN,
        _MAX,
        _TEMP,
        _ILLUMINATION,
        _SOIL_HUMIDITY,
        _COUNT
    };
    ScoreSummaryWidget(QWidget * parent = 0);
    ~ScoreSummaryWidget();

public slots:
    void setSelected(QListWidgetItem * selected_plant);
    void setType(int type);
    void setClusterIdx(int cluster_idx);
    void refreshHistograms();
    void clear();

private:
    void hide_all();
    void init_layout();
    void init_connections();
    HistogramWrapperWidget * m_aggregate_histogram_widgets[Type::_COUNT];
    TerrainSuitabilityScore m_score;
    int m_selected_cluster_idx;
    Type m_selected_type;
    QComboBox * m_cluster_selection_combo_box;
    QComboBox * m_type_selection_combo_box;
};

/********************
 * SEARCH LINE EDIT *
 ********************/
class SearchLineEdit : public QLineEdit
{
Q_OBJECT
public:
    SearchLineEdit(QWidget * parent = 0);
    ~SearchLineEdit();
};

/**************************
 * PLANT SELECTION DIALOG *
 **************************/
class QPushButton;
class PlantSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    PlantSelectionWidget(QWidget * parent = 0);
    ~PlantSelectionWidget();
    std::vector<EcoSimRunConfig> getRunConfigs();
    int getSelectedPlantCount();

signals:
    void plantAdded();
    void plantRemoved();
    void noPlantsSelected();

public slots:
    void setClusters(std::vector<ClusterData> clusters);

private slots:
    void refresh();
    void add_selected_plant();
    void remove_selected_plant();
    void refresh_filter(QString filter_text);

private:
    void init_layout();
    void init_connections();
    void remove_all_list_items();

    SpecieSuitabilityListWidget * m_available_plants_widget;
    SpecieSuitabilityListWidget * m_added_plants_widget;
    ScoreSummaryWidget * m_score_summary_widget;
    QPushButton * m_add_btn;
    QPushButton * m_remove_btn;
    QLineEdit * m_filter_le;
    std::vector<ClusterData> m_cluster_data;
    static const QFont _LABEL_FONT;
};

#endif //PLANT_SELECTION_DLG_H
