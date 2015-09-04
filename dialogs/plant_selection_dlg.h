#ifndef PLANT_SELECTION_DLG_H
#define PLANT_SELECTION_DLG_H

#include <QDialog>
#include <QListWidget>
#include "plantDB/plant_db.h"
#include "../clustering/cluster_data.h"
#include <QLineEdit>

/*****************************
 * TERRAIN SUITABILITY SCORE *
 *****************************/
class TerrainSuitabilityScore{
public:
    class Score{
    public:
        Score();
        int slope;
        int illumination[12];
        int min_illumination;
        int max_illumination;
        float avg_illumination;

        int temperature[12];
        int min_temp;
        int max_temp;
        float avg_temp;

        int soil_humidities[12];
        int min_humidity;
        int max_humidity;
        float avg_humidity;

        float aggregate_avg;
    };

    TerrainSuitabilityScore(const SpecieProperties * specie_properties, std::vector<ClusterData> cluster_data);
    TerrainSuitabilityScore();
    ~TerrainSuitabilityScore();
    std::vector<int> getValidClusters();
    int n_clusters() const;
    float getMaxSuitabilityScore() const;
    int getMaxSuitabilityClusterIdx() const;
    float getMinSuitabilityScore() const;
    int getMinSuitabilityClusterIdx() const;
    Score operator[](int cluster_idx);

private:
    int calculate_lower_end_score(int value, int min, int prime_start);
    int calculate_upper_end_score(int value, int max, int prime_end);
    int getMin(std::vector<int> values);
    Score calculate_score(const SpecieProperties * specie_properties, ClusterData & cluster_data);
    std::vector<Score> m_per_cluster_scores;
    std::vector<int> m_valid_clusters;
    int m_max_suitability_score;
    int m_max_suitability_cluster_idx;
    int m_min_suitability_score;
    int m_min_suitability_cluster_idx;
};

/*******************************
 * SPECIE PROPERTIES LIST ITEM *
 *******************************/
class SpeciePropertiesListItem : public QListWidgetItem
{
public:
    SpeciePropertiesListItem ( const SpecieProperties * specie_properties, TerrainSuitabilityScore suitability_score);
    ~SpeciePropertiesListItem ();
    void setSpecieProperties(const SpecieProperties * specie_properties);
    const SpecieProperties * getProperties() const;
    TerrainSuitabilityScore getSuitabilityScore() const;
    bool operator<(const QListWidgetItem & other) const override;

private:
    const SpecieProperties * m_specie_properties;
    const TerrainSuitabilityScore m_suitability_score;
};

/*********************************
 * SPECIE PROPERTIES LIST WIDGET *
 *********************************/
class SpeciePropertiesListWidget : public QListWidget
{
Q_OBJECT
public:
    SpeciePropertiesListWidget( QWidget * parent = 0 );
    ~SpeciePropertiesListWidget();

    void filter(QString filter_string);

public slots:
    void unselect();

private:
    void hide_all();
    void init_layout(QString title);
};

/********************
 * HISTOGRAM WIDGET *
 ********************/
class QLabel;
class HistogramWidget : public QWidget
{
Q_OBJECT
public:
    HistogramWidget(QString heading, QWidget * parent = 0);
    ~HistogramWidget();

    void setValue(int value);
    void clear();

protected:
    void paintEvent(QPaintEvent * event) override;

private:
    void init_layout();
    int m_value;
    static const QFont _LABEL_FONT;
    QString m_heading;
    QLabel * m_heading_lbl;
};

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
class PlantSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    PlantSelectionDialog(QWidget * parent = 0);
    ~PlantSelectionDialog();

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

    SpeciePropertiesListWidget * m_available_plants_widget;
    SpeciePropertiesListWidget * m_added_plants_widget;
    ScoreSummaryWidget * m_score_summary_widget;
    QPushButton * m_add_btn;
    QPushButton * m_remove_btn;
    QLineEdit * m_filter_le;
    std::vector<ClusterData> m_cluster_data;
    static const QFont _LABEL_FONT;
};

#endif //PLANT_SELECTION_DLG_H
