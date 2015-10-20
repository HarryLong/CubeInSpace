#include "plant_selection_widget.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QComboBox>

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

            if(m_score.n_clusters() > 1)
            {
                if(m_score.getMaxSuitabilityClusterIdx() == i)
                    item.append(" +");
                if(m_score.getMinSuitabilityClusterIdx() == i)
                    item.append(" -");
            }
            m_cluster_selection_combo_box->addItem(item);

            if(m_score.n_clusters() > 1)
            {
                TerrainSuitabilityScore::Score score(m_score[i]);
                if(score.slope == 0 || score.min_humidity == 0 || score.min_illumination == 0 || score.min_temp == 0)
                    m_cluster_selection_combo_box->setItemData(i, QColor(Qt::red), Qt::BackgroundRole);
                else
                    m_cluster_selection_combo_box->setItemData(i, QColor(Qt::green), Qt::BackgroundRole);
            }
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
        scores[AggregateHistogramWrapperWidget::_TEMP] = std::round(score.avg_temp);
        scores[AggregateHistogramWrapperWidget::_ILLUMINATION] = std::round(score.avg_illumination);
        scores[AggregateHistogramWrapperWidget::_HUMIDITY] = std::round(score.avg_humidity);
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

/********************
 * SEARCH LINE EDIT *
 ********************/
SearchLineEdit::SearchLineEdit( QWidget * parent ) : QLineEdit(parent)
{
    setPlaceholderText("Search");
}

SearchLineEdit::~SearchLineEdit()
{

}


/**************************
 * PLANT SELECTION DIALOG *
 **************************/
const QFont PlantSelectionWidget::_LABEL_FONT("Times", 15, QFont::Weight::Bold);
PlantSelectionWidget::PlantSelectionWidget( QWidget * parent ) : QWidget(parent),
  m_available_plants_widget ( new SpecieSuitabilityListWidget(this) ),
  m_added_plants_widget ( new SpecieSuitabilityListWidget(this) ),
  m_score_summary_widget( new ScoreSummaryWidget(this) ),
  m_add_btn ( new QPushButton(">>", this) ),
  m_remove_btn ( new QPushButton("<<", this) ),
  m_filter_le (new SearchLineEdit(this) )
{
    setWindowTitle("Plant Selection");
    setFixedSize(1200,800);
    init_layout();
    init_connections();
}

PlantSelectionWidget::~PlantSelectionWidget()
{
    remove_all_list_items();
}

void PlantSelectionWidget::setClusters(std::vector<ClusterData> clusters)
{
    m_cluster_data = clusters;
    refresh();
}

void PlantSelectionWidget::refresh()
{
    remove_all_list_items();

    PlantDB::SpeciePropertiesHolder all_species(PlantDB().getAllPlantData());
    for(auto it(all_species.begin()); it != all_species.end(); it++)
    {
        TerrainSuitabilityScore suitability_score(it->second, m_cluster_data);
        if(suitability_score.getValidClusters().size() > 0) // No point adding plants that can grow in no clusters
            m_available_plants_widget->addItem( new SpeciePropertiesListItem(it->second, suitability_score) );
    }

    m_available_plants_widget->sortItems(Qt::SortOrder::DescendingOrder);
}

void PlantSelectionWidget::remove_all_list_items()
{
    m_available_plants_widget->clear();
    m_added_plants_widget->clear();
}

void PlantSelectionWidget::add_selected_plant()
{
    QList<QListWidgetItem*> selected_items (m_available_plants_widget->selectedItems());

    bool count(selected_items.size());

    if(count > 0)
    {
        QListWidgetItem* selected_item(m_available_plants_widget->takeItem(m_available_plants_widget->row(selected_items.at(0))));
        m_added_plants_widget->addItem(selected_item);
        int row(m_added_plants_widget->row(selected_item));
        m_added_plants_widget->item(row)->setSelected(true);
        m_added_plants_widget->sortItems(Qt::SortOrder::DescendingOrder);
        m_available_plants_widget->unselect();
    }
}

void PlantSelectionWidget::remove_selected_plant()
{
    QList<QListWidgetItem*> selected_items (m_added_plants_widget->selectedItems());
    bool count(selected_items.size());

    if(count > 0)
    {
        QListWidgetItem* selected_item(m_added_plants_widget->takeItem(m_added_plants_widget->row(selected_items.at(0))));
        m_available_plants_widget->addItem(selected_item);
        int row(m_available_plants_widget->row(selected_item));
        m_available_plants_widget->item(row)->setSelected(true);
        m_available_plants_widget->sortItems(Qt::SortOrder::DescendingOrder);
        m_added_plants_widget->unselect();
    }
}

void PlantSelectionWidget::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;

    // Available plants & add/remove buttons & Added plants
    {
        QHBoxLayout * h_layout = new QHBoxLayout;

        // Available plants
        {
            QVBoxLayout * v_layout = new QVBoxLayout;
            {
                QHBoxLayout * h_layout_2 = new QHBoxLayout;
                QLabel * title = new QLabel("Available Plants");
                title->setFont(_LABEL_FONT);
                h_layout_2->addWidget(title, 1, Qt::AlignCenter);
                v_layout->addLayout(h_layout_2);
            }
            v_layout->addWidget(m_available_plants_widget, 1);
            h_layout->addLayout(v_layout);
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
            QVBoxLayout * v_layout = new QVBoxLayout;
            {
                QHBoxLayout * h_layout_2 = new QHBoxLayout;
                QLabel * title = new QLabel("Added Plants");
                title->setFont(_LABEL_FONT);
                h_layout_2->addWidget(title, 1, Qt::AlignCenter);
                v_layout->addLayout(h_layout_2);
            }
            v_layout->addWidget(m_added_plants_widget, 1);
            h_layout->addLayout(v_layout);
        }

        main_layout->addLayout(h_layout);
    }

    // Filter
    {
        m_filter_le->setMinimumWidth(20);
        QHBoxLayout * layout = new QHBoxLayout;

        layout->addWidget(m_filter_le,1, Qt::AlignCenter); //left-padding

        main_layout->addLayout(layout);
    }

    // Score summary widget
    {
        // Heading
        {
            QHBoxLayout * layout = new QHBoxLayout;
            QLabel * title = new QLabel("SUITABILITY", this);
            title->setFont(PlantSelectionWidget::_LABEL_FONT);
            layout->addWidget(title, 1, Qt::AlignCenter);
            main_layout->addLayout(layout);
        }
        main_layout->addWidget(m_score_summary_widget);
    }

    setLayout(main_layout);
}

void PlantSelectionWidget::refresh_filter(QString filter_text)
{
    m_available_plants_widget->filter(filter_text);
    m_added_plants_widget->filter(filter_text);
}

void PlantSelectionWidget::init_connections()
{
    connect(m_available_plants_widget, SIGNAL(itemClicked(QListWidgetItem*)), m_score_summary_widget, SLOT(setSelected(QListWidgetItem*)));
    connect(m_added_plants_widget, SIGNAL(itemClicked(QListWidgetItem*)), m_score_summary_widget, SLOT(setSelected(QListWidgetItem*)));

    connect(m_available_plants_widget, SIGNAL(itemClicked(QListWidgetItem*)), m_added_plants_widget, SLOT(unselect()));
    connect(m_added_plants_widget, SIGNAL(itemClicked(QListWidgetItem*)), m_available_plants_widget, SLOT(unselect()));

    connect(m_add_btn, SIGNAL(clicked(bool)), this, SLOT(add_selected_plant()));
    connect(m_remove_btn, SIGNAL(clicked(bool)), this, SLOT(remove_selected_plant()));
    connect(m_filter_le, SIGNAL(textEdited(QString)), this, SLOT(refresh_filter(QString)));
}
