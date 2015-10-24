#include "cluster_controller_dialog.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include "../clustering/k_means_clusterer.h"
#include <QCloseEvent>
#include <QKeyEvent>

const QColor ClusterInfoTable::_CLUSTER_COLORS[216] = {
    QColor(204,102,0), QColor(153,102,102), QColor(0,51,102), QColor(255,255,102), QColor(0,153,255), QColor(255,0,0),
    QColor(204,51,0), QColor(255,102,204), QColor(0,204,102), QColor(204,102,255), QColor(102,204,153), QColor(153,255,255),
    QColor(153,204,153), QColor(153,0,0), QColor(255,255,255), QColor(153,255,153), QColor(153,102,153), QColor(204,255,153),
    QColor(255,255,0), QColor(153,51,51), QColor(204,153,0), QColor(153,255,102), QColor(51,0,204), QColor(204,255,102),
    QColor(102,51,204), QColor(204,204,153), QColor(204,0,102), QColor(153,0,153), QColor(102,255,51), QColor(0,153,102),
    QColor(204,102,153), QColor(255,0,255), QColor(0,255,102), QColor(153,153,153), QColor(0,0,255), QColor(102,255,0),
    QColor(204,0,255), QColor(255,204,153), QColor(0,102,153), QColor(51,102,153), QColor(153,153,204), QColor(153,255,51),
    QColor(102,0,0), QColor(102,0,51), QColor(102,102,0), QColor(51,255,51), QColor(102,51,102), QColor(102,153,204),
    QColor(102,255,102), QColor(153,0,102), QColor(153,51,0), QColor(51,204,0), QColor(204,153,255), QColor(51,0,51),
    QColor(51,255,153), QColor(255,204,102), QColor(204,255,204), QColor(0,51,255), QColor(51,153,102), QColor(255,51,153),
    QColor(153,102,255), QColor(204,102,102), QColor(204,51,255), QColor(0,0,0), QColor(51,51,0), QColor(255,153,204),
    QColor(0,102,255), QColor(153,204,0), QColor(204,0,51), QColor(0,0,153), QColor(0,204,153), QColor(153,153,255),
    QColor(0,153,153), QColor(102,204,255), QColor(0,204,0), QColor(255,0,102), QColor(51,255,0), QColor(102,255,153),
    QColor(51,204,51), QColor(51,0,0), QColor(153,204,255), QColor(255,102,0), QColor(102,153,153), QColor(102,153,51),
    QColor(51,102,255), QColor(0,153,51), QColor(255,51,255), QColor(153,153,102), QColor(102,153,255), QColor(204,255,51),
    QColor(51,255,204), QColor(0,255,204), QColor(102,204,102), QColor(51,102,0), QColor(51,102,51), QColor(204,204,255),
    QColor(0,255,255), QColor(0,0,51), QColor(153,255,0), QColor(255,255,153), QColor(255,51,102), QColor(204,255,0),
    QColor(0,51,204), QColor(255,255,51), QColor(0,204,255), QColor(204,204,102), QColor(255,102,102), QColor(0,102,0),
    QColor(255,51,0), QColor(51,153,51), QColor(51,255,255), QColor(102,255,255), QColor(102,153,0), QColor(102,0,102),
    QColor(102,51,255), QColor(204,153,204), QColor(153,51,153), QColor(51,102,204), QColor(102,0,255), QColor(153,204,102),
    QColor(0,255,153), QColor(51,204,255), QColor(51,0,153), QColor(51,51,255), QColor(51,255,102), QColor(0,102,204),
    QColor(0,102,51), QColor(51,153,0), QColor(102,102,255), QColor(51,153,153), QColor(102,102,204), QColor(255,153,51),
    QColor(51,204,204), QColor(255,51,204), QColor(204,102,204), QColor(0,204,51), QColor(153,102,204), QColor(102,0,153),
    QColor(51,51,51), QColor(51,51,204), QColor(153,204,51), QColor(51,0,255), QColor(204,0,204), QColor(204,153,51),
    QColor(255,0,51), QColor(0,204,204), QColor(204,51,204), QColor(153,0,255), QColor(102,255,204), QColor(255,153,0),
    QColor(204,153,102), QColor(255,204,255), QColor(204,0,153), QColor(255,204,51), QColor(102,102,102), QColor(255,51,51),
    QColor(0,51,51), QColor(102,102,153), QColor(204,0,0), QColor(153,0,51), QColor(0,255,51), QColor(0,51,153),
    QColor(204,204,0), QColor(153,102,0), QColor(102,51,51), QColor(204,51,102), QColor(0,255,0), QColor(153,153,0),
    QColor(255,0,153), QColor(102,0,204), QColor(255,204,0), QColor(51,204,102), QColor(102,153,102), QColor(255,153,255),
    QColor(102,204,51), QColor(255,102,153), QColor(255,255,204), QColor(0,102,102), QColor(51,0,102), QColor(51,204,153),
    QColor(255,102,51), QColor(255,153,102), QColor(255,0,204), QColor(51,153,204), QColor(102,51,153), QColor(102,204,0),
    QColor(204,51,51), QColor(51,51,102), QColor(102,51,0), QColor(255,204,204), QColor(153,255,204), QColor(153,51,255),
    QColor(204,153,153), QColor(51,102,102), QColor(255,153,153), QColor(153,51,102), QColor(204,102,51), QColor(51,153,255),
    QColor(102,102,51), QColor(0,0,102), QColor(153,0,204), QColor(204,255,255), QColor(153,102,51), QColor(51,51,153),
    QColor(0,153,204), QColor(204,51,153), QColor(153,204,204), QColor(153,153,51), QColor(102,204,204), QColor(204,204,204),
    QColor(204,204,51), QColor(0,0,204), QColor(255,102,255), QColor(153,51,204), QColor(0,51,0), QColor(0,153,0)
};

ClusterInfoTable::RowIndices ClusterInfoTable::build_row_indices()
{
    RowIndices row_indices;
    int row_idx(0);
    row_indices.cluster_id = row_idx++;
    row_indices.color = row_idx++;
    row_indices.member_count = row_idx++;
    row_indices.slope = row_idx++;
    for(int i = 0; i < 12; i++)
        row_indices.temp[i] = row_idx++;
    for(int i = 0; i < 12; i++)
        row_indices.illumination[i] = row_idx++;
    for(int i = 0; i < 12; i++)
        row_indices.soil_humidity[i] = row_idx++;
    row_indices.row_count = row_idx;

    return row_indices;
}

ClusterInfoTable::RowIndices ClusterInfoTable::_ROW_INDICES = ClusterInfoTable::build_row_indices();
ClusterInfoTable::ClusterInfoTable(QWidget * parent) : QTableWidget(parent)
{
    ClusterInfoTable::build_row_indices();
    horizontalHeader()->setVisible(false);

    setRowCount(_ROW_INDICES.row_count);
    setColumnCount(0);

    setVerticalHeaderItem(_ROW_INDICES.cluster_id, new QTableWidgetItem("Cluster ID"));
    setVerticalHeaderItem(_ROW_INDICES.color, new QTableWidgetItem("Color"));
    setVerticalHeaderItem(_ROW_INDICES.member_count, new QTableWidgetItem("# members"));
    setVerticalHeaderItem(_ROW_INDICES.slope, new QTableWidgetItem("Slope"));
    for(int i = 0; i < 12; i++)
    {
        QString heading ("Temp [");
        heading.append(QString::number(i+1)).append("]");
        setVerticalHeaderItem(_ROW_INDICES.temp[i], new QTableWidgetItem(heading));
    }
    for(int i = 0; i < 12; i++)
    {
        QString heading ("Illumination [");
        heading.append(QString::number(i+1)).append("]");
        setVerticalHeaderItem(_ROW_INDICES.illumination[i], new QTableWidgetItem(heading));
    }
    for(int i = 0; i < 12; i++)
    {
        QString heading ("Soil Humdity [");
        heading.append(QString::number(i+1)).append("]");
        setVerticalHeaderItem(_ROW_INDICES.soil_humidity[i], new QTableWidgetItem(heading));
    }
}

ClusterInfoTable::~ClusterInfoTable()
{

}

void ClusterInfoTable::addCluster(ClusterData cluster_data, int cluster_id)
{
    // Cluster ID
    setItem(_ROW_INDICES.cluster_id, cluster_id, generate_read_only_cell(QString::number(cluster_id)));
    // Color
    {
        QTableWidgetItem * color_cell (generate_read_only_cell(""));
        color_cell->setBackgroundColor(ClusterInfoTable::_CLUSTER_COLORS[cluster_id]);
        setItem(_ROW_INDICES.color, cluster_id, color_cell);
    }
    // Member count
    setItem(_ROW_INDICES.member_count, cluster_id, generate_read_only_cell(QString::number(cluster_data.member_count)));
    // Slope
    setItem(_ROW_INDICES.slope, cluster_id, generate_read_only_cell(QString::number(cluster_data.slope)));
    // Temp
    for(int i(0); i < 12; i++)
    {
        setItem(i+_ROW_INDICES.temp[0], cluster_id, generate_read_only_cell(QString::number(cluster_data.temperatures[i])));
    }
    // Illumination
    for(int i(0); i < 12; i++)
    {
        setItem(i+_ROW_INDICES.illumination[0], cluster_id, generate_read_only_cell(QString::number(cluster_data.illumination[i])));
    }
    // Temp
    for(int i(0); i < 12; i++)
    {
        setItem(i+_ROW_INDICES.soil_humidity[0], cluster_id, generate_read_only_cell(QString::number(cluster_data.soil_humidities[i])));
    }
}

QTableWidgetItem * ClusterInfoTable::generate_read_only_cell(QString p_cell_content)
{
    QTableWidgetItem * cell = new QTableWidgetItem(p_cell_content);
    cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);

    return cell;
}

//------------------------------------------------------------------------------

ClusterControllerWidget::ClusterControllerWidget(QWidget * parent) :
    m_cluster_count_le( new QIntLineEdit(1, KMeansClusterer::_MAX_CLUSTERS, 1, parent)),
    m_refresh_btn(new QPushButton("Refresh"))
{
    init_layout();
    connect(m_refresh_btn, SIGNAL(clicked(bool)), this, SLOT(emit_refresh_clusters()));
}

ClusterControllerWidget::~ClusterControllerWidget()
{

}

int ClusterControllerWidget::clusterCount()
{
    return m_cluster_count_le->value();
}

void ClusterControllerWidget::emit_refresh_clusters()
{
    emit refreshClusters(clusterCount());
}

void ClusterControllerWidget::init_layout()
{
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(new QLabel("Cluster Count: "));
    layout->addWidget(m_cluster_count_le);
    layout->addWidget(m_refresh_btn);

    setLayout(layout);
}

//------------------------------------------------------------------------------

ClusterControllerDialog::ClusterControllerDialog(QWidget * parent) : QDialog(parent),
    m_cluster_info_table(new ClusterInfoTable(this)),
    m_cluster_controller_widget(new ClusterControllerWidget(this)),
    m_valid(false)
{
    setWindowTitle("Clusters");
    setFixedSize(800, 800);
    init_layout();

    connect(m_cluster_controller_widget, SIGNAL(refreshClusters(int)), this, SLOT(emit_refresh_clusters(int)));
}

ClusterControllerDialog::~ClusterControllerDialog()
{

}

void ClusterControllerDialog::emit_refresh_clusters(int count)
{
    emit refresh_clusters(count);
}

void ClusterControllerDialog::clear()
{
    m_cluster_info_table->clearContents();
    m_valid = false;
}

void ClusterControllerDialog::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(m_cluster_controller_widget, 0);
    layout->addWidget(m_cluster_info_table, 1);

    setLayout(layout);
}

bool ClusterControllerDialog::containsData()
{
    return m_valid;
}

void ClusterControllerDialog::setClusters(const Clusters &  clusters)
{
    int cluster_count(clusters.clusterCount());

    m_cluster_info_table->clearContents();
    m_cluster_info_table->setColumnCount(cluster_count);

    for(int cluster_id(0); cluster_id < cluster_count; cluster_id++)
    {
        ClusterData cluster_data(clusters.getClusterData(cluster_id));
        if(cluster_data.member_count > 0)
            m_cluster_info_table->addCluster(clusters.getClusterData(cluster_id), cluster_id);
    }

    m_valid = true;
}

void ClusterControllerDialog::keyPressEvent(QKeyEvent * event)
{
    if(event->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(event);
    else
        event->ignore();
}

void ClusterControllerDialog::closeEvent(QCloseEvent * event)
{
    event->ignore();
}

int ClusterControllerDialog::clusterCount()
{
    return m_cluster_controller_widget->clusterCount();
}
