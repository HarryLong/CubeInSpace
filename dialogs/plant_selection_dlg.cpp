#include "plant_selection_dlg.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QComboBox>

/*****************************
 * TERRAIN SUITABILITY SCORE *
 *****************************/
TerrainSuitabilityScore::Score::Score() :
    slope(0),
    min_illumination(0),
    max_illumination(0),
    avg_illumination(0),
    min_temp(0),
    max_temp(0),
    avg_temp(0),
    min_humidity(0),
    max_humidity(0),
    avg_humidity(0) {}

TerrainSuitabilityScore::TerrainSuitabilityScore(const SpecieProperties * specie_properties, std::vector<ClusterData> cluster_data)
{
    for(int cluster_idx(0); cluster_idx < cluster_data.size(); cluster_idx++)
    {
        ClusterData & cluster(cluster_data.at(cluster_idx));
        TerrainSuitabilityScore::Score cluster_score( calculate_score(specie_properties, cluster) );
        m_per_cluster_scores.push_back(cluster_score);

        // Check if highest scoring
        std::vector<int> aggregates;
        aggregates.push_back(cluster_score.min_humidity);
        aggregates.push_back(cluster_score.min_illumination);
        aggregates.push_back(cluster_score.min_temp);
        aggregates.push_back(cluster_score.slope);
        int min_aggregate(getMin(aggregates));

        if(min_aggregate > 0)
        {
            m_valid_clusters.push_back(cluster_idx);
        }

        if(cluster_idx == 0 || cluster_score.aggregate_avg < m_min_suitability_score)
        {
            m_min_suitability_score = cluster_score.aggregate_avg;
            m_min_suitability_cluster_idx = cluster_idx;
        }
        if(cluster_idx == 0 || cluster_score.aggregate_avg > m_max_suitability_score)
        {
            m_max_suitability_score = cluster_score.aggregate_avg;
            m_max_suitability_cluster_idx = cluster_idx;
        }
    }
}

TerrainSuitabilityScore::TerrainSuitabilityScore()
{

}

TerrainSuitabilityScore::~TerrainSuitabilityScore()
{

}
int TerrainSuitabilityScore::getMin(std::vector<int> values)
{
    int min(values.at(0));

    for(int i(1); i < values.size(); i++)
        min = std::min(min, values.at(i));

    return min;
}

int TerrainSuitabilityScore::calculate_lower_end_score(int value, int min, int prime_start)
{
    return std::round(( ((float)value-min) / (prime_start-min)) * 100 );
}

int TerrainSuitabilityScore::calculate_upper_end_score(int value, int max, int prime_end)
{
    return std::round( (1 - ( ((float)value-prime_end) / (max-prime_end))) * 100 );
}

TerrainSuitabilityScore::Score TerrainSuitabilityScore::calculate_score(const SpecieProperties * specie_properties, ClusterData & cluster_data)
{
    TerrainSuitabilityScore::Score cluster_score;

    for(int i = 0; i < 12; i++)
    {
        int illumination(cluster_data.illumination[i]);
        int soil_humidity(cluster_data.soil_humidities[i]);
        int temperature(cluster_data.temperatures[i]);

        // Illumination
        {
            const IlluminationProperties * properties(specie_properties->illumination_properties);
            int score = 0;
            if(illumination >= properties->prime_illumination.first &&
                    illumination <= properties->prime_illumination.second)
            {
                score = 100;
            }
            else if(illumination < properties->prime_illumination.first &&
                    illumination >= properties->min_illumination)
            {
                score = calculate_lower_end_score(illumination, properties->min_illumination, properties->prime_illumination.first);
            }
            else if(illumination > properties->prime_illumination.second &&
                    illumination <= properties->max_illumination)
            {
                score = calculate_upper_end_score(illumination, properties->max_illumination, properties->prime_illumination.second);
            }
            cluster_score.illumination[i] = score;
        }

        // Soil Humidity
        {
            const SoilHumidityProperties * properties(specie_properties->soil_humidity_properties);
            int score = 0;
            if(soil_humidity >= properties->prime_soil_humidity.first &&
                    soil_humidity <= properties->prime_soil_humidity.second)
            {
                score = 100;
            }
            else if(soil_humidity < properties->prime_soil_humidity.first &&
                    soil_humidity >= properties->min_soil_humidity)
            {
                score = calculate_lower_end_score(soil_humidity, properties->min_soil_humidity, properties->prime_soil_humidity.first);
            }
            else if(soil_humidity > properties->prime_soil_humidity.second &&
                    soil_humidity <= properties->max_soil_humidity)
            {
                score = calculate_upper_end_score(soil_humidity, properties->max_soil_humidity, properties->prime_soil_humidity.second);
            }
            cluster_score.soil_humidities[i] = score;
        }

        // Temperature
        {
            const TemperatureProperties * properties(specie_properties->temperature_properties);
            int score = 0;
            if(temperature >= properties->prime_temp.first &&
                    temperature <= properties->prime_temp.second)
            {
                score = 100;
            }
            else if(temperature < properties->prime_temp.first &&
                    temperature >= properties->min_temp)
            {
                score = calculate_lower_end_score(temperature, properties->min_temp, properties->prime_temp.first);
            }
            else if(temperature > properties->prime_temp.second &&
                    temperature <= properties->max_temp)
            {
                score = calculate_upper_end_score(temperature, properties->max_temp, properties->prime_temp.second);
            }
            cluster_score.temperature[i] = score;
        }

        // Aggregates
        cluster_score.avg_temp += cluster_score.temperature[i];
        if(i == 0 || cluster_score.temperature[i] < cluster_score.min_temp)
            cluster_score.min_temp = cluster_score.temperature[i];
        if(i == 0 || cluster_score.temperature[i] > cluster_score.max_temp)
            cluster_score.max_temp = cluster_score.temperature[i];

        cluster_score.avg_illumination += cluster_score.illumination[i];
        if(i == 0 || cluster_score.illumination[i] < cluster_score.min_illumination)
            cluster_score.min_illumination = cluster_score.illumination[i];
        if(i == 0 || cluster_score.illumination[i] > cluster_score.max_illumination)
            cluster_score.max_illumination = cluster_score.illumination[i];

        cluster_score.avg_humidity += cluster_score.soil_humidities[i];
        if(i == 0 || cluster_score.soil_humidities[i] < cluster_score.min_humidity)
            cluster_score.min_humidity = cluster_score.soil_humidities[i];
        if(i == 0 || cluster_score.soil_humidities[i] > cluster_score.max_humidity)
            cluster_score.max_humidity = cluster_score.soil_humidities[i];
    }
    // Avg aggregates
    cluster_score.avg_temp = std::round(cluster_score.avg_temp/12.f);
    cluster_score.avg_illumination = std::round(cluster_score.avg_illumination/12.f);
    cluster_score.avg_humidity = std::round(cluster_score.avg_humidity/12.f);

    // TODO: Slope
    {
        cluster_score.slope = 100;
    }

    // Aggregate avg
    cluster_score.aggregate_avg = std::round((cluster_score.avg_temp + cluster_score.avg_illumination + cluster_score.avg_humidity + cluster_score.slope) / 4.f);

//    qCritical() << "<-- SCORE SUMMARY [" << specie_properties->specie_name << "] --> ";
//    qCritical() << "--Illumination--";
//    for(int i(0); i < 12; i++)
//        qCritical() << "[" << i << " --> " << cluster_score.illumination[i];
//    qCritical() << "MIN --> " << cluster_score.min_illumination;
//    qCritical() << "--Temperature--";
//    for(int i(0); i < 12; i++)
//        qCritical() << "[" << i << " --> " << cluster_score.temperature[i];
//    qCritical() << "MIN --> " << cluster_score.min_temp;
//    qCritical() << "--Soil Humidity--";
//    for(int i(0); i < 12; i++)
//        qCritical() << "[" << i << " --> " << cluster_score.soil_humidities[i];
//    qCritical() << "MIN --> " << cluster_score.min_humidity;
//    qCritical() << "--Slope --> " << cluster_score.slope;

    return cluster_score;
}

std::vector<int> TerrainSuitabilityScore::getValidClusters()
{
    return m_valid_clusters;
}

TerrainSuitabilityScore::Score TerrainSuitabilityScore::operator[](int cluster_idx)
{
    return m_per_cluster_scores.at(cluster_idx);
}

int TerrainSuitabilityScore::n_clusters() const
{
    return m_per_cluster_scores.size();
}

int TerrainSuitabilityScore::getMaxSuitabilityScore() const
{
    return m_max_suitability_score;
}

int TerrainSuitabilityScore::getMaxSuitabilityClusterIdx() const
{
    return m_max_suitability_cluster_idx;
}

int TerrainSuitabilityScore::getMinSuitabilityScore() const
{
    return m_min_suitability_score;
}

int TerrainSuitabilityScore::getMinSuitabilityClusterIdx() const
{
    return m_min_suitability_cluster_idx;
}

/*******************************
 * SPECIE PROPERTIES LIST ITEM *
 *******************************/
SpeciePropertiesListItem::SpeciePropertiesListItem ( const SpecieProperties * specie_properties, TerrainSuitabilityScore suitability_score) :
    QListWidgetItem(), m_specie_properties(nullptr), m_suitability_score(suitability_score)
{
    setSpecieProperties(specie_properties);

    float green_ratio(suitability_score.getMaxSuitabilityScore()/100.f);
    float red_ratio(1-green_ratio);
    QColor color(red_ratio*255, green_ratio*255, 0);
    setBackground(color);
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

const SpecieProperties * SpeciePropertiesListItem::getProperties() const
{
    return m_specie_properties;
}

TerrainSuitabilityScore SpeciePropertiesListItem::getSuitabilityScore() const
{
    return m_suitability_score;
}

bool SpeciePropertiesListItem::operator<(const QListWidgetItem & other) const
{
    const SpeciePropertiesListItem & _other = static_cast<const SpeciePropertiesListItem &>(other);
    return m_suitability_score.getMaxSuitabilityScore() < _other.getSuitabilityScore().getMaxSuitabilityScore();
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

void SpeciePropertiesListWidget::unselect()
{
    int current_row(m_list->currentRow());
    if(current_row >= 0)
        m_list->item(current_row)->setSelected(false);
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

/********************
 * HISTOGRAM WIDGET *
 ********************/
const QFont HistogramWidget::_LABEL_FONT("Times", 10, QFont::Weight::Bold);
HistogramWidget::HistogramWidget(QString heading, QWidget * parent) : QWidget(parent),
    m_heading(heading), m_value(0), m_heading_lbl(new QLabel(heading, this))
{
    m_heading_lbl->setFont(HistogramWidget::_LABEL_FONT);
    init_layout();
}

HistogramWidget::~HistogramWidget()
{

}

void HistogramWidget::paintEvent(QPaintEvent * event)
{
    QPainter p(this);

    // Calculate color
    float green_ratio(m_value/100.f);
    float red_ratio(1-green_ratio);
    QColor fill_color(red_ratio*255, green_ratio*255, 0);

    int lbl_height(m_heading_lbl->height()*2);
    int w(width());
    int h(height()-lbl_height);

    int h_ratio(std::round((m_value/100.f) * h));

    QPoint top_left(0, h-h_ratio);
    QPoint bottom_right(w,h);
    QRect rect(top_left, bottom_right);
    p.fillRect(rect, fill_color);

    // Draw text
    QFontMetrics fm(p.font());
    QString value_txt(QString::number(m_value));
    value_txt.append("%");
    int text_h(fm.height());
    QRect text_rect(top_left, QPoint(w, top_left.y()+text_h));
    p.drawText(text_rect, Qt::AlignCenter, value_txt);

    p.end();
}

void HistogramWidget::setValue(int value)
{
    m_value = value;
}

void HistogramWidget::clear()
{
    setValue(0);
}

void HistogramWidget::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;

    main_layout->addWidget(new QLabel, 1); // Padding

    {
        QHBoxLayout * layout = new QHBoxLayout;
        layout->addWidget(m_heading_lbl, 1, Qt::AlignCenter);
        main_layout->addLayout(layout,0);
    }

    setLayout(main_layout);
}

/*****************************
 * HISTOGRAM WRAPPER WIDGETS *
 *****************************/
HistogramWrapperWidget::HistogramWrapperWidget(std::vector<QString> headings, QWidget * parent ) : QWidget(parent)
{
    init_layout(headings);
}

HistogramWrapperWidget::~HistogramWrapperWidget()
{

}

HistogramWidget* HistogramWrapperWidget::operator[](int index)
{
    return m_histograms.at(index);
}

void HistogramWrapperWidget::init_layout(std::vector<QString> & headings)
{
    QHBoxLayout * main_layout = new QHBoxLayout;
    for(int i(0); i < headings.size(); i++)
    {
        HistogramWidget * w = new HistogramWidget(headings.at(i), this);
        m_histograms.push_back(w);
        main_layout->addWidget(w);
    }
    setLayout(main_layout);
}

void HistogramWrapperWidget::setScores(int * scores)
{
    for(int i = 0; i < m_histograms.size(); i++)
        m_histograms.at(i)->setValue(scores[i]);
}

void HistogramWrapperWidget::clear()
{
    for(int i = 0; i < m_histograms.size(); i++)
        m_histograms.at(i)->clear();
}

std::vector<QString> HistogramWrapperWidget::getMonthTitles()
{
    std::vector<QString> ret;
    ret.push_back("Jan");
    ret.push_back("Feb");
    ret.push_back("Mar");
    ret.push_back("Apr");
    ret.push_back("May");
    ret.push_back("Jun");
    ret.push_back("Jul");
    ret.push_back("Aug");
    ret.push_back("Sep");
    ret.push_back("Oct");
    ret.push_back("Nov");
    ret.push_back("Dec");

    return ret;
}

/********************************
 * AGGREGATE HISTOGRAMS WRAPPER *
 ********************************/
AggregateHistogramWrapperWidget::AggregateHistogramWrapperWidget(QWidget * parent) : HistogramWrapperWidget(AggregateHistogramWrapperWidget::getTitles(), parent) {}
AggregateHistogramWrapperWidget::~AggregateHistogramWrapperWidget() {}
std::vector<QString> AggregateHistogramWrapperWidget::getTitles()
{
    std::vector<QString> ret;
    ret.push_back("Slope");
    ret.push_back("Temp");
    ret.push_back("Illumination");
    ret.push_back("Soil Humidity");

    return ret;
}

/****************
 * SCORE WIDGET *
 ****************/
ScoreSummaryWidget::ScoreSummaryWidget(QWidget * parent ) : QWidget(parent),
    m_selected_cluster_idx(0),
    m_cluster_selection_combo_box(new QComboBox(this)),
    m_type_selection_combo_box(new QComboBox(this))
{
    for(int i = 0; i < Type::_COUNT; i++)
    {
        if(i < Type::_TEMP)
            m_aggregate_histogram_widgets[i] = new AggregateHistogramWrapperWidget(this);
        else
            m_aggregate_histogram_widgets[i] = new HistogramWrapperWidget(HistogramWrapperWidget::getMonthTitles(), this);
    }
    {
        m_type_selection_combo_box->insertItem(Type::_AVG, "Average");
        m_type_selection_combo_box->insertItem(Type::_MIN, "Min.");
        m_type_selection_combo_box->insertItem(Type::_MAX, "Max.");
        m_type_selection_combo_box->insertItem(Type::_TEMP, "Temp");
        m_type_selection_combo_box->insertItem(Type::_ILLUMINATION, "Illumination");
        m_type_selection_combo_box->insertItem(Type::_SOIL_HUMIDITY, "Soil Humidity");
    }

    init_layout();
    init_connections();
    setType(Type::_AVG);
}

ScoreSummaryWidget::~ScoreSummaryWidget()
{

}

void ScoreSummaryWidget::init_layout()
{
    QHBoxLayout * main_layout = new QHBoxLayout;

    //Add the combo box
    {
        QVBoxLayout * v_layout = new QVBoxLayout;

        // Cluster selection combo box
        {
            QHBoxLayout * h_layout = new QHBoxLayout;
            h_layout->addWidget(new QLabel("Cluster: "));
            h_layout->addWidget(m_cluster_selection_combo_box);

            v_layout->addLayout(h_layout, 0);
        }
        // Info type
        {
            QHBoxLayout * h_layout = new QHBoxLayout;
            h_layout->addWidget(new QLabel("Resource: "));
            h_layout->addWidget(m_type_selection_combo_box);

            v_layout->addLayout(h_layout, 0);
        }
        v_layout->addWidget(new QLabel, 1);

        main_layout->addLayout(v_layout, 0);
    }

    // The histogram wrappers
    {
        QGridLayout * layout = new QGridLayout;
        for(int i = 0; i < Type::_COUNT; i++)
        {
            layout->addWidget(m_aggregate_histogram_widgets[i],0,0,1,1);
        }
        main_layout->addLayout(layout, 1);
    }
    setLayout(main_layout);
}

void ScoreSummaryWidget::init_connections()
{
    connect(m_cluster_selection_combo_box, SIGNAL(activated(int)), this, SLOT(setClusterIdx(int)));
    connect(m_type_selection_combo_box, SIGNAL(activated(int)), this, SLOT(setType(int)));
}

void ScoreSummaryWidget::setSelected(QListWidgetItem * selected_plant)
{
    if(selected_plant)
    {
        m_score = static_cast<SpeciePropertiesListItem*>(selected_plant)->getSuitabilityScore();
        m_cluster_selection_combo_box->clear();

        for(int i(0); i < m_score.n_clusters(); i++)
        {
            QString item("Cluster ");
            item.append(QString::number(i));
            m_cluster_selection_combo_box->addItem(item);
        }

        if(m_score.n_clusters() > 1)
        {
            m_cluster_selection_combo_box->setItemData(m_score.getMaxSuitabilityClusterIdx(), QColor(Qt::green), Qt::BackgroundRole);
            m_cluster_selection_combo_box->setItemData(m_score.getMinSuitabilityClusterIdx(), QColor(Qt::red), Qt::BackgroundRole);
        }

        refreshHistograms();
        repaint();
    }
    else
        clear();
}

void ScoreSummaryWidget::setType(int type)
{
    m_selected_type = static_cast<Type>(type);
    hide_all();
    m_aggregate_histogram_widgets[type]->setVisible(true);
    repaint();
}

void ScoreSummaryWidget::hide_all()
{
    for(int i = 0; i < Type::_COUNT; i++)
    {
        m_aggregate_histogram_widgets[i]->setVisible(false);
    }
    repaint();
}

void ScoreSummaryWidget::setClusterIdx(int cluster_idx)
{
    m_selected_cluster_idx = cluster_idx;
    refreshHistograms();
    repaint();
}

void ScoreSummaryWidget::refreshHistograms()
{
    TerrainSuitabilityScore::Score score = m_score[m_selected_cluster_idx];

    // Averages
    {
        int scores[4];
        scores[AggregateHistogramWrapperWidget::_SLOPE] = score.slope;
        scores[AggregateHistogramWrapperWidget::_TEMP] = score.avg_temp;
        scores[AggregateHistogramWrapperWidget::_ILLUMINATION] = score.avg_illumination;
        scores[AggregateHistogramWrapperWidget::_HUMIDITY] = score.avg_humidity;
        m_aggregate_histogram_widgets[Type::_AVG]->setScores(scores);
    }
    // Minimums
    {
        int scores[4];
        scores[AggregateHistogramWrapperWidget::_SLOPE] = score.slope;
        scores[AggregateHistogramWrapperWidget::_TEMP] = score.min_temp;
        scores[AggregateHistogramWrapperWidget::_ILLUMINATION] = score.min_illumination;
        scores[AggregateHistogramWrapperWidget::_HUMIDITY] = score.min_humidity;
        m_aggregate_histogram_widgets[Type::_MIN]->setScores(scores);
    }
    // Maximums
    {
        int scores[4];
        scores[AggregateHistogramWrapperWidget::_SLOPE] = score.slope;
        scores[AggregateHistogramWrapperWidget::_TEMP] = score.max_temp;
        scores[AggregateHistogramWrapperWidget::_ILLUMINATION] = score.max_illumination;
        scores[AggregateHistogramWrapperWidget::_HUMIDITY] = score.max_humidity;
        m_aggregate_histogram_widgets[Type::_MAX]->setScores(scores);
    }
    // Temp
    {
        m_aggregate_histogram_widgets[Type::_TEMP]->setScores(score.temperature);
    }
    // Illumination
    {
        m_aggregate_histogram_widgets[Type::_ILLUMINATION]->setScores(score.illumination);
    }
    // Soil humidity
    {
        m_aggregate_histogram_widgets[Type::_SOIL_HUMIDITY]->setScores(score.soil_humidities);
    }
}

void ScoreSummaryWidget::clear()
{
    for(int i = 0; i < Type::_COUNT; i++)
        m_aggregate_histogram_widgets[i]->clear();
}

/**************************
 * PLANT SELECTION DIALOG *
 **************************/
const QFont PlantSelectionDialog::_LABEL_FONT("Times", 15, QFont::Weight::Bold);
PlantSelectionDialog::PlantSelectionDialog( QWidget * parent ) : QDialog(parent),
  m_available_plants_widget ( new SpeciePropertiesListWidget("Available", this) ),
  m_added_plants_widget ( new SpeciePropertiesListWidget("Added", this) ),
  m_score_summary_widget( new ScoreSummaryWidget(this) ),
  m_add_btn ( new QPushButton(">>", this) ),
  m_remove_btn ( new QPushButton("<<", this) ),
  m_filter_le (new QLineEdit(this) )
{
    setWindowTitle("Plant Selection");
    setFixedSize(1200,800);
    init_layout();
    init_connections();

//    ClusterData dummy_cluster_1;
//    ClusterData dummy_cluster_2;
//    dummy_cluster_1.slope = 0;
//    dummy_cluster_2.slope = 0;
//    for(int i(0); i < 12; i++)
//    {
//        dummy_cluster_1.illumination[i] = 14;
//        dummy_cluster_2.illumination[i] = std::min(14,5+i);

//        dummy_cluster_1.soil_humidities[i] = 70;
//        dummy_cluster_2.soil_humidities[i] = 45+i;

//        dummy_cluster_1.temperatures[i] = 28;
//        dummy_cluster_2.temperatures[i] = 20+i;
//    }
//    std::vector<ClusterData> dummy_clusters{dummy_cluster_1, dummy_cluster_2};
//    setClusters(dummy_clusters);
}

PlantSelectionDialog::~PlantSelectionDialog()
{
    remove_all_list_items();
}

void PlantSelectionDialog::setClusters(std::vector<ClusterData> clusters)
{
    m_cluster_data = clusters;
    refresh();
}

void PlantSelectionDialog::refresh()
{
    remove_all_list_items();

    PlantDB::SpeciePropertiesHolder all_species(PlantDB().getAllPlantData());
    for(auto it(all_species.begin()); it != all_species.end(); it++)
    {
        TerrainSuitabilityScore suitability_score(it->second, m_cluster_data);
        if(suitability_score.getValidClusters().size() > 0) // No point adding plants that can grow in no clusters
            m_available_plants_widget->m_list->addItem( new SpeciePropertiesListItem(it->second, suitability_score) );
    }

    m_available_plants_widget->m_list->sortItems(Qt::SortOrder::DescendingOrder);
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

    if(count > 0)
    {
        QListWidgetItem* selected_item(m_available_plants_widget->m_list->takeItem(m_available_plants_widget->m_list->row(selected_items.at(0))));
        m_added_plants_widget->m_list->addItem(selected_item);
        int row(m_added_plants_widget->m_list->row(selected_item));
        m_added_plants_widget->m_list->item(row)->setSelected(true);
        m_added_plants_widget->m_list->sortItems(Qt::SortOrder::DescendingOrder);
    }
}

void PlantSelectionDialog::remove_selected_plant()
{
    QList<QListWidgetItem*> selected_items (m_added_plants_widget->m_list->selectedItems());
    bool count(selected_items.size());

    if(count > 0)
    {
        QListWidgetItem* selected_item(m_added_plants_widget->m_list->takeItem(m_added_plants_widget->m_list->row(selected_items.at(0))));
        m_available_plants_widget->m_list->addItem(selected_item);
        int row(m_available_plants_widget->m_list->row(selected_item));
        m_available_plants_widget->m_list->item(row)->setSelected(true);
        m_available_plants_widget->m_list->sortItems(Qt::SortOrder::DescendingOrder);
    }
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

    // Score summary widget
    {
        // Heading
        {
            QHBoxLayout * layout = new QHBoxLayout;
            QLabel * title = new QLabel("SUITABILITY", this);
            title->setFont(PlantSelectionDialog::_LABEL_FONT);
            layout->addWidget(title, 1, Qt::AlignCenter);
            main_layout->addLayout(layout);
        }
        main_layout->addWidget(m_score_summary_widget);
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
    connect(m_available_plants_widget->m_list, SIGNAL(itemClicked(QListWidgetItem*)), m_score_summary_widget, SLOT(setSelected(QListWidgetItem*)));
    connect(m_added_plants_widget->m_list, SIGNAL(itemClicked(QListWidgetItem*)), m_score_summary_widget, SLOT(setSelected(QListWidgetItem*)));

    connect(m_available_plants_widget->m_list, SIGNAL(itemClicked(QListWidgetItem*)), m_added_plants_widget, SLOT(unselect()));
    connect(m_added_plants_widget->m_list, SIGNAL(itemClicked(QListWidgetItem*)), m_available_plants_widget, SLOT(unselect()));

    connect(m_add_btn, SIGNAL(clicked(bool)), this, SLOT(add_selected_plant()));
    connect(m_remove_btn, SIGNAL(clicked(bool)), this, SLOT(remove_selected_plant()));
    connect(m_filter_le, SIGNAL(textEdited(QString)), this, SLOT(refresh_filter(QString)));
}
