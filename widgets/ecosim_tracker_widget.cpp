#include "ecosim_tracker_widget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QProgressBar>

SingleSimTrackerWidget::SingleSimTrackerWidget(int cluster_idx, QWidget * parent) : QWidget(parent),
    m_cluster_idx(cluster_idx), m_progress_bar(new QProgressBar(this))
{
    QPalette pal(palette());
    // set black background
    pal.setColor(QPalette::Background, Qt::red);
    setAutoFillBackground(true);
    setPalette(pal);

    init_layout();
}

SingleSimTrackerWidget::~SingleSimTrackerWidget()
{

}

void SingleSimTrackerWidget::reset()
{
    m_progress_bar->setValue(0);
}

void SingleSimTrackerWidget::init_layout()
{
    QBoxLayout * layout = new QHBoxLayout;

    {
        QLabel * lbl = new QLabel(QString::number(m_cluster_idx));
        layout->addWidget(lbl,1);
    }

    {
        m_progress_bar->setRange(0,100);
        layout->addWidget(m_progress_bar,0);
    }

    setLayout(layout);
}

//------------------------------------------------------------------------------------

EcosimTrackerWidget::EcosimTrackerWidget(QWidget * parent) : QWidget(parent)
{
    init_layout();
    reset();
}

EcosimTrackerWidget::~EcosimTrackerWidget()
{

}

void EcosimTrackerWidget::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;
    for(int i(0); i < KMeansClusterer::_MAX_CLUSTERS; i++)
    {
        SingleSimTrackerWidget * tracker = new SingleSimTrackerWidget(i, this);
        m_trackers[i] = tracker;

        QHBoxLayout * h_layout(new QHBoxLayout);
        h_layout->addWidget(tracker, 10);

        layout->addLayout(h_layout);
    }

    layout->addWidget(new QLabel, 1); // Padding
    setLayout(layout);
}

void EcosimTrackerWidget::track(std::vector<EcoSimRunConfig> run_configs)
{
    reset();

    // Create a Widget for each cluster
    for(int i(0); i < run_configs.size(); i++)
        m_trackers[i]->setVisible(true);

    // TODO: Actually start the simulations...
}

void EcosimTrackerWidget::reset()
{
    for(int i(0); i < KMeansClusterer::_MAX_CLUSTERS; i++)
    {
        m_trackers[i]->reset();
        m_trackers[i]->setVisible(true);
    }
}
