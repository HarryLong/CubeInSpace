#include "controller_widgets.h"
#include "month_controller.h"
#include "latitude_controller.h"
#include "soil_infiltration_controller.h"
#include "soil_infiltration_shortcuts_widget.h"
#include "time_controller.h"
#include "water_shortcuts_controller.h"
#include "clustering_controller.h"
#include "mode_controller.h"
#include <QBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QAction>

/******************************
 * CONTROLLER WIDGETS WRAPPER *
 ******************************/
OverlayWidgets::OverlayWidgets(QWidget * parent) :
    m_time_widget(new TimeControllerWidget(Qt::AlignRight, parent)),
    m_month_widget(new MonthControllerWidget(Qt::AlignLeft, parent)),
    m_latitude_widget(new LatitudeControllerWidget(Qt::AlignRight, parent)),
    m_soil_infiltration_widget(new SoilInfiltrationControllerWidget(Qt::AlignRight, parent)),
    m_soil_infiltration_shortcut_widget(new SoilInfiltrationShortcutWidget(Qt::AlignLeft, parent)),
    m_water_shortcut_widget(new WaterShortcutWidget(Qt::AlignLeft, parent)),
//    m_clustering_controller_widget(new ClusteringControllerWidget(Qt::AlignRight, parent)),
    m_previous_mode_controller_widget(new ModeControllerWidget(Qt::AlignLeft, "LEFT", parent)),
    m_next_mode_controller_widget(new ModeControllerWidget(Qt::AlignRight, "RIGHT", parent))
{
    m_alignment_sorted_widgets[Qt::AlignTop] = std::vector<QWidget*>();
    m_alignment_sorted_widgets[Qt::AlignRight] = std::vector<QWidget*>();
    m_alignment_sorted_widgets[Qt::AlignBottom] = std::vector<QWidget*>();
    m_alignment_sorted_widgets[Qt::AlignLeft] = std::vector<QWidget*>();

    // LATITUDE
    {
        connect(m_latitude_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_latitude_changed(int)));

        m_dynamic_display_widgets.push_back(m_latitude_widget);
        m_alignment_sorted_widgets[m_latitude_widget->alignment()].push_back(m_latitude_widget);
    }

    // TIME
    {
        connect(m_month_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_month_changed(int)));
        connect(m_time_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_time_changed(int)));

        m_dynamic_display_widgets.push_back(m_month_widget);
        m_dynamic_display_widgets.push_back(m_time_widget);

        m_alignment_sorted_widgets[m_month_widget->alignment()].push_back(m_month_widget);
        m_alignment_sorted_widgets[m_time_widget->alignment()].push_back(m_time_widget);
    }

    // SOIL INFILTRATION
    {
        connect(m_soil_infiltration_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_soil_infiltration_rate_changed(int)));
        connect(m_soil_infiltration_shortcut_widget, SIGNAL(soilInfiltrationZeroOverSlope(int)), this, SLOT(emit_soil_infiltration_zero_over_slope(int)));
        connect(m_soil_infiltration_shortcut_widget, SIGNAL(fill(int)), this, SLOT(emit_soil_infiltration_fill(int)));

        m_dynamic_display_widgets.push_back(m_soil_infiltration_widget);
        m_dynamic_display_widgets.push_back(m_soil_infiltration_shortcut_widget);

        m_alignment_sorted_widgets[m_soil_infiltration_widget->alignment()].push_back(m_soil_infiltration_widget);
        m_alignment_sorted_widgets[m_soil_infiltration_shortcut_widget->alignment()].push_back(m_soil_infiltration_shortcut_widget);
    }

    // WATER SHORTCUTS
    {
        connect(m_water_shortcut_widget, SIGNAL(absoluteHeight(int)), this, SLOT(emit_absolute_height_changed(int)));

        m_dynamic_display_widgets.push_back(m_water_shortcut_widget);
        m_alignment_sorted_widgets[m_water_shortcut_widget->alignment()].push_back(m_water_shortcut_widget);
    }

//    // CLUSTERING
//    {
//        connect(m_clustering_controller_widget, SIGNAL(clusteringSensitivityChanged(int)) ,this, SLOT(emit_clustering_sensitivity_changed(int)));

//        m_dynamic_display_widgets.push_back(m_clustering_controller_widget);
//        m_alignment_sorted_widgets[m_clustering_controller_widget->alignment()].push_back(m_clustering_controller_widget);
//    }

    // LEFT MODE SELECTION WIDGET
    {
        connect(m_previous_mode_controller_widget, SIGNAL(triggered()) ,this, SLOT(emit_previous_mode_triggered()));

        m_static_display_widgets.push_back(m_previous_mode_controller_widget);
        m_alignment_sorted_widgets[m_previous_mode_controller_widget->alignment()].push_back(m_previous_mode_controller_widget);
    }

    // LEFT MODE SELECTION WIDGET
    {
        connect(m_next_mode_controller_widget, SIGNAL(triggered()) ,this, SLOT(emit_next_mode_triggered()));

        m_static_display_widgets.push_back(m_next_mode_controller_widget);
        m_alignment_sorted_widgets[m_next_mode_controller_widget->alignment()].push_back(m_next_mode_controller_widget);
    }


    hideAll();
}

void OverlayWidgets::enable_static_widgets(bool enable)
{
    m_previous_mode_controller_widget->setVisible(enable && m_enable_previous);
    m_next_mode_controller_widget->setVisible(enable && m_enable_next);
}

OverlayWidgets::~OverlayWidgets()
{
    for(QWidget* w : m_dynamic_display_widgets)
        delete w;
    for(QWidget* w : m_static_display_widgets)
        delete w;
}

void OverlayWidgets::enableModeWidgets(bool previous, bool next)
{
    m_enable_previous = previous;
    m_enable_next = next;
    enable_static_widgets(true);
}

void OverlayWidgets::setModeLabels(QString previous, QString next)
{
    m_previous_mode_controller_widget->setLabel(previous);
    m_next_mode_controller_widget->setLabel(next);
}

int OverlayWidgets::getLatitude()
{
    return m_latitude_widget->value();
}

int OverlayWidgets::getTime()
{
    return m_time_widget->value();
}

int OverlayWidgets::getMonth()
{
    return m_month_widget->value();
}

int OverlayWidgets::getSoilInfiltrationRate()
{
    return m_soil_infiltration_widget->value();
}

int OverlayWidgets::getSoilInfiltrationZeroOverSlope()
{
    return m_soil_infiltration_shortcut_widget->getSoilInfiltrationZeroOverSlope();
}

//int OverlayWidgets::getClusteringSensitivity()
//{
//    return m_clustering_controller_widget->getClusteringSensitivity();
//}

bool OverlayWidgets::timeControllersActive()
{
    return m_time_widget->isVisible();
}

bool OverlayWidgets::latitudeControllersActive()
{
    return m_latitude_widget->isVisible();
}

bool OverlayWidgets::soilInfiltrationControllersActive()
{
    return m_soil_infiltration_widget->isVisible();
}

bool OverlayWidgets::waterControllersActive()
{
    return m_water_shortcut_widget->isVisible();
}

//bool OverlayWidgets::clusteringSensitivityControllersActive()
//{
//    return m_clustering_controller_widget->isVisible();
//}

void OverlayWidgets::hideAll()
{
    for(QWidget* w : m_dynamic_display_widgets)
        w->hide();

    enable_static_widgets(true);
}

void OverlayWidgets::triggerTimeControllers(bool show)
{
    if(show)
    {
        hideAll();
        m_time_widget->show();
        m_month_widget->show();
    }
    else
    {
        m_time_widget->hide();
        m_month_widget->hide();
    }

    enable_static_widgets(!show);

    emit timeControllersStateChanged(show);
}

void OverlayWidgets::triggerLatitudeControllers(bool show)
{
    if(show)
    {
        hideAll();
        m_latitude_widget->show();
    }
    else
    {
        m_latitude_widget->hide();
    }
    enable_static_widgets(!show);

    emit latitudeControllersStateChanged(show);
}

void OverlayWidgets::triggerSoilInfiltrationControllers(bool show)
{
    if(show)
    {
        hideAll();
        m_soil_infiltration_widget->show();
        m_soil_infiltration_shortcut_widget->show();
    }
    else
    {
        m_soil_infiltration_widget->hide();
        m_soil_infiltration_shortcut_widget->hide();
    }
    enable_static_widgets(!show);

    emit soilInfiltrationControllersStateChanged(show);
}

void OverlayWidgets::triggerWaterControllers(bool show)
{
    if(show)
    {
        hideAll();
        m_water_shortcut_widget->show();
    }
    else
    {
        m_water_shortcut_widget->hide();
    }
    enable_static_widgets(!show);

    emit waterControllersStateChanged(show);
}

//void OverlayWidgets::triggerClusteringControllers(bool show)
//{
//    if(show)
//    {
//        hideAll();
//        m_clustering_controller_widget->show();
//    }
//    else
//    {
//        m_clustering_controller_widget->hide();
//    }
//    enable_static_widgets(!show);

//    emit clusteringControllersStateChanged(show);
//}

void OverlayWidgets::resize(int container_width, int container_height)
{
    for(QWidget * w : m_alignment_sorted_widgets[Qt::AlignRight])
        w->move(container_width-w->width(), 0);

    for(QWidget * w : m_dynamic_display_widgets)
        w->setFixedHeight(container_height);
}

void OverlayWidgets::emit_latitude_changed(int latitude)
{
    emit latitudeChanged(latitude);
}

void OverlayWidgets::emit_time_changed(int time)
{
    emit timeChanged(time);
}

void OverlayWidgets::emit_month_changed(int month)
{
    emit monthChanged(month);
}

void OverlayWidgets::emit_soil_infiltration_rate_changed(int infiltration_rate)
{
    emit soilInfiltrationRateChanged(infiltration_rate);
}

void OverlayWidgets::emit_soil_infiltration_zero_over_slope(int min_slope)
{
    emit soilInfiltrationZeroOverSlope(min_slope);
}

void OverlayWidgets::emit_soil_infiltration_fill(int infiltration_rate)
{
    emit soilInfiltrationFill(infiltration_rate);
}

void OverlayWidgets::emit_absolute_height_changed(int height)
{
    emit absoluteHeightChanged(height);
}

void OverlayWidgets::emit_clustering_sensitivity_changed(int clustering_sensitivity)
{
    emit clusteringSensitivityChanged(clustering_sensitivity);
}

void OverlayWidgets::emit_previous_mode_triggered()
{
    emit previousModeTriggered();
}

void OverlayWidgets::emit_next_mode_triggered()
{
    emit nextModeTriggered();
}
