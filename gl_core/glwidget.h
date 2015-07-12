#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QScopedPointer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <vector>

#include "dialogs/dialogs.h"
#include "shaders/shaders.h"
#include "renderer.h"
#include <atomic>
#include <thread>
#include "../gl_assets/rays.h"
#include "widgets/orientation_widget.h"
#include "widgets/controller_widgets.h"
#include "widgets/progress_bar_widget.h"
#include "resources/resource_wrapper.h"
#include "terrain/terrain.h"
#include "camera.h"
#include "../gl_assets/sun_asset.h"
#include "../lighting/lighting_manager.h"
#include "../actions.h"

class QProgressDialog;
class QMouseEvent;
class MouseTracker{
public:
    void setStartPosition(float x, float y, float z = 0);
    void setEndPosition(float x, float y, float z = 0);

    void getStartPosition(float & x, float & y, float & z);
    void getEndPosition(float & x, float & y, float & z);

    float getDiffX();
    float getDiffY();
    float getDiffZ();

    bool ctrl_pressed;
private:
    std::atomic<float> start_point_x, start_point_y, start_point_z;
    std::atomic<float> end_point_x, end_point_y, end_point_z;
};


class QTimer;
class ActionFamily;
class AllActions;
class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget(AllActions * actions, QWidget * parent = NULL);
    ~GLWidget();
    void loadTerrain(QString filename);

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
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * wheel);
    void keyPressEvent ( QKeyEvent * event );
    void keyReleaseEvent(QKeyEvent * event);
    void resizeEvent(QResizeEvent * event);

private slots:
    void control_changed();
    void clear_rays();
    void overlay_changed();
    void sunPositionChanged(float,float,float);
    void load_terrain_file();
    void refresh_temperature();
    void refresh_water();
    void refresh_illumination();
    void refresh_shade();
    void reset_overlay();
    void time_controllers_state_changed(bool active);
    void latitude_controllers_state_changed(bool active);
    void enablePositionDependentOverlays(bool enable);

private:
    void normalizeScreenCoordinates(float & p_x, float & p_y);
    void setNavigationEnabled(bool enabled);
    void reset_fps_cursor();
    bool get_intersection_point_with_terrain(int screen_x, int screen_y, glm::vec3 & intersection_point);
    bool get_intersection_point_with_base_plane(int screen_x, int screen_y, glm::vec3 & intersection_point);
    void establish_connections();
    void update_info_pointer_dlg(const glm::vec2 & screen_pos);
    glm::vec3 to_world(const glm::vec3 & screen_coord);
    int month();

    bool render_rays();
    bool render_grid();
    bool render_sun();
    bool render_terrain();
    bool overlay_none();
    bool overlay_slope();
    bool overlay_altitude();
    bool overlay_shade();
    bool overlay_temperature();
    bool overlay_min_illumination();
    bool overlay_max_illumination();
    bool fps();
    bool softimage();

    void enable_continuous_mouse_tracking(bool enabled);
    void mouse_tracking_callback();
    void show_cursor(bool show);

    AllActions * m_actions;

    Dialogs m_dialogs;
    ProgressBarWidget m_progress_bar_widget;
    OverlayWidgets m_overlay_widgets;
    OrientationWidget m_orientation_widget;

    Renderer m_renderer;
    Camera m_camera;
    RayDrawer m_rays;
    MouseTracker m_mouse_position_tracker;
    MouseTracker m_terrain_position_tracker;

    std::thread * m_mouse_tracking_thread;
    std::atomic<float> m_width;
    std::atomic<float> m_height;
    std::atomic<float> m_mouse_x;
    std::atomic<float> m_mouse_y;
    std::atomic<bool> m_mouse_tracking_thread_run;
    std::atomic<bool> m_ctrl_pressed;

    bool m_navigation_enabled;
    bool m_authorise_navigation_mode_switch;

    Shaders m_shaders;
    QTimer * m_fps_callback_timer;

    static const int _TARGET_FPS;

    Terrain m_terrain;
    ResourceWrapper m_resources;
    LightingManager m_lighting_manager;
    Grid m_grid;
    SunAsset m_sun;

    const char * m_active_overlay;
};

#endif