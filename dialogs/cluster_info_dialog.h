#ifndef CLUSTER_INFO_DIALOG_H
#define CLUSTER_INFO_DIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "../clustering/cluster.h"

class ClusterInfoTable : public QTableWidget
{
public:
    struct RowIndices{
        int cluster_id;
        int color;
        int member_count;
        int slope;
        int temp[12];
        int illumination[12];
        int soil_humidity[12];
        int row_count;
    };

    ClusterInfoTable(QWidget * parent);
    ~ClusterInfoTable();

    void addCluster(ClusterData cluster_data, int cluster_id);

    static ClusterInfoTable::RowIndices build_row_indices();
private:
    static const QColor _CLUSTER_COLORS[216];
    QTableWidgetItem * generate_read_only_cell(QString p_cell_content);
    static RowIndices _ROW_INDICES;
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
