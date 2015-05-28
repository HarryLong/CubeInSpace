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
    LightingManager(glm::vec2 north_orientation);
    ~LightingManager();

    const SunLightProperties & getSunlightProperties();
public slots:
    void setMonth(int month);
    void setTime(int minutes);
    void setTerrainDimensions(int width, int depth);
    void setNorth(glm::vec2 orientation);

private:
    SunLightProperties m_sunlight_properties;
};

#endif // LIGHT_MANAGER_H
