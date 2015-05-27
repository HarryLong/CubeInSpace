#ifndef GLWIDGET_H
#define GLWIDGET_H

//#include <GL/glew.h>

#include <QGLWidget>
#include <QScopedPointer>
#include <QMatrix4x4>
#include <QMouseEvent>

#include "renderer.h"
#include "view_manager.h"
#include <vector>
#include "scene_manager.h"
#include "constants.h"

#include <atomic>
#include <thread>
#include "settings.h"
#include "rays.h"

class QSurface;
class QMouseEvent;

struct MouseTracker{
    float start_point_x, start_point_y, start_point_z;
    float end_point_x, end_point_y, end_point_z;

    float getDiffX() {return end_point_x-start_point_x;}
    float getDiffY() {return end_point_y-start_point_y;}
    float getDiffZ() {return end_point_z-start_point_z;}

    bool ctrl_pressed;
};

enum ControlStyle {
    SoftImage,
    FPS,
    Experimental1
};

enum Mode {
    None,
    TerrainEdit,
    Selection,
    OrientationEdit
};

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(const Settings & settings, const QGLFormat& format, QWidget * parent = NULL);
    ~GLWidget();
    void loadTerrain(QString filename);
    void updateSettings(const Settings & settings);

public slots:
    void renderGrid(bool enabled);
    void renderTerrain(bool enabled);
    void renderAccelerationStructure(bool enabled);
    void renderRays(bool enabled);
    void setControlStyle(ControlStyle control_style);
    void disableOverlays();
    void enableSlopeOverlay();
    void enableAltitudeOverlay();
    void setMode(Mode mode);
    void setMonth(int month);
    void setTime(int hour_of_day);

protected:
    void initializeGL(); // Override
    void paintGL(); // Override
    void resizeGL(int p_width, int p_height); // Override
    QSize minimumSizeHint() const; //Override
    QSize sizeHint() const;//Override

    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );

    // Mouse input stuff
    void mousePressEvent(QMouseEvent *event);
//    void mouseDoubleClickEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * wheel);

    void keyPressEvent ( QKeyEvent * event );
    void keyReleaseEvent(QKeyEvent * event);

private:
    void normalizeScreenCoordinates(float & p_x, float & p_y);
    void setNavigationEnabled(bool enabled);
    void reset_fps_cursor();
    bool get_intersection_point_with_terrain(int screen_x, int screen_y, glm::vec3 & intersection_point);
    bool get_intersection_point_with_base_plane(int screen_x, int screen_y, glm::vec3 & intersection_point);

    Renderer* m_renderer;
    ViewManager* m_view_manager;
    SceneManager* m_scene_manager;
    RayDrawer * m_rays;
    MouseTracker m_mouse_position_tracker;

    void enable_continuous_mouse_tracking(bool enabled);
    void mouse_tracking_callback();
    void show_cursor(bool show);

    std::thread * m_mouse_tracking_thread;
    std::atomic<float> m_width;
    std::atomic<float> m_height;
    std::atomic<float> m_mouse_x;
    std::atomic<float> m_mouse_y;
    std::atomic<bool> m_mouse_tracking_thread_run;
    std::atomic<bool> m_ctrl_pressed;

    bool m_navigation_enabled;
    bool m_draw_grid;
    bool m_draw_terrain;
    bool m_draw_rays;
    bool m_draw_acceleration_structure;
    bool m_authorise_navigation_mode_switch;

    ControlStyle m_control_style;
    Mode m_active_mode;

    std::vector<glm::vec3> m_active_points;
};

#endif
