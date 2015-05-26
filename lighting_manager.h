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

    const SunLightProperties & getSunlightProperties();
public slots:
    void setMonth(int month);
    void setTime(int hour_of_day);
    void setTerrainDimensions(int width, int depth);

private:
    SunLightProperties m_sunlight_properties;
};

#endif // LIGHT_MANAGER_H
