#ifndef CLUSTERING_CONTROLLER_H
#define CLUSTERING_CONTROLLER_H

#include "base_slider_controller.h"

/********************************
 * CLUSTERING CONTROLLER WIDGET *
 ********************************/
class ClusteringSlider : public QSlider
{
public:
    ClusteringSlider(QWidget * parent = 0);
    ~ClusteringSlider();
};
class ClusteringControllerWidget : public BaseSliderControllerWidget
{
    Q_OBJECT
public:
    ClusteringControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~ClusteringControllerWidget();
    int getSensitivity() const;

protected slots:
    virtual void refresh_label(int value);
};

#endif // CLUSTERING_CONTROLLER_H
