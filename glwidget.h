#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
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
#include "rays.h"
#include "orientation_widget.h"
#include "controller_widgets.h"
#include "water_flow_analyser.h"

class QProgressDialog;
class QSurface;
class QMouseEvent;
class PointerInformationDialog;
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

/*******************
 * SHADER PROGRAMS *
 *******************/
class QOpenGLShaderProgram;
class WaterFluxGeneratorShader;
class ShaderPrograms {

public:
    ShaderPrograms(QObject * parent);
    ~ShaderPrograms();

    QOpenGLShaderProgram * m_base;
    QOpenGLShaderProgram * m_terrain;
    QOpenGLShaderProgram * m_terrain_elements;
    QOpenGLShaderProgram * m_normals_generator;
    WaterFluxGeneratorShader * m_water_flux_generator;
};

class Actions;
class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidget(ViewControllers view_controllers, TerrainControllers terrain_controllers, TemperatureEditDialog * temp_edit_dlg,
             Actions * render_actions, Actions* overlay_actions, Actions * control_actions, Actions * show_actions, Actions * edit_actions,
             Actions * tmp_actions, QWidget * parent = NULL);
    ~GLWidget();
    void loadTerrain(QString filename);

signals:
    void GLReady();

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
    void display_info_pointer_dlg(bool display);
    void clear_rays();

    void tmp_function1();

private:
    void normalizeScreenCoordinates(float & p_x, float & p_y);
    void setNavigationEnabled(bool enabled);
    void reset_fps_cursor();
    bool get_intersection_point_with_terrain(int screen_x, int screen_y, glm::vec3 & intersection_point);
    bool get_intersection_point_with_base_plane(int screen_x, int screen_y, glm::vec3 & intersection_point);
    void set_center_camera_position();
    void establish_connections();
    void update_info_pointer_dlg(const glm::vec2 & screen_pos);

    bool render_rays();
    bool render_grid();
    bool render_sun();
    bool render_terrain();
    bool render_acceleration_structure();
    bool fps();
    bool softimage();

    void enable_continuous_mouse_tracking(bool enabled);
    void mouse_tracking_callback();
    void show_cursor(bool show);

    Actions * m_render_actions;
    Actions * m_control_actions;
    Actions * m_show_actions;
    Actions * m_edit_actions;
    Actions * m_tmp_actions;

    ProgressBarWidget * m_progress_bar_widget;
    PointerInformationDialog * m_pointer_info_dlg;
    ControllerWidgetsWrapper m_overlay_widgets;

    Renderer m_renderer;
    SceneManager * m_scene_manager; // Pointer to ensure GL context is current on deletion
    ViewManager m_view_manager;
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

    std::vector<glm::vec3> m_active_points;

    ShaderPrograms * m_shaders;
    OrientationWidget m_orientation_widget;
    WaterFlowAnalyzer m_water_flow_analyzer;
};

#endif
