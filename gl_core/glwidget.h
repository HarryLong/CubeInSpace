#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QScopedPointer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <vector>

#include "dialogs/dialogs.h"
#include "renderer.h"
#include "computer.h"
#include <atomic>
#include <thread>
#include "../gl_assets/rays.h"
#include "widgets/orientation_widget.h"
#include "widgets/controller_widgets.h"
#include "widgets/progress_bar_widget.h"
#include "resources/resource_wrapper.h"
#include "terrain/terrain.h"
#include "terrain/padded_terrain.h"
#include "camera.h"
#include "../gl_assets/sun_asset.h"
#include "../lighting/lighting_manager.h"
#include "../actions.h"
#include "../gl_texture/overlay_texture.h"
#include "../resources/flood_fill_tracker.h"
#include "clustering/k_means_clusterer.h"

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

signals:
    void plantModeTriggered();
    void clusteringModeTriggered();
    void resourceModeTriggered();

protected:
    enum Mode{
        _RESOURCE = 0,
        _CLUSTERING,
        _PLANT
    } m_current_mode;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int p_width, int p_height) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
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
    void load_terrain(TerragenFile terrain_file);
    void clear_rays();
    void sunPositionChanged(float,float,float);
    void load_terrain_file();
    void refresh_camera_properties();
    void previous_mode_triggered();
    void next_mode_triggered();

    void refresh_temperature(bool refresh_overlay = true);
    void refresh_temperature(int jun_temp, int dec_temp, float lapse_rate, bool refresh_overlay = true);
    void refresh_illumination(bool refresh_overlay = true);
    void refresh_shade(bool refresh_overlay = true);
    void refresh_water();
    void reset_soil_infiltration_rate();

    void reset_overlay();
    void set_overlay(QAction * active_overlay_action);

    void edit(QAction * triggered_action);
    void reset_edit();

    void hide_all_dialogs();

    void set_overlays_active(bool active);
    void set_edit_actions_active(bool active);

    void soil_infiltration_rate_changed(int);
    void refresh_normals();//    void disable_all_overlay_widget_actions();

    void temp_invalidated();
    void shade_invalidated();
    void daily_illumination_invalidated();

    void monthChanged();

    void update_soil_infiltration_rate(const glm::vec3 & intersection_point);
    void zeroify_soil_infiltration_above_slope(int slope);
    void fill_infiltration_rate(int infiltration_rate);
    void set_absolute_aggregate_height(int height);
    void reset_water();
    void format_overlay_texture(int width, int depth);
    void set_flood_fill_enabled(bool);
    void flood_fill(int month, const glm::ivec2 & seed_point);
    bool flood_fill(const glm::ivec2 & point, TerrainWater & terrain_water, int month, float & seed_height);
    void new_terrain_is_going_to_load(int,int);
    void new_terrain_loaded(int,int);

    void refresh_overlay_texture();
    void refresh_clusters(int);
    void reset_clusters();

    void standing_water_set(int month);

    void place_plants();
    void setMode(Mode mode);

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
    void append_surrounding_points(const glm::ivec2 & pos, std::vector<glm::ivec2> & surrounding_points, FloodFillTracker & tracker);
    void balance_water(bool one_step = false);

    bool render_rays();
    bool render_grid();
    bool render_sun();
    bool render_terrain();
    bool overlay_none();
    bool overlay_slope();
    bool overlay_altitude();
    bool overlay_shade();
    bool overlay_temperature();
    bool overlay_illumination();
    bool overlay_soil_infiltration();
    bool overlay_monthly_soil_humidity();
    bool overlay_weighted_avg_soil_humidity();
    bool overlay_clusters();
    bool edit_infiltration_rate();
    bool fps();
    bool softimage();

    void trigger_resource_mode();
    void trigger_clustering_mode();
    void trigger_plant_mode();

    void enable_continuous_mouse_tracking(bool enabled);
    void mouse_tracking_callback();
    void show_cursor(bool show);

    AllActions * m_actions;

    Dialogs m_dialogs;
    ProgressBarWidget m_progress_bar_widget;
    OverlayWidgets m_overlay_widgets;
    OrientationWidget m_orientation_widget;

    Renderer m_renderer;
    Computer m_computer;
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
    std::atomic<bool> m_render_water;

    std::map<QAction *, const char *> m_overlay_action_to_overlay_uniform;

    bool m_navigation_enabled;
    bool m_authorise_navigation_mode_switch;
    bool m_flood_fill_mode;
    bool m_previous_flood_fill_all_months_changed;

    QTimer * m_fps_callback_timer;

    static const int _TARGET_FPS;

    Terrain m_terrain;
    PaddedTerrain m_padded_terrain;
    ResourceWrapper m_resources;
    LightingManager m_lighting_manager;
    Grid m_grid;
    SunAsset m_sun;
    FloodFillTracker m_floodfill_tracker;

    GLTerrainRect * m_selection_rect;
    GLTerrainRect * m_humidity_rect;
    OverlayTexture m_overlay_texture;

    ClusterMembershipTexture m_cluster_membership_texture;
    KMeansClusterer m_clusterer;

    const char * m_active_overlay;

    static const QString _RESOURCE_MODE_LABEL;
    static const QString _CLUSTER_MODE_LABEL;
    static const QString _PLANT_MODE_LABEL;

    static const int _STANDING_WATER_HUMIDITY = 10000;
};

#endif
