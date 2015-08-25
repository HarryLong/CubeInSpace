#include "cluster_info_dialog.h"
#include <QBoxLayout>

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

ClusterInfoTable::ClusterInfoTable(QWidget * parent) : QTableWidget(parent)
{
    setWindowTitle("Clusters");

    setRowCount(ClusterInfoTable::Rows::_ROW_COUNT);
    setColumnCount(0);

    setVerticalHeaderItem(ClusterInfoTable::Rows::_CLUSTER_ID, new QTableWidgetItem("Cluster ID"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_COLOR, new QTableWidgetItem("Color"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_MEMBER_COUNT, new QTableWidgetItem("# members"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SLOPE, new QTableWidgetItem("Slope"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_TEMP_JUN, new QTableWidgetItem("Temp (Jun)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_TEMP_DEC, new QTableWidgetItem("Temp (Dec)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_MIN_ILLUMINATION, new QTableWidgetItem("Min Illumination"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_MAX_ILLUMINATION, new QTableWidgetItem("Max Illumination"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_JAN, new QTableWidgetItem("Soil Humidity (Jan)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_FEB, new QTableWidgetItem("Soil Humidity (Feb)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_MAR, new QTableWidgetItem("Soil Humidity (Mar)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_APR, new QTableWidgetItem("Soil Humidity (Apr)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_MAY, new QTableWidgetItem("Soil Humidity (May)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_JUN, new QTableWidgetItem("Soil Humidity (Jun)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_JUL, new QTableWidgetItem("Soil Humidity (Jul)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_AUG, new QTableWidgetItem("Soil Humidity (Aug)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_SEP, new QTableWidgetItem("Soil Humidity (Sep)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_OCT, new QTableWidgetItem("Soil Humidity (Oct)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_NOV, new QTableWidgetItem("Soil Humidity (Nov)"));
    setVerticalHeaderItem(ClusterInfoTable::Rows::_SOIL_HUMIDITY_DEC, new QTableWidgetItem("Soil Humidity (Dec)"));
}

ClusterInfoTable::~ClusterInfoTable()
{

}

void ClusterInfoTable::addCluster(ClusterData cluster_data, int cluster_id)
{
    // Cluster ID
    setItem(ClusterInfoTable::Rows::_CLUSTER_ID, cluster_id, generate_read_only_cell(QString::number(cluster_id)));
    // Color
    {
        QTableWidgetItem * color_cell (generate_read_only_cell(""));
        color_cell->setBackgroundColor(ClusterInfoTable::_CLUSTER_COLORS[cluster_id]);
        setItem(ClusterInfoTable::Rows::_COLOR, cluster_id, color_cell);
    }
    // Member count
    setItem(ClusterInfoTable::Rows::_MEMBER_COUNT, cluster_id, generate_read_only_cell(QString::number(cluster_data.member_count)));
    // Slope
    setItem(ClusterInfoTable::Rows::_SLOPE, cluster_id, generate_read_only_cell(QString::number(cluster_data.slope)));
    // Temp
    for(int i(ClusterInfoTable::Rows::_TEMP_JUN); i <= ClusterInfoTable::Rows::_TEMP_DEC; i++)
    {
        setItem(static_cast<ClusterInfoTable::Rows>(i), cluster_id, generate_read_only_cell(QString::number(cluster_data.temperatures[i-ClusterInfoTable::Rows::_TEMP_JUN])));
    }
    // Illumination
    for(int i(ClusterInfoTable::Rows::_MIN_ILLUMINATION); i <= ClusterInfoTable::Rows::_MAX_ILLUMINATION; i++)
    {
        setItem(static_cast<ClusterInfoTable::Rows>(i), cluster_id, generate_read_only_cell(QString::number(cluster_data.illumination[i-ClusterInfoTable::Rows::_MIN_ILLUMINATION])));
    }
    // Soil Humidity
    for(int i(ClusterInfoTable::Rows::_SOIL_HUMIDITY_JAN); i <= ClusterInfoTable::Rows::_SOIL_HUMIDITY_DEC; i++)
    {
        setItem(static_cast<ClusterInfoTable::Rows>(i), cluster_id, generate_read_only_cell(QString::number(cluster_data.soil_humidities[i-ClusterInfoTable::Rows::_SOIL_HUMIDITY_JAN])));
    }
}

QTableWidgetItem * ClusterInfoTable::generate_read_only_cell(QString p_cell_content)
{
    QTableWidgetItem * cell = new QTableWidgetItem(p_cell_content);
    cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);

    return cell;
}

ClusterInfoDialog::ClusterInfoDialog(QWidget * parent) : QDialog(parent),
    m_cluster_info_table(new ClusterInfoTable(this))
{
    setFixedSize(800, 800);
    init_layout();
}

ClusterInfoDialog::~ClusterInfoDialog()
{

}

void ClusterInfoDialog::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(m_cluster_info_table, 1);

    setLayout(layout);
}

void ClusterInfoDialog::setClusters(const Clusters &  clusters)
{
    int cluster_count(clusters.clusterCount());

    m_cluster_info_table->clearContents();
    m_cluster_info_table->setColumnCount(cluster_count);

    for(int cluster_id(0); cluster_id < cluster_count; cluster_id++)
    {
        m_cluster_info_table->addCluster(clusters.getClusterData(cluster_id), cluster_id);
    }
}
