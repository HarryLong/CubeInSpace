#include "clustering_controller.h"
#include <QBoxLayout>

/********************************
 * CLUSTERING CONTROLLER WIDGET *
 ********************************/
#define DEFAULT_SENSITIVITY 10
ClusteringSensitivityRow::ClusteringSensitivityRow(QWidget * parent) :
    ShortcutRow("Sensitivity: ", 1, 50, parent)
{

}

ClusteringSensitivityRow::~ClusteringSensitivityRow()
{

}

ClusteringControllerWidget::ClusteringControllerWidget(int alignment, QWidget * parent, Qt::WindowFlags f) : QWidget(parent),
    m_clustering_sensitivity_shortcut(new ClusteringSensitivityRow(parent)),
    m_alignment(alignment)
{
    setFixedWidth(250);
    connect(m_clustering_sensitivity_shortcut->m_ok_btn, SIGNAL(clicked(bool)), this, SLOT(emit_clustering_sensitivity_changed()));
    init_layout();
}

ClusteringControllerWidget::~ClusteringControllerWidget()
{

}

int ClusteringControllerWidget::getClusteringSensitivity() const
{
    return m_clustering_sensitivity_shortcut->value();
}

int ClusteringControllerWidget::alignment()
{
    return m_alignment;
}

void ClusteringControllerWidget::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(m_clustering_sensitivity_shortcut,0);
    layout->addWidget(new QLabel(this),1); // PADDING

    setLayout(layout);
}

void ClusteringControllerWidget::emit_clustering_sensitivity_changed()
{
    emit clusteringSensitivityChanged(getClusteringSensitivity());
}
