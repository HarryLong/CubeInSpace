#ifndef CLUSTER_INFO_DIALOG_H
#define CLUSTER_INFO_DIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "../clustering/cluster.h"
#include "custom_line_edit.h"

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

class ClusterControllerWidget : public QWidget
{
    Q_OBJECT
public:
    ClusterControllerWidget(QWidget * parent = 0);
    ~ClusterControllerWidget();
    int clusterCount();

signals:
    void refreshClusters(int);

private slots:
    void emit_refresh_clusters();

private:
    void init_layout();

    QIntLineEdit * m_cluster_count_le;
    QPushButton * m_refresh_btn;
};

class ClusterControllerDialog : public QDialog
{
Q_OBJECT
 public:
    ClusterControllerDialog(QWidget * parent);
    ~ClusterControllerDialog();

    void setClusters(const Clusters & clusters);
    bool containsData();
    int clusterCount();

signals:
    void refresh_clusters(int);

public slots:
    void clear();

protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void closeEvent(QCloseEvent * event);

private slots:
    void emit_refresh_clusters(int);

private:
    void init_layout();
    bool m_valid;

    ClusterInfoTable * m_cluster_info_table;
    ClusterControllerWidget * m_cluster_controller_widget;
};

#endif // CLUSTER_INFO_DIALOG_H
