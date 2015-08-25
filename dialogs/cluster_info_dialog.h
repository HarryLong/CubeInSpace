#ifndef CLUSTER_INFO_DIALOG_H
#define CLUSTER_INFO_DIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "../clustering/cluster.h"

class ClusterInfoTable : public QTableWidget
{
public:
    enum Rows{
        _CLUSTER_ID = 0,
        _COLOR,
        _MEMBER_COUNT,
        _SLOPE,
        _TEMP_JUN,
        _TEMP_DEC,
        _MIN_ILLUMINATION,
        _MAX_ILLUMINATION,
        _SOIL_HUMIDITY_JAN,
        _SOIL_HUMIDITY_FEB,
        _SOIL_HUMIDITY_MAR,
        _SOIL_HUMIDITY_APR,
        _SOIL_HUMIDITY_MAY,
        _SOIL_HUMIDITY_JUN,
        _SOIL_HUMIDITY_JUL,
        _SOIL_HUMIDITY_AUG,
        _SOIL_HUMIDITY_SEP,
        _SOIL_HUMIDITY_OCT,
        _SOIL_HUMIDITY_NOV,
        _SOIL_HUMIDITY_DEC,
        _ROW_COUNT
    };

    ClusterInfoTable(QWidget * parent);
    ~ClusterInfoTable();

    void addCluster(ClusterData cluster_data, int cluster_id);

private:
    static const QColor _CLUSTER_COLORS[216];
    QTableWidgetItem * generate_read_only_cell(QString p_cell_content);
};

class ClusterInfoDialog : public QDialog
{
 public:
    ClusterInfoDialog(QWidget * parent);
    ~ClusterInfoDialog();

    void setClusters(const Clusters & clusters);

private:
    void init_layout();

    ClusterInfoTable * m_cluster_info_table;
};

#endif // CLUSTER_INFO_DIALOG_H
