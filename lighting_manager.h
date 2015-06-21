#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "light_properties.h"
#include <QObject>
#include "controllers.h"

/********************
 * LIGHTING MANAGER *
 ********************/
class LightingManager : public QObject{
    Q_OBJECT
public:
    LightingManager(TimeControllers & time_controllers, glm::vec3 north_orientation, glm::vec3 true_north_orientation, glm::vec3 east_orientation);
    ~LightingManager();

    SunLightProperties & getSunlightProperties();

    int currentMonth() const;
    int currentTime() const;
signals:
    void sunPositionChanged(float pos_x, float pos_y, float pos_z);

public slots:
    void setMonth(int month);
    void setTime(int minutes);
    void setTerrainDimensions(int width, int depth, int base_height, int max_height);
    void setOrientation(float north_x, float north_y, float north_z,
                        float true_north_x, float true_north_y, float true_north_z,
                        float east_x, float east_y, float east_z);
    void emit_sun_position_change(float pos_x, float pos_y, float pos_z);

private:
    SunLightProperties m_sunlight_properties;
};

#endif // LIGHT_MANAGER_H
