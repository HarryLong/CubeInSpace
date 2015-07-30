#include "controller_widgets.h"
#include "month_controller.h"
#include "latitude_controller.h"
#include "soil_infiltration_controller.h"
#include "soil_infiltration_shortcuts_widget.h"
#include "time_controller.h"
#include "water_shortcuts_controller.h"
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
    m_water_shortcut_widget(new WaterShortcutWidget(Qt::AlignLeft, parent))
{
    m_alignment_sorted_widgets[Qt::AlignTop] = std::vector<QWidget*>();
    m_alignment_sorted_widgets[Qt::AlignRight] = std::vector<QWidget*>();
    m_alignment_sorted_widgets[Qt::AlignBottom] = std::vector<QWidget*>();
    m_alignment_sorted_widgets[Qt::AlignLeft] = std::vector<QWidget*>();

    // LATITUDE
    {
        connect(m_latitude_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_latitude_changed(int)));

        m_raw_widgets.push_back(m_latitude_widget);
        m_alignment_sorted_widgets[m_latitude_widget->alignment()].push_back(m_latitude_widget);
    }

    // TIME
    {
        connect(m_month_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_month_changed(int)));
        connect(m_time_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_time_changed(int)));

        m_raw_widgets.push_back(m_month_widget);
        m_raw_widgets.push_back(m_time_widget);

        m_alignment_sorted_widgets[m_month_widget->alignment()].push_back(m_month_widget);
        m_alignment_sorted_widgets[m_time_widget->alignment()].push_back(m_time_widget);
    }

    // SOIL INFILTRATION
    {
        connect(m_soil_infiltration_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_soil_infiltration_rate_changed(int)));
        connect(m_soil_infiltration_shortcut_widget, SIGNAL(soilInfiltrationZeroOverSlope(int)), this, SLOT(emit_soil_infiltration_zero_over_slope(int)));
        connect(m_soil_infiltration_shortcut_widget, SIGNAL(fill(int)), this, SLOT(emit_soil_infiltration_fill(int)));

        m_raw_widgets.push_back(m_soil_infiltration_widget);
        m_raw_widgets.push_back(m_soil_infiltration_shortcut_widget);

        m_alignment_sorted_widgets[m_soil_infiltration_widget->alignment()].push_back(m_soil_infiltration_widget);
        m_alignment_sorted_widgets[m_soil_infiltration_shortcut_widget->alignment()].push_back(m_soil_infiltration_shortcut_widget);
    }

    // WATER SHORTCUTS
    {
        connect(m_water_shortcut_widget, SIGNAL(absoluteHeight(int)), this, SLOT(emit_absolute_height_changed(int)));

        m_raw_widgets.push_back(m_water_shortcut_widget);
        m_alignment_sorted_widgets[m_water_shortcut_widget->alignment()].push_back(m_water_shortcut_widget);
    }

    hideAll();
}

OverlayWidgets::~OverlayWidgets()
{
    for(QWidget* w : m_raw_widgets)
        delete w;
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

void OverlayWidgets::hideAll()
{
    for(QWidget* w : m_raw_widgets)
        w->hide();
}

void OverlayWidgets::trigger_time_controllers(bool show)
{
    hideAll();
    if(show)
    {
        m_time_widget->show();
        m_month_widget->show();
    }
    else
    {
        m_time_widget->hide();
        m_month_widget->hide();
    }

    emit timeControllersStateChanged(show);
}

void OverlayWidgets::trigger_latitude_controllers(bool show)
{
    hideAll();
    if(show)
    {
        m_latitude_widget->show();
    }
    else
    {
        m_latitude_widget->hide();
    }

    emit latitudeControllersStateChanged(show);
}

void OverlayWidgets::trigger_soil_infiltration_controllers(bool show)
{
    hideAll();
    if(show)
    {
        m_soil_infiltration_widget->show();
        m_soil_infiltration_shortcut_widget->show();
    }
    else
    {
        m_soil_infiltration_widget->hide();
        m_soil_infiltration_shortcut_widget->hide();
    }

    emit soilInfiltrationControllersStateChanged(show);
}

void OverlayWidgets::trigger_water_controllers(bool show)
{
    hideAll();
    if(show)
    {
        m_water_shortcut_widget->show();
    }
    else
    {
        m_water_shortcut_widget->hide();
    }

    emit waterControllersStateChanged(show);
}

void OverlayWidgets::resize(int container_width, int container_height)
{
    for(QWidget * w : m_alignment_sorted_widgets[Qt::AlignRight])
        w->move(container_width-w->width(), 0);

    for(QWidget * w : m_raw_widgets)
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

