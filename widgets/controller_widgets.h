#ifndef CONTROLLER_WIDGETS_H
#define CONTROLLER_WIDGETS_H

#include <map>
#include <vector>
#include <QObject>

class BaseSliderControllerWidget;
class SoilInfiltrationShortcutWidget;
/******************************
 * CONTROLLER WIDGETS WRAPPER *
 ******************************/
class OverlayWidgets : public QObject{
    Q_OBJECT
public:
    OverlayWidgets(QWidget * parent);
    ~OverlayWidgets();

    void hideAll();
    void resize(int container_width, int container_height);

    int getLatitude();
    int getTime();
    int getMonth();
    int getSoilInfiltrationRate();
    int getSoilInfiltrationZeroOverSlope();

signals:
    void timeControllersStateChanged(bool active);
    void latitudeControllersStateChanged(bool active);
    void soilInfiltrationControllersStateChanged(bool active);
    void soilInfiltrationZeroOverSlope(int min_slope);

    void monthChanged(int month);
    void latitudeChanged(int latitude);
    void timeChanged(int time);
    void soilInfiltrationRateChanged(int);

public slots:
    void trigger_time_controllers(bool show_widget);
    void trigger_latitude_controllers(bool show_widget);
    void trigger_soil_infiltration_controllers(bool show_widget);

private slots:
    void emit_latitude_changed(int latitude);
    void emit_time_changed(int time);
    void emit_month_changed(int month);
    void emit_soil_infiltration_rate_changed(int month);
    void emit_soil_infiltration_zero_over_slope(int min_slope);

private:
    std::vector<QWidget*> m_raw_widgets;
    std::map<int, std::vector<QWidget*> > m_alignment_sorted_widgets;

    BaseSliderControllerWidget * m_time_widget;
    BaseSliderControllerWidget * m_month_widget;
    BaseSliderControllerWidget * m_latitude_widget;
    BaseSliderControllerWidget * m_soil_infiltration_widget;
    SoilInfiltrationShortcutWidget * m_soil_infiltration_shortcut_widget;
};


#endif // CONTROLLER_WIDGETS_H
