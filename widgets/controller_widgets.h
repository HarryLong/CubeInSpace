#ifndef CONTROLLER_WIDGETS_H
#define CONTROLLER_WIDGETS_H

#include <map>
#include <vector>
#include <QObject>

class BaseSliderControllerWidget;
class SoilInfiltrationShortcutWidget;
class WaterShortcutWidget;
class ClusteringControllerWidget;
class ModeControllerWidget;
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
    int getClusteringSensitivity();

    bool timeControllersActive();
    bool latitudeControllersActive();
    bool soilInfiltrationControllersActive();
    bool waterControllersActive();
    bool clusteringSensitivityControllersActive();

signals:
    void timeControllersStateChanged(bool active);
    void latitudeControllersStateChanged(bool active);
    void soilInfiltrationControllersStateChanged(bool active);
    void waterControllersStateChanged(bool active);
    void clusteringControllersStateChanged(bool active);

    void soilInfiltrationZeroOverSlope(int min_slope);
    void soilInfiltrationFill(int infiltration_rate);
    void absoluteHeightChanged(int absoluteHeight);

    void monthChanged(int month);
    void latitudeChanged(int latitude);
    void timeChanged(int time);
    void soilInfiltrationRateChanged(int);
    void clusteringSensitivityChanged(int);

    void nextModeTriggered();
    void previousModeTriggered();

public slots:
    void triggerTimeControllers(bool show_widget);
    void triggerLatitudeControllers(bool show_widget);
    void triggerSoilInfiltrationControllers(bool show_widget);
    void triggerWaterControllers(bool show_widget);
    void enableModeWidgets(bool previous, bool next);
    void setModeLabels(QString previous, QString next);

private slots:
    void emit_latitude_changed(int latitude);
    void emit_time_changed(int time);
    void emit_month_changed(int month);
    void emit_soil_infiltration_rate_changed(int month);
    void emit_soil_infiltration_zero_over_slope(int min_slope);
    void emit_soil_infiltration_fill(int infiltration_rate);
    void emit_absolute_height_changed(int infiltration_rate);
    void emit_clustering_sensitivity_changed(int clustering_sensitivity);
    void emit_next_mode_triggered();
    void emit_previous_mode_triggered();
    void enable_static_widgets(bool enable);

private:    
    std::vector<QWidget*> m_dynamic_display_widgets;
    std::vector<QWidget*> m_static_display_widgets;
    std::map<int, std::vector<QWidget*> > m_alignment_sorted_widgets;

    BaseSliderControllerWidget * m_time_widget;
    BaseSliderControllerWidget * m_month_widget;
    BaseSliderControllerWidget * m_latitude_widget;
    BaseSliderControllerWidget * m_soil_infiltration_widget;
    SoilInfiltrationShortcutWidget * m_soil_infiltration_shortcut_widget;
    WaterShortcutWidget * m_water_shortcut_widget;
//    ClusteringControllerWidget * m_clustering_controller_widget;
    ModeControllerWidget * m_previous_mode_controller_widget;
    ModeControllerWidget * m_next_mode_controller_widget;

    bool m_enable_previous;
    bool m_enable_next;
};

#endif // CONTROLLER_WIDGETS_H
