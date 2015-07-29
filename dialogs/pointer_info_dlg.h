#ifndef POINTER_INFO_DIALOG_H
#define POINTER_INFO_DIALOG_H

#include <QDialog>
#include <map>
#include <glm/common.hpp>

class QLabel;
class PointerInformationDialog : public QDialog
{
    Q_OBJECT
public:
    PointerInformationDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~PointerInformationDialog();

    virtual QSize minimumSizeHint() const;

public slots:
    void update(const glm::vec2 & point,
                float altitude, float slope, int water_height, int soil_infiltration_rate, int soil_humidity,
                bool shade_set, bool shaded,
                bool temp_set, float temp,
                bool daily_illumination_set, int min_daily_illumination, int max_daily_illumination);
    void invalidPoint();

private slots:
    void setPoint(const glm::vec2 & point);
    void setAltitude(float altitude);
    void setSlope(float slope);
    void setTemp(float temp);
    void setShaded(bool shaded);
    void setMinDailyIllumination(int min_daily_illumination);
    void setMaxDailyIllumination(int max_daily_illumination);
    void setWaterHeight(int water_height);
    void setSoilInfiltrationRate(int soil_infiltration_rate);
    void setSoilHumidity(int soil_humidity);
    void setAggregateHeight(int height);

    void setInvalid(QLabel * lbl);

private:
    void init_labels();
    void init_layout();

    enum LabelType {
        _POINT,
        _ALTITUDE,
        _SLOPE,
        _SOIL_INFILTRATION_RATE,
        _SOIL_HUMIDITY,
        _SHADE,
        _TEMP,
        _MIN_DAILY_ILLUMINATION,
        _MAX_DAILY_ILLUMINATION,
        _WATER_HEIGHT,
        _AGGREGATE_HEIGHT
    };
    std::map<LabelType, QLabel*> m_labels;
};


#endif // POINTER_INFO_DIALOG_H
