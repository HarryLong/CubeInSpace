#ifndef CLUSTERING_CONTROLLER_H
#define CLUSTERING_CONTROLLER_H

#include "shortcut_controller_row.h"

/********************************
 * CLUSTERING CONTROLLER WIDGET *
 ********************************/
class ClusteringSensitivityRow : public ShortcutRow
{
public:
    ClusteringSensitivityRow(QWidget * parent = 0);
    ~ClusteringSensitivityRow();
};
class ClusteringControllerWidget : public QWidget
{
    Q_OBJECT
public:
    ClusteringControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~ClusteringControllerWidget();
    int getClusteringSensitivity() const;
    int alignment();

signals:
    void clusteringSensitivityChanged(int);

private slots:
    void emit_clustering_sensitivity_changed();

private:
    void init_layout();
    int m_alignment;
    ShortcutRow * m_clustering_sensitivity_shortcut;
};

#endif // CLUSTERING_CONTROLLER_H
