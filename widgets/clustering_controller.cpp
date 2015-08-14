#include "clustering_controller.h"

/********************************
 * CLUSTERING CONTROLLER WIDGET *
 ********************************/
#define DEFAULT_SENSITIVITY 10
ClusteringSlider::ClusteringSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(1,50);
    setTickInterval(5);
    setValue(DEFAULT_SENSITIVITY);
}

ClusteringSlider::~ClusteringSlider()
{

}

ClusteringControllerWidget::ClusteringControllerWidget(int alignment, QWidget * parent, Qt::WindowFlags f) :
    BaseSliderControllerWidget(new ClusteringSlider, "SENSITIVITY", alignment, parent, f)
{
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
    refresh_label(m_slider->value());
}

ClusteringControllerWidget::~ClusteringControllerWidget()
{

}

int ClusteringControllerWidget::getSensitivity() const
{
    return value();
}

void ClusteringControllerWidget::refresh_label(int value)
{
    m_value_label->setText(QString::number(value));
}
