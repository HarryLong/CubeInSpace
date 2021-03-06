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
                float altitude, float slope, float water_height, int soil_infiltration_rate, int soil_humidity, float weighted_soil_humidity,
                bool shade_set, bool shaded,
                bool temp_set, float temp,
                bool daily_illumination_set, int daily_illumination);
    void invalidPoint();

private slots:
    void setPoint(const glm::vec2 & point);
    void setAltitude(float altitude);
    void setSlope(float slope);
    void setTemp(float temp);
    void setShaded(bool shaded);
    void setDailyIllumination(int daily_illumination);
    void setWaterHeight(float water_height);
    void setSoilInfiltrationRate(int soil_infiltration_rate);
    void setSoilHumidity(int soil_humidity);
    void setWeightedSoilHumidity(float weighted_soil_humidity);
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
        _WEIGHTED_SOIL_HUMIDITY,
        _SHADE,
        _TEMP,
        _DAILY_ILLUMINATION,
        _WATER_HEIGHT,
        _AGGREGATE_HEIGHT
    };
    std::map<LabelType, QLabel*> m_labels;
};


#endif // POINTER_INFO_DIALOG_H
