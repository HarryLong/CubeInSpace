#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "light_properties.h"
#include <QObject>

/********************
 * LIGHTING MANAGER *
 ********************/
class LightingManager : public QObject{
    Q_OBJECT
public:
    LightingManager();
    ~LightingManager();

    SunLightProperties & getSunlightProperties();

    int currentMonth() const;
    int currentTime() const;
signals:
    void sunPositionChanged(float pos_x, float pos_y, float pos_z);

public slots:
    void setMonth(int month);
    void setTime(int minutes);
    void setLatitude(int latitude);
    void setTerrainDimensions(int width, int depth, int base_height, int max_height);
    void setNorthOrientation(float north_x, float north_y, float north_z);
    void emit_sun_position_change(float pos_x, float pos_y, float pos_z);
    void signal_sun_position_change(bool enabled);

private:
    void establish_connections();

    SunLightProperties m_sunlight_properties;
    bool m_signal_sun_position_change;

};

#endif // LIGHT_MANAGER_H
