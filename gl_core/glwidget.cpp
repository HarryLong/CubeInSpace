#include "glwidget.h"

#include <QOpenGLContext>
#include <iostream>
#include <QMouseEvent>
#include "constants.h"
#include <math.h>
#include "geom.h"
#include "glm_wrapper.h"
#include "actions.h"
#include "glheader.h"
#include "lighting/lighting_manager.h"
#include <QTimer>
#include "terragen_file_manager/terragen_file_manager.h"
#include <QOpenGLFunctions_4_3_Core>
#include <QFileDialog>
#include "../actions.h"
#include <cstring>
#include "../widgets/soil_infiltration_controller.h"
#include <algorithm>
#include <functional>
/*****************
 * MOUSE TRACKER *
 *****************/
void MouseTracker::setStartPosition(float x, float y, float z)
{
    start_point_x.store(x);
    start_point_y.store(y);
    start_point_z.store(z);
}

void MouseTracker::setEndPosition(float x, float y, float z)
{
    end_point_x.store(x);
    end_point_y.store(y);
    end_point_z.store(z);
}

void MouseTracker::getStartPosition(float & x, float & y, float & z)
{
    x = start_point_x.load();
    y = start_point_y.load();
    z = start_point_z.load();
}

void MouseTracker::getEndPosition(float & x, float & y, float & z)
{
    x = end_point_x.load();
    y = end_point_y.load();
    z = end_point_z.load();
}

float MouseTracker::getDiffX()
{
    return end_point_x.load()-start_point_x.load();
}

float MouseTracker::getDiffY()
{
    return end_point_y.load()-start_point_y.load();
}

float MouseTracker::getDiffZ()
{
    return end_point_z.load()-start_point_z.load();
}

//----------------------------------------------------------------------------------------------
#include <QBoxLayout>
const int GLWidget::_TARGET_FPS = 30;
const QString GLWidget::_RESOURCE_MODE_LABEL = "RESOURCES";
const QString GLWidget::_CLUSTER_MODE_LABEL = "CLUSTERS";
const QString GLWidget::_PLANT_MODE_LABEL = "PLANTS";
GLWidget::GLWidget(AllActions * actions, QWidget * parent) :
  QOpenGLWidget(parent),
  m_progress_bar_widget(this),
  m_actions(actions),
  m_overlay_widgets(this),
  m_navigation_enabled(false), m_mouse_tracking_thread(NULL), m_authorise_navigation_mode_switch(true), m_render_water(true),
  m_fps_callback_timer(new QTimer),
  m_dialogs(parent),
  m_orientation_widget(this),
  m_active_overlay(Uniforms::Overlay::_NONE),
  m_selection_rect(ShapeFactory::getTerrainRectangle()),
  m_humidity_rect(ShapeFactory::getTerrainRectangle()),
  m_flood_fill_mode(false)
{
    m_humidity_rect->resize(5,5); // Temporary

    // Set up camera properties
    refresh_camera_properties();

    // Set up orientation widget
    m_orientation_widget.setCameraDirection(m_camera.getCameraDirection());

    // Set up lighting manager
    m_lighting_manager.setLatitude(m_overlay_widgets.getLatitude());
    m_lighting_manager.setMonth(m_overlay_widgets.getMonth());
    m_lighting_manager.setTime(m_overlay_widgets.getTime());

    // Set up mapping of overlay actions to overlay uniforms
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_NONE]] = Uniforms::Overlay::_NONE;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_SLOPE]] = Uniforms::Overlay::_SLOPE;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_ALTITUDE]] = Uniforms::Overlay::_ALTITUDE;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_SHADE]] = Uniforms::Overlay::_SHADE;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_TEMPERATURE]] = Uniforms::Overlay::_TEMPERATURE;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_ILLUMINATION]] =
            Uniforms::Overlay::_ILLUMINATION;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_SOIL_INFILTRATION_RATE]] =
            Uniforms::Overlay::_SOIL_INFILTRATION_RATE;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_MONTHLY_SOIL_HUMIDITY]] =
            Uniforms::Overlay::_MONTHLY_SOIL_HUMIDITY;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_WEIGHTED_AVG_SOIL_HUMIDITY]] =
            Uniforms::Overlay::_WEIGHTED_AVG_SOIL_HUMIDITY;
    m_overlay_action_to_overlay_uniform[m_actions->m_overlay_actions[OverlayActionFamily::_CLUSTERS]] =
            Uniforms::Overlay::_CLUSTERS;

    m_mouse_tracking_thread_run.store(true);
    m_ctrl_pressed.store(false);
    setFocusPolicy(Qt::ClickFocus);
    establish_connections();
    setNavigationEnabled(false);
    setMode(Mode::_RESOURCE);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    m_fps_callback_timer->stop();
    enable_continuous_mouse_tracking(false);
    delete m_mouse_tracking_thread;
    delete m_selection_rect;
    delete m_humidity_rect;
}

void GLWidget::establish_connections()
{
    // LOAD NEW TERRAIN
    connect(m_actions->m_base_actions[BaseActionFamily::_LOAD_TERRAIN], SIGNAL(triggered(bool)), this, SLOT(load_terrain_file()));

    // OVERLAY WIDGETS
    connect(m_actions->m_edit_actions.getActionGroup(), SIGNAL(triggered(QAction*)), this, SLOT(edit(QAction*)));

    // NORMALS NEED RECALCULATING
    connect(&m_terrain, SIGNAL(normalsInvalid()), this, SLOT(refresh_normals()));

    // TERRAIN DIMENSTIONS CHANGED
    connect(&m_terrain, SIGNAL(newTerrainGoingToLoad(int,int)), this, SLOT(new_terrain_is_going_to_load(int,int)));
    connect(&m_terrain, SIGNAL(newTerrainLoaded(int,int)), this, SLOT(new_terrain_loaded(int,int)));

    // CAMERA ORIENTATION CHANGE
    connect(&m_camera, SIGNAL(cameraDirectionChanged(float,float,float)), &m_orientation_widget, SLOT(setCameraDirection(float,float,float)));

    // DISPLAY DIALOGS
    connect(m_actions->m_show_actions[ShowActionFamily::_POINTER_INFO], SIGNAL(triggered(bool)), &m_dialogs, SLOT(showPointerInfoDialog()));
    connect(m_actions->m_base_actions[BaseActionFamily::_OPEN_SETTINGS], SIGNAL(triggered(bool)), &m_dialogs, SLOT(showSettingsDialog()));

    // UPDATE PROGRESS BAR
    connect(&m_resources, SIGNAL(processing(QString)), &m_progress_bar_widget, SLOT(processing(QString)));
    connect(&m_resources, SIGNAL(processingUpdate(int)), &m_progress_bar_widget, SLOT(updateProgress(int)));
    connect(&m_resources, SIGNAL(processingComplete()), &m_progress_bar_widget, SLOT(finishedProcessing()));
    connect(&m_resources, SIGNAL(processDescriptionUpdate(QString)), &m_progress_bar_widget, SLOT(updateInfoLabel(QString)));

    connect(&m_simulator, SIGNAL(processing(QString)), &m_progress_bar_widget, SLOT(processing(QString)));
    connect(&m_simulator, SIGNAL(processingUpdate(int)), &m_progress_bar_widget, SLOT(updateProgress(int)));
    connect(&m_simulator, SIGNAL(processingComplete()), &m_progress_bar_widget, SLOT(finishedProcessing()));
    connect(&m_simulator, SIGNAL(processDescriptionUpdate(QString)), &m_progress_bar_widget, SLOT(updateInfoLabel(QString)));

    // CLEAR RENDERED RAYS
    connect(m_actions->m_render_actions[RenderActionFamily::_RAYS], SIGNAL(triggered(bool)), this, SLOT(clear_rays()));

    // When overlay is changed
    connect(m_actions->m_overlay_actions.getActionGroup(), SIGNAL(triggered(QAction*)), this, SLOT(set_overlay(QAction*)));

    // When controller widgets are activated
    connect(&m_overlay_widgets, SIGNAL(soilInfiltrationRateChanged(int)), this, SLOT(soil_infiltration_rate_changed(int)));
    connect(&m_overlay_widgets, SIGNAL(absoluteHeightChanged(int)), this, SLOT(set_absolute_aggregate_height(int)));

    // When sun position changed
    connect(&m_lighting_manager, SIGNAL(sunPositionChanged(float,float,float)), this, SLOT(sunPositionChanged(float,float,float)));
    connect(&m_lighting_manager, SIGNAL(sunPositionChanged(float,float,float)), &m_resources, SLOT(sunPositionChanged()));

    // When latitude changes
    connect(&m_overlay_widgets, SIGNAL(latitudeChanged(int)), &m_resources, SLOT(latitudeChanged()));
    connect(&m_overlay_widgets, SIGNAL(latitudeChanged(int)), &m_lighting_manager, SLOT(setLatitude(int)));

    // MONTH CHANGE
    connect(&m_overlay_widgets, SIGNAL(monthChanged(int)), &m_lighting_manager, SLOT(setMonth(int)));
    connect(&m_overlay_widgets, SIGNAL(monthChanged(int)), this, SLOT(monthChanged()));

    // TIME CHANGE
    connect(&m_overlay_widgets, SIGNAL(timeChanged(int)), &m_lighting_manager, SLOT(setTime(int)));

    // SOIL INFILTRATION SHORTCUTS
    connect(&m_overlay_widgets, SIGNAL(soilInfiltrationZeroOverSlope(int)), this, SLOT(zeroify_soil_infiltration_above_slope(int)));
    connect(&m_overlay_widgets, SIGNAL(soilInfiltrationFill(int)), this, SLOT(fill_infiltration_rate(int)));

    // ORIENTATION CHANGE
    connect(&m_orientation_widget, SIGNAL(northOrientationChanged(float,float,float)), &m_lighting_manager, SLOT(setNorthOrientation(float,float,float)));

    // CAMERA SENSITIVITY
    connect(&m_dialogs.m_settings_dlg, SIGNAL(accepted()), this, SLOT(refresh_camera_properties()));

    // TEMPERATURE
    connect(&m_dialogs.m_temp_editor_dlg, SIGNAL(temperatureValuesChanged(int,int,float)), this, SLOT(refresh_temperature(int,int,float)));

    // WATER
    connect(&m_dialogs.m_monthly_rainfall_edit_dlg, SIGNAL(accepted()), this, SLOT(refresh_water()));  

    // FPS CALLBACK
    connect(m_fps_callback_timer, SIGNAL(timeout()), this, SLOT(update()));

    // WHEN A RESOUCE IS INVALIDATED CHECK REFRESH IT IF NECESSARY
    connect(&m_resources, SIGNAL(shadeInvalidated()), this, SLOT(shade_invalidated()));
    connect(&m_resources, SIGNAL(tempInvalidated()), this, SLOT(temp_invalidated()));
    connect(&m_resources, SIGNAL(dailyIlluminationInvalidated()), this, SLOT(daily_illumination_invalidated()));

    // When water is balanced, calculate soil humidities
    connect(&m_resources, SIGNAL(water_balanced(int)), this, SLOT(standing_water_set(int)));

    connect(&m_overlay_widgets, SIGNAL(previousModeTriggered()), this, SLOT(previous_mode_triggered()));
    connect(&m_overlay_widgets, SIGNAL(nextModeTriggered()), this, SLOT(next_mode_triggered()));

    connect(&m_dialogs.m_cluster_info_dlg, SIGNAL(refresh_clusters(int)), this, SLOT(refresh_clusters(int)));

    connect(&m_dialogs.m_plant_placement_dlg, SIGNAL(refreshPlants()), this, SLOT(refresh_plants()));

    // CLUSTERING
//    connect(&m_clusterer, SIGNAL(clustering_start(QString)), this, SLOT(show_progress_bar(QString)));
//    connect(&m_clusterer, SIGNAL(clustering_update(int)), &m_progress_bar_widget, SLOT(updateProgress(int)));
//    connect(&m_clusterer, SIGNAL(clustering_complete()), this, SLOT(hide_progress_bar()));
}

void GLWidget::initializeGL() // Override
{
    std::cout << "** OPEN GL INFO **" << std::endl;
    std::cout << "- Version: " << glGetString(GL_VERSION) << std::endl;

    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(.8f, .8f, .8f, 1.0f);
    setMouseTracking(true);

    m_renderer.compileAndLinkShaders();
    m_computer.compileAndLinkShaders();
    load_terrain(BaseTerrain());

    m_fps_callback_timer->start((int)(1000.0f/GLWidget::_TARGET_FPS));
}

void GLWidget::setMode(Mode mode)
{
    m_current_mode = mode;

    if(mode == Mode::_RESOURCE)
    {
        m_overlay_widgets.setModeLabels("", GLWidget::_CLUSTER_MODE_LABEL);
        m_overlay_widgets.enableModeWidgets(false,true);
        trigger_resource_mode();
        emit resourceModeTriggered();
    }
    else if(mode == Mode::_CLUSTERING)
    {
        m_overlay_widgets.setModeLabels(GLWidget::_RESOURCE_MODE_LABEL, GLWidget::_PLANT_MODE_LABEL);
        m_overlay_widgets.enableModeWidgets(true,true);
        trigger_clustering_mode();
        emit clusteringModeTriggered();
    }
    else // PLANT MODE
    {
        m_overlay_widgets.setModeLabels(GLWidget::_CLUSTER_MODE_LABEL, "");
        m_overlay_widgets.enableModeWidgets(true,false);
        trigger_plant_mode();
        emit plantModeTriggered();
    }
}

void GLWidget::previous_mode_triggered()
{
    setMode(static_cast<Mode>(static_cast<int>(m_current_mode)-1));
}

void GLWidget::next_mode_triggered()
{
    setMode(static_cast<Mode>(static_cast<int>(m_current_mode)+1));
}

void GLWidget::paintGL() // Override
{
    if(!m_fps_callback_timer->isActive())
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CE();

    glm::mat4x4 proj_mat;
    glm::mat4x4 view_mat;

    m_camera.getTransforms(proj_mat, view_mat);
    Transform transform(view_mat, proj_mat);

    std::vector<Asset*> assets_to_render;

    // Check if we need calculate water balance
    if(!m_resources.getTerrainWater().balanced())
        balance_water();

    // Update the overlay texture
    if(edit_infiltration_rate())
        refresh_overlay_texture();

    // Draw the grid
    if(render_grid())
    {
        std::vector<Asset*> grid_assets (m_grid.getAssets());
        assets_to_render.insert(assets_to_render.end(), grid_assets.begin(), grid_assets.end());CE();
    }

    // Draw the terrain
    if(render_terrain())
    {
        // TERRAIN
        m_renderer.renderTerrain(m_terrain, m_resources.getTerrainWater()[month()-1]->textureId(), m_render_water.load(), transform, m_lighting_manager.getSunlightProperties(),
                m_overlay_texture.textureId(), !overlay_none());

        // TERRAIN ELEMENTS
        std::vector<Asset*> terrain_elements_to_render;
        if(edit_infiltration_rate())
            terrain_elements_to_render.push_back(m_humidity_rect);
        else
            terrain_elements_to_render.push_back(m_selection_rect);
        m_renderer.renderTerrainElements(m_terrain, transform, terrain_elements_to_render) ;
    }

    // Draw the rays
    if(render_rays())
        assets_to_render.push_back(&m_rays);

    // Draw the sun
    if(render_sun())
        assets_to_render.push_back(&m_sun);

    m_renderer.renderAssets(transform, assets_to_render);
}

void GLWidget::resizeGL(int w, int h) // Override
{
    QOpenGLWidget::resizeGL(w ,h);
    m_width.store(width());
    m_height.store(height());

    m_camera.setAspect(static_cast<double>(w)/ h);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(1000, 600);
}

QSize GLWidget::sizeHint() const
{
    return QSize(1000, 600);
}

void GLWidget::focusInEvent ( QFocusEvent * event )
{
//    grabKeyboard();
}

void GLWidget::focusOutEvent ( QFocusEvent * event )
{
    releaseKeyboard();
}

void GLWidget::set_flood_fill_enabled(bool enabled)
{
    m_flood_fill_mode = enabled;
}

void GLWidget::normalizeScreenCoordinates(float & p_x, float & p_y)
{
    float w = (float) width();
    float h = (float) height();

    p_x = (p_x - w)/w;
    p_y = (p_y - w)/w;
}

bool GLWidget::get_intersection_point_with_terrain(int screen_x, int screen_y, glm::vec3 & intersection_point)
{
    glm::vec3 screen_cord(screen_x, screen_y, 0.0f);
    glm::vec3 start(to_world(screen_cord));
    screen_cord.z = 1.f;
    glm::vec3 end(to_world(screen_cord));
    glm::vec3 direction(glm::normalize(Geom::diff(end,start)));
    return m_terrain.traceRay(start, direction, intersection_point);
}

bool GLWidget::get_intersection_point_with_base_plane(int screen_x, int screen_y, glm::vec3 & intersection_point)
{
    glm::vec3 screen_cord(screen_x, screen_y, 0.0f);
    glm::vec3 start(to_world(screen_cord));
    screen_cord.z = 1.f;
    glm::vec3 end(to_world(screen_cord));
    glm::vec3 direction(glm::normalize(Geom::diff(end,start)));
    return Geom::rayPlaneIntersection(m_terrain.getBaseHeight(), start, direction, intersection_point);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    QOpenGLWidget::mousePressEvent(event);

    if(m_navigation_enabled)
    {
        if(softimage())
        {
            float x = event->x(); float y = event->y();
            normalizeScreenCoordinates(x,y);

            m_mouse_position_tracker.setStartPosition(x,y);
            m_mouse_position_tracker.ctrl_pressed = (event->modifiers() == Qt::ControlModifier);
        }
    }
    else // Navigation disabled - Perform ray tracing to get intersection point
    {
        glm::vec3 intersection_point;
        bool intersection(get_intersection_point_with_terrain(event->x(), event->y(), intersection_point));


        if(render_rays()) // Add the ray
        {
            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            glm::vec3 start(to_world(glm::vec3(event->x(), event->y(), .0f)));
            glm::vec3 end(to_world(glm::vec3(event->x(), event->y(), 1.f)));
            m_rays.add(start, end) ;
        }

        if(intersection)
        {
            if(edit_infiltration_rate())
            {
                update_soil_infiltration_rate(intersection_point);
            }
            else if(m_flood_fill_mode)
            {
                glm::ivec2 seed_point((int)intersection_point[0], (int)intersection_point[2]);

                if(m_ctrl_pressed)
                {
                    m_previous_flood_fill_all_months_changed = false;
                    flood_fill(month(), seed_point);
                }
                else
                {
                    m_previous_flood_fill_all_months_changed = true;
                    for(int m(1); m < 13; m++)
                    {
                        flood_fill(m, seed_point);
                    }
                }
            }
            else
            {
                m_terrain_position_tracker.setStartPosition(intersection_point[0],0,intersection_point[2]);
            }
        }
    }
}

void GLWidget::flood_fill(const int m, const glm::ivec2 & seed_point)
{
    std::vector<glm::ivec2> points_to_process;
    TerrainWater & terrain_water(m_resources.getTerrainWater());
    m_floodfill_tracker.reset(m_terrain.getWidth(), m_terrain.getDepth());

    terrain_water.push(m-1);
    float seed_height(m_terrain.getHeight(seed_point)); // to mm
    append_surrounding_points(seed_point, points_to_process, m_floodfill_tracker);
    while(!points_to_process.empty())
    {
        std::vector<glm::ivec2> cpy_point_to_process(points_to_process.begin(), points_to_process.end());
        points_to_process.clear();
        for(glm::ivec2 & point : cpy_point_to_process)
        {
            if(flood_fill(point, terrain_water, m, seed_height))
                append_surrounding_points(point, points_to_process, m_floodfill_tracker);
        }
    }
    makeCurrent();
    terrain_water.pushToGPU(m-1);

    standing_water_set(m);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QOpenGLWidget::mouseMoveEvent(event);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if(m_navigation_enabled)
    {
        if(fps() || (softimage() && event->buttons() == Qt::LeftButton))
        {
            float x = event->x();
            float y = event->y();
            normalizeScreenCoordinates(x,y);

            m_mouse_position_tracker.setEndPosition(x,y);

            float diff_x(m_mouse_position_tracker.getDiffX());
            float diff_y(m_mouse_position_tracker.getDiffY());

            if(fps())
            {
                m_camera.rotate(diff_x, diff_y);
                reset_fps_cursor();
            }
            else // SoftImage
            {
                if(m_mouse_position_tracker.ctrl_pressed)
                {
                    m_camera.rotate(-diff_x, -diff_y);
                }
                else
                {
                    m_camera.move(diff_x > 0 ? Camera::Direction::LEFT : Camera::Direction::RIGHT, std::abs(diff_x));
                    m_camera.move(diff_y > 0 ? Camera::Direction::UP : Camera::Direction::DOWN, std::abs(diff_y));
                }

                float end_x, end_y, end_z;
                m_mouse_position_tracker.getEndPosition(end_x, end_y, end_z);
                m_mouse_position_tracker.setStartPosition(end_x, end_y, end_z);
            }
        }
    }
    else // navigation disabled
    {
        if(edit_infiltration_rate())
        {
            glm::vec3 intersection_point;
            if(get_intersection_point_with_terrain(event->x(), event->y(), intersection_point))
            {
                if(event->buttons() == Qt::LeftButton)
                    update_soil_infiltration_rate(intersection_point);
                m_humidity_rect->setTranformation(glm::translate(glm::mat4x4(), glm::vec3(intersection_point.x, 0, intersection_point.z)));
            }
        }
        else if(!m_flood_fill_mode &&  event->buttons() == Qt::LeftButton) // Selection rectangle
        {
            glm::vec3 intersection_point;
            if(get_intersection_point_with_terrain(event->x(), event->y(), intersection_point) ||
                get_intersection_point_with_base_plane(event->x(), event->y(), intersection_point))
            {
                float start_point_x, start_point_y, start_point_z;
                m_terrain_position_tracker.getStartPosition(start_point_x, start_point_y, start_point_z);

                glm::vec3 rect_min( std::max(0.f,std::min(start_point_x, intersection_point[0])),
                        0,
                        std::max(0.f,std::min(start_point_z, intersection_point[2])));

                glm::vec3 rect_max( std::min((float)m_terrain.getWidth()-1, std::max(start_point_x, intersection_point[0])),
                        0,
                        std::min((float)m_terrain.getDepth()-1, std::max(start_point_z, intersection_point[2])));


                float rec_width(rect_max.x-rect_min.x);
                float rec_depth(rect_max.z-rect_min.z);

                m_selection_rect->resize(rec_width/2.0f, rec_depth/2.0f);

                glm::vec3 rect_center(rect_min.x + (rec_width/2.0f), 0, rect_min.z + (rec_depth/2.0f));

                // Build mtw matrix
                glm::mat4x4 mtw_mat = glm::translate(glm::mat4x4(), rect_center);
                m_selection_rect->setTranformation(mtw_mat);
            }
        }

        if(m_dialogs.m_pointer_info_dlg.isVisible())
            update_info_pointer_dlg(glm::vec2(event->x(), event->y()));
    }
}

void GLWidget::wheelEvent(QWheelEvent * wheel)
{
    QOpenGLWidget::wheelEvent(wheel);

    float delta = .0f;

    QPoint pix = wheel->pixelDelta();
    QPoint deg = wheel->angleDelta();

    if(!pix.isNull()) // screen resolution tracking, e.g., from magic mouse
        delta = (float) pix.y() / 360.f;
    else if(!deg.isNull()) // mouse wheel instead
        delta = deg.y() / 360.f;

    bool ctrl_pressed(m_ctrl_pressed.load());

    if(edit_infiltration_rate())
    {
        int increment ( ctrl_pressed ? 10 : 1 );
        if(delta < 0)
            increment *= -1;
        m_humidity_rect->increment_size(increment, increment);
    }
    else
    {
        int boost(ctrl_pressed ? 10 : 1);
        m_camera.increment_fov(delta > 0 ? -0.01 * boost : 0.01 * boost);
    }
}

void GLWidget::append_surrounding_points(const glm::ivec2 & pos, std::vector<glm::ivec2> & surrounding_points, FloodFillTracker & tracker)
{
    // Start 8 surrounding points
    for(int x(std::max(0, (int) pos.x-1)); x <= std::min(m_terrain.getWidth()-1, (int) pos.x+1); x++)
    {
        for(int y(std::max(0, (int) pos.y-1)); y <= std::min(m_terrain.getDepth()-1, (int) pos.y+1); y++)
        {
            glm::ivec2 neighbour_point(x,y);
            if(!tracker.tracked(neighbour_point))
            {
                tracker.setTracked(neighbour_point);
                surrounding_points.push_back(neighbour_point);
            }
        }
    }
}

void GLWidget::update_soil_infiltration_rate(const glm::vec3 & intersection_point)
{
    makeCurrent();
    glm::vec2 intersection_point_2d(intersection_point.x, intersection_point.z);
    m_resources.getSoilInfiltration().update(intersection_point_2d, m_humidity_rect->w_radius(),
                                             m_terrain.getWidth(), m_terrain.getDepth(), m_overlay_widgets.getSoilInfiltrationRate());

    m_resources.getSoilInfiltration().syncFromGPU();
}

void GLWidget::reset_fps_cursor()
{
    setMouseTracking(false);
    float screen_x(width()/2);
    float screen_y(height()/2);
    QCursor::setPos(mapToGlobal(QPoint(screen_x,screen_y)));
    normalizeScreenCoordinates(screen_x,screen_y);
    m_mouse_position_tracker.setStartPosition(screen_x, screen_y);
    setMouseTracking(true);
}

void GLWidget::show_cursor(bool show)
{
    if(show)
        setCursor(QCursor(Qt::ArrowCursor));
    else
        setCursor(QCursor(Qt::BlankCursor));
}

void GLWidget::setNavigationEnabled(bool enabled)
{
    m_navigation_enabled = enabled;
    if(fps())
    {
        if(m_navigation_enabled)
        {
            show_cursor(false);
            reset_fps_cursor();
        }
        else
        {
            show_cursor(true);
        }
    }
}

void GLWidget::keyPressEvent ( QKeyEvent * event )
{
    QOpenGLWidget::keyPressEvent(event);

    if(event->key() == Qt::Key_R)
    {
        m_camera.reset();
        return;
    }
    if(m_authorise_navigation_mode_switch && event->key() == Qt::Key_Space)
    {
        setNavigationEnabled(!m_navigation_enabled);
        return;
    }
    if(event->key() == Qt::Key_Control)
    {
        m_ctrl_pressed.store(true);
        return;
    }
    if(event->key() == Qt::Key_N)
    {
        m_render_water.store(false);
        return;
    }
    if(event->key() == Qt::Key_H)
    {
        balance_water(true);
        return;
    }
    if(event->key() == Qt::Key_Z && m_ctrl_pressed.load()) // UNDO
    {
        if(m_flood_fill_mode || m_overlay_widgets.waterControllersActive())
        {
            makeCurrent();
            TerrainWater & tw(m_resources.getTerrainWater());
            if(m_previous_flood_fill_all_months_changed)
            {
                for(int i(0); i < 12; i++)
                {
                    tw.pop(i);
                    standing_water_set(i+1);
                }
            }
            else
            {
                int m(month());
                tw.pop(m-1);
                standing_water_set(m);
            }
        }
        return;
    }
    if(m_navigation_enabled)
    {
        if(fps())
        {
            switch(event->key()){
            case Qt::Key_W:
                m_camera.move(Camera::Direction::FORWARD);
                break;
            case Qt::Key_S:
                m_camera.move(Camera::Direction::BACK);
                break;
            case Qt::Key_A:
                m_camera.move(Camera::Direction::LEFT);
                break;
            case Qt::Key_D:
                m_camera.move(Camera::Direction::RIGHT);
                break;
            default:
                break;
            }
        }
    }
    if(m_orientation_widget.editMode())
    {
        switch(event->key()){
        case Qt::Key_Left:
            m_orientation_widget.rotateNorth(OrientationWidget::Orientation::WEST);
            break;
        case Qt::Key_Right:
            m_orientation_widget.rotateNorth(OrientationWidget::Orientation::EAST);
            break;
        default:
            break;
        }
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent * event)
{
    QOpenGLWidget::keyReleaseEvent(event);
    if(event->key() == Qt::Key_Control)
    {
        m_ctrl_pressed.store(false);
        return;
    }
    if(event->key() == Qt::Key_N)
    {
        m_render_water.store(true);
        return;
    }
}

void GLWidget::load_terrain_file()
{
    QFileDialog::Options options(QFileDialog::DontUseNativeDialog);

    QString selectedFilter;

    QString fileName ( QFileDialog::getOpenFileName(this,
                                                    tr("Open Terrain File"),
                                                    QDir::homePath().append("/ter-files/"),
                                                    tr("Terragen Files (*.ter)"),
                                                    &selectedFilter,
                                                    options));

    if (!fileName.isEmpty())
    {
        TerragenFile parsed_terragen_file(fileName.toStdString());
        load_terrain(parsed_terragen_file);
    }
}

void GLWidget::load_terrain(TerragenFile terrain_file)
{
    makeCurrent();
    m_terrain.setTerrain(terrain_file);
}

#define PROGRESS_BAR_HEIGHT 100
#define ORIENTATION_WIDGET_HEIGHT 25
void GLWidget::resizeEvent(QResizeEvent *event)
{
    QOpenGLWidget::resizeEvent(event);
    QSize new_size(event->size());
    int h(new_size.height());
    int w(new_size.width());

    m_progress_bar_widget.setFixedSize(w, PROGRESS_BAR_HEIGHT);
    m_progress_bar_widget.move(0, h-PROGRESS_BAR_HEIGHT);

    m_orientation_widget.setFixedSize(w/2.0f, ORIENTATION_WIDGET_HEIGHT);
    m_orientation_widget.move(w/4.0f, 0);

    m_overlay_widgets.resize(w,h);

    refresh_overlay_texture();
}

void GLWidget::update_info_pointer_dlg(const glm::vec2 &screen_pos)
{
    glm::vec3 intersection_point;
    if(get_intersection_point_with_terrain(screen_pos[0], screen_pos[1], intersection_point))
    {                
        glm::vec2 intersecion_point_2d(intersection_point[0], intersection_point[2]);
        // Altitude
        float altitude(m_terrain.getAltitude(intersecion_point_2d));

        bool shaded;
        int illumination, temperature, soil_infiltration_rate, soil_humidity;
        float slope, water_height, weighted_soil_humidity;
        m_resources.getResourceInfo(intersecion_point_2d, month(), slope, water_height, shaded, illumination,
                                    temperature, soil_infiltration_rate, soil_humidity, weighted_soil_humidity);
        bool shade_valid, temp_valid, illumination_valid;
        m_resources.valid(shade_valid,illumination_valid,temp_valid);

        // Convert water height to mm
        water_height = water_height * m_terrain.getScale() * 1000;

        m_dialogs.m_pointer_info_dlg.update(intersecion_point_2d,
                                            altitude, slope, water_height, soil_infiltration_rate, soil_humidity, weighted_soil_humidity,
                                            shade_valid, shaded,
                                            temp_valid, temperature,
                                            illumination_valid, illumination);
    }
    else
        m_dialogs.m_pointer_info_dlg.invalidPoint();
}
void GLWidget::set_overlay(QAction * active_overlay_action)
{
    active_overlay_action->setChecked(true); // In case it was called to set the overlay

    bool shade_valid, temp_valid, illumination_valid;
    m_resources.valid(shade_valid,illumination_valid,temp_valid);
    m_active_overlay = m_overlay_action_to_overlay_uniform[active_overlay_action];

    if(overlay_clusters() && (m_cluster_membership_texture.width() != m_terrain.getWidth() ||
                              m_cluster_membership_texture.height() != m_terrain.getDepth()))
    {
        m_cluster_membership_texture.reset(m_terrain.getWidth(), m_terrain.getDepth());
    }
    else if(overlay_shade() && !shade_valid)
        refresh_shade(false);
    else if(overlay_temperature() && !temp_valid)
        refresh_temperature(false);
    else if(overlay_illumination() && !illumination_valid)
        refresh_illumination(false);

    refresh_overlay_texture();
}

void GLWidget::refresh_overlay_texture()
{
    if(!overlay_none())
    {
        makeCurrent();
        m_computer.createOverlayTexture(m_overlay_texture.textureId(),
                                        m_terrain,
                                        m_resources,
                                        m_cluster_membership_texture,
                                        m_active_overlay,
                                        month());
    }
}

void GLWidget::refresh_clusters()
{
    refresh_clusters(m_dialogs.m_cluster_info_dlg.clusterCount());
}

void GLWidget::refresh_clusters(int k)
{
    m_fps_callback_timer->stop();

    makeCurrent();
    if(m_cluster_membership_texture.width() != m_terrain.getWidth() ||
            m_cluster_membership_texture.height() != m_terrain.getDepth())
    {
        m_cluster_membership_texture.reset(m_terrain.getWidth(), m_terrain.getDepth());
    }

    // Calculate monthly temperature change
    float monthly_temp_change ( (m_dialogs.m_temp_editor_dlg.getDecTemp() - m_dialogs.m_temp_editor_dlg.getJunTemp())/6.f );

    Clusters resulting_clusters(k);
    std::function<void(Clusters &, ResourceWrapper &, ClusterMembershipTexture &,int)> clustering_function =
            std::bind(&Computer::kMeansCluster, &m_computer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);


    m_clusterer.perform_clustering(k,
                                   m_resources,
                                   resulting_clusters,
                                   m_cluster_membership_texture,
                                   monthly_temp_change,
                                   clustering_function); CE();


    m_dialogs.m_cluster_info_dlg.setClusters(resulting_clusters);

    // Format clusters
    std::vector<ClusterData> cluster_data;
    for(int i(0); i < k; i++)
    {
        cluster_data.push_back(resulting_clusters.getClusterData(i));
    }
    m_dialogs.m_plant_placement_dlg.setClusters(cluster_data);

    refresh_overlay_texture();

    m_fps_callback_timer->start();
}

void GLWidget::refresh_temperature(int jun_temp, int dec_temp, float lapse_rate, bool refresh_overlay)
{
    // Disable fps callback temporarily
    m_fps_callback_timer->stop();

    m_resources.refreshTemperature(m_terrain, jun_temp, dec_temp, lapse_rate);

    if(refresh_overlay)
        refresh_overlay_texture();

   // Disable fps callback temporarily
   m_fps_callback_timer->start();
}

void GLWidget::refresh_temperature(bool refresh_overlay)
{
    refresh_temperature(m_dialogs.m_temp_editor_dlg.getJunTemp(), m_dialogs.m_temp_editor_dlg.getDecTemp(), m_dialogs.m_temp_editor_dlg.getLapseRate(), refresh_overlay);
}

void GLWidget::reset_soil_infiltration_rate()
{
    m_fps_callback_timer->stop();
    makeCurrent();
    m_resources.getSoilInfiltration().reset(m_terrain.getWidth(), m_terrain.getDepth());

    m_fps_callback_timer->start();
}

void GLWidget::refresh_water()
{
    m_fps_callback_timer->stop();

    reset_water();

    makeCurrent();

    //  CALCULATE SOIL HUMIDITY AND STANDING WATER
    TerrainWater & terrain_water(m_resources.getTerrainWater());
    for(int i = 0; i < 12; i++)
    {
        m_computer.calculateStandingWater(m_resources,
                                          i+1,
                                          m_dialogs.m_monthly_rainfall_edit_dlg.getRainfall(i+1),
                                          m_dialogs.m_monthly_rainfall_edit_dlg.getRainfallIntensity(i+1),
                                          m_terrain.getWidth(),
                                          m_terrain.getDepth(),
                                          m_terrain.getScale());
    }

    tmp_water_flow_iteration_count = 0;
    terrain_water.setUnbalanced();
    set_edit_actions_active(false); // Disable editing until water is balanced

    refresh_overlay_texture();
    // Disable fps callback temporarily
    m_fps_callback_timer->start();
}

void GLWidget::refresh_illumination(bool refresh_overlay)
{
    // Disable fps callback temporarily
    m_fps_callback_timer->stop();

    // Temporarily disable signaling on sun position change (as it will recursively call this function)
    m_lighting_manager.signal_sun_position_change(false);
    m_resources.refreshDailyIllumination(m_lighting_manager, m_terrain);
    m_lighting_manager.signal_sun_position_change(true); // re-enable signaling

    m_fps_callback_timer->start();

    if(refresh_overlay)
        refresh_overlay_texture();
}

void GLWidget::refresh_shade(bool refresh_overlay)
{
    // Disable fps callback temporarily
    m_fps_callback_timer->stop();

    m_resources.refreshShade(m_terrain, m_lighting_manager.getSunlightProperties().getPosition());

    m_fps_callback_timer->start();

    if(refresh_overlay)
        refresh_overlay_texture();
}

void GLWidget::reset_water()
{
    makeCurrent();
    // RESET SOIL HUMIDITY
    m_resources.getSoilHumidity().reset(m_terrain.getWidth(), m_terrain.getDepth());
    // RESET WEIGHTED SOIL HUMIDITY
    m_resources.getWeightedSoilHumidity().reset(m_terrain.getWidth(), m_terrain.getDepth());
    // RESET TERRAIN WATER
    m_resources.getTerrainWater().reset(m_terrain.getWidth(), m_terrain.getDepth());
}

void GLWidget::refresh_normals()
{
    makeCurrent();

    m_renderer.calculateNormals(m_terrain);

    // First ensure the size is correct
    Slope & slope (m_resources.getSlope());
    slope.reset(m_terrain.getWidth(), m_terrain.getDepth());
    m_computer.convertNormalsToSlope(m_terrain.getNormals(), slope);
    slope.syncFromGPU();

    // CALCULATE MIN/MAX
    float min(slope(0,0)), max(slope(0,0));
    for(int x(0); x < m_terrain.getWidth(); x++)
        for(int y(0); y < m_terrain.getWidth(); y++)
        {
            int v( slope(x,y) );
            if(v < min)
                min = v;
            if(v > max)
                max = v;
        }
    slope.setMin(min);
    slope.setMax(max);
}

void GLWidget::sunPositionChanged(float pos_x, float pos_y, float pos_z)
{
    glm::mat4x4 transform ( glm::translate(glm::mat4x4(), glm::vec3(pos_x, pos_y, pos_z)) );
    m_sun.setTranformation(transform);
}

void GLWidget::set_overlays_active(bool active)
{
    for(QAction * action : m_actions->m_overlay_actions.allActions())
        action->setEnabled(active);

    if(!active)
    {
        set_overlay(m_actions->m_overlay_actions[OverlayActionFamily::_NONE]);
    }
}

void GLWidget::set_edit_actions_active(bool active)
{
    for(QAction * action : m_actions->m_edit_actions.allActions())
        action->setEnabled(active);
}

void GLWidget::balance_water(bool one_step)
{
    makeCurrent();

    TerrainWater & terrain_water(m_resources.getTerrainWater());
    if(one_step)
    {
        int m(month());
        terrain_water.incrementBalancingIteration(m, m_computer.balanceWater(m_padded_terrain, terrain_water, m));
        terrain_water.syncFromGPU(m-1);
    }
    else
    {
        for(int m(1); m < 13; m++)
        {
            if(!terrain_water.balanced(m))
            {
                terrain_water.incrementBalancingIteration(m, m_computer.balanceWater(m_padded_terrain, terrain_water, m));
                terrain_water.syncFromGPU(m-1);
            }
        }
    }
}

void GLWidget::zeroify_soil_infiltration_above_slope(int min_slope)
{
    makeCurrent();
    m_computer.slopeBasedInfiltrationRateFilter(m_resources.getSlope(), m_resources.getSoilInfiltration(), min_slope);
}

void GLWidget::fill_infiltration_rate(int infiltration_rate)
{
    int sz(m_terrain.getWidth() * m_terrain.getDepth() * sizeof(GLuint));
    GLuint * data = (GLuint*) malloc(sz);

    if(infiltration_rate == 0) // Optimization
    {
        std::memset(data, 0, sz);
    }
    else
    {
#pragma omp parallel for
        for(int x = 0; x < m_terrain.getWidth(); x++)
        {
#pragma omp parallel for
            for(int z = 0; z < m_terrain.getDepth(); z++)
            {
                data[x*m_terrain.getWidth()+z] = infiltration_rate;
            }
        }
    }
    makeCurrent();
    m_resources.getSoilInfiltration().setData(data);
}

void GLWidget::soil_infiltration_rate_changed(int infiltration_rate)
{
    glm::vec4 rect_color(0, 0, infiltration_rate/SoilInfiltrationControllerWidget::_MAX_INFILTRATION_RATE, .5);
    m_humidity_rect->setColor(rect_color);
}

bool GLWidget::flood_fill(const glm::ivec2 & point, TerrainWater & terrain_water, int month, float & seed_height)
{
    float terrain_height(m_terrain.getHeight(point)); // x 1000 to convert to mm

    if(terrain_height < seed_height)
    {
        terrain_water.set(month-1, seed_height-terrain_height, point.x, point.y);
        return true;
    }

    return false;
}

void GLWidget::new_terrain_loaded(int width, int depth)
{
    format_overlay_texture(width, depth);
    m_padded_terrain.refresh(m_terrain); // refresh the padded terrain for water flow calculations

    m_lighting_manager.setTerrainDimensions(width, depth);
    m_resources.terrainChanged(width, depth);

    refresh_water();
    refresh_temperature(false);

    reset_edit();
}

void GLWidget::new_terrain_is_going_to_load(int width, int depth)
{
    m_selection_rect->clear();
    m_dialogs.m_monthly_rainfall_edit_dlg.reset();
    m_overlay_widgets.hideAll();
    reset_overlay();
    set_overlays_active(true);
}

void GLWidget::set_absolute_aggregate_height(int height)
{
    float terrain_scale_height(height/m_terrain.getScale());
    TerrainWater & terrain_water(m_resources.getTerrainWater());
    int m(month());
    makeCurrent();

    m_computer.setAbsoluteAggregateHeight(m_terrain,
                                          terrain_water[m-1]->textureId(),
                                          terrain_scale_height);
    terrain_water.syncFromGPU(month()-1, true);

    standing_water_set(m);
}

void GLWidget::clear_rays()
{
    m_rays.clear();
}

void GLWidget::reset_overlay()
{
    set_overlay(m_actions->m_overlay_actions[OverlayActionFamily::_NONE]);
}

void GLWidget::reset_edit()
{
    for(QAction * action : m_actions->m_edit_actions.allActions())
        action->setChecked(false);

    m_orientation_widget.toggle_edit_mode(false);
    set_flood_fill_enabled(false);
}

void GLWidget::edit(QAction * triggered_action)
{
    // Enable all overlays & edit actions
    set_overlays_active(true);
    m_overlay_widgets.hideAll();
    bool checked(triggered_action->isChecked());
    reset_edit();
    triggered_action->setChecked(checked);
    // If it's to trigger a dialog
    if(triggered_action == m_actions->m_edit_actions[EditActionFamily::_TEMPERATURE])
        m_dialogs.showTempDialog();
    else if(triggered_action == m_actions->m_edit_actions[EditActionFamily::_MONTHLY_RAINFALL])
        m_dialogs.showMonthlyRainfallDialog();
    else if(triggered_action ==  m_actions->m_edit_actions[EditActionFamily::_ORIENTATION])
    {
        if(checked)
        {
            m_orientation_widget.toggle_edit_mode(true);
        }
        else
            m_orientation_widget.toggle_edit_mode(false);
    }
    else if(triggered_action ==  m_actions->m_edit_actions[EditActionFamily::_TIME])
    {
        if(checked)
        {
            m_overlay_widgets.triggerTimeControllers(true);
        }
    }
    else if(triggered_action ==  m_actions->m_edit_actions[EditActionFamily::_LATITUDE])
    {
        if(checked)
        {
            m_overlay_widgets.triggerLatitudeControllers(true);
        }
    }
    else if(triggered_action ==  m_actions->m_edit_actions[EditActionFamily::_SOIL_INFILTRATION_RATE])
    {
        if(checked)
        {
            set_overlays_active(false);

            m_overlay_widgets.triggerSoilInfiltrationControllers(true);
            reset_water();
            soil_infiltration_rate_changed(m_overlay_widgets.getSoilInfiltrationRate());
            set_overlay(m_actions->m_overlay_actions[OverlayActionFamily::_SOIL_INFILTRATION_RATE]);
            refresh_overlay_texture();
        }
        else
        {
            refresh_water();
            reset_overlay();
        }
    }
    else if(triggered_action ==  m_actions->m_edit_actions[EditActionFamily::_ABSOLUTE_AGGREGATE_HEIGHT])
    {
        if(checked)
        {
            m_overlay_widgets.triggerWaterControllers(true);
        }
    }
    else if(triggered_action ==  m_actions->m_edit_actions[EditActionFamily::_FLOOD_FILL])
    {
        set_flood_fill_enabled(checked);
    }
}

void GLWidget::format_overlay_texture(int width, int depth)
{
    makeCurrent();
    m_overlay_texture.setDimensions(width, depth);
}

bool GLWidget::render_rays()
{
    return m_actions->m_render_actions[RenderActionFamily::_RAYS]->isChecked();
}

bool GLWidget::render_grid()
{
    return m_actions->m_render_actions[RenderActionFamily::_GRID]->isChecked();
}

bool GLWidget::render_sun()
{
    return m_actions->m_render_actions[RenderActionFamily::_SUN]->isChecked();
}

bool GLWidget::render_terrain()
{
    return m_actions->m_render_actions[RenderActionFamily::_TERRAIN]->isChecked();
}

bool GLWidget::overlay_none()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_NONE]->isChecked();
}

bool GLWidget::overlay_slope()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_SLOPE]->isChecked();
}

bool GLWidget::overlay_altitude()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_ALTITUDE]->isChecked();
}

bool GLWidget::overlay_shade()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_SHADE]->isChecked();
}

bool GLWidget::overlay_temperature()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_TEMPERATURE]->isChecked();
}

bool GLWidget::overlay_illumination()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_ILLUMINATION]->isChecked();
}

bool GLWidget::overlay_soil_infiltration()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_SOIL_INFILTRATION_RATE]->isChecked();
}

bool GLWidget::overlay_monthly_soil_humidity()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_MONTHLY_SOIL_HUMIDITY]->isChecked();
}

bool GLWidget::overlay_weighted_avg_soil_humidity()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_WEIGHTED_AVG_SOIL_HUMIDITY]->isChecked();
}

bool GLWidget::overlay_clusters()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_CLUSTERS]->isChecked();
}

bool GLWidget::edit_infiltration_rate()
{
    return m_actions->m_edit_actions[EditActionFamily::_SOIL_INFILTRATION_RATE]->isChecked();
}

bool GLWidget::fps()
{
    return m_dialogs.m_settings_dlg.fps();
}

bool GLWidget::softimage()
{
    return m_dialogs.m_settings_dlg.softimage();
}

glm::vec3 GLWidget::to_world(const glm::vec3 & screen_coord)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // unproject screen point to derive world coordinates
    float window_x ( screen_coord[0] );
    float window_y ( m_height.load() - screen_coord[1] );
    float window_z ( screen_coord[2] );

    glm::mat4x4 proj_mat;
    glm::mat4x4 view_mat;
    m_camera.getTransforms(proj_mat, view_mat);

    glm::vec3 window_pos = glm::vec3(window_x, window_y, window_z); // Actual window position
    glm::vec3 world_pos = glm::unProject(window_pos, view_mat, proj_mat, glm::vec4(viewport[0], viewport[1], m_width.load(), m_height.load()));
    return glm::vec3(world_pos.x, world_pos.y, world_pos.z);
}

int GLWidget::month()
{
    m_overlay_widgets.getMonth();
}

void GLWidget::reset_clusters()
{
    m_cluster_membership_texture.reset(m_terrain.getWidth(), m_terrain.getDepth());
    m_dialogs.m_cluster_info_dlg.clear();
}

void GLWidget::trigger_resource_mode()
{
    hide_all_dialogs();
    reset_overlay();
}

void GLWidget::trigger_clustering_mode()
{
    hide_all_dialogs();
    reset_edit();
    makeCurrent();

    bool temp_valid, illumination_valid, shade_valid;
    m_resources.valid(shade_valid, illumination_valid, temp_valid);
    if(!temp_valid)
        refresh_temperature();
    if(!illumination_valid)
        refresh_illumination();

    refresh_clusters();

    set_overlay(m_actions->m_overlay_actions[OverlayActionFamily::_CLUSTERS]);

    m_dialogs.showClusterInfoDialog();
}

void GLWidget::trigger_plant_mode()
{
    hide_all_dialogs();
    reset_edit();
    makeCurrent();

    set_overlay(m_actions->m_overlay_actions[OverlayActionFamily::_CLUSTERS]);

    m_dialogs.showPlantSelectionDialog();
}

void GLWidget::hide_all_dialogs()
{
    m_dialogs.hideAll();
}

void GLWidget::temp_invalidated()
{
    if(overlay_temperature())
        reset_overlay();
}

void GLWidget::shade_invalidated()
{
    if(overlay_shade())
        reset_overlay();
}

void GLWidget::daily_illumination_invalidated()
{
    if(overlay_illumination())
        reset_overlay();
}

void GLWidget::monthChanged()
{
    if(overlay_temperature() || overlay_monthly_soil_humidity() || overlay_weighted_avg_soil_humidity() || overlay_illumination()) // Month dependant overlays
        refresh_overlay_texture();
}

void GLWidget::standing_water_set(int month)
{
    int layer(month-1);

    TerrainWater & standing_water (m_resources.getTerrainWater());
    standing_water.syncFromGPU(layer);

#pragma omp parallel for
    for(int x = 0; x < m_terrain.getWidth(); x++)
    {
        for(int y = 0; y < m_terrain.getDepth(); y++)
        {
            float water_height(standing_water(layer, x, y));
            if(water_height * m_terrain.getScale() * 100 < 1) // First ensure it isn't below 1 cm
                standing_water.set(layer,0,x,y);
        }
    }
    standing_water.pushToGPU(layer);

    // CALCULATE SOIL HUMIDITY
    {
        m_computer.calculateSoilHumidity(m_resources,
                                         month,
                                         m_dialogs.m_monthly_rainfall_edit_dlg.getRainfall(month),
                                         m_dialogs.m_monthly_rainfall_edit_dlg.getRainfallIntensity(month),
                                         m_terrain.getWidth(),
                                         m_terrain.getDepth(),
                                         m_terrain.getScale());
        m_resources.getSoilHumidity().syncFromGPU(month-1);
    }

    //  CALCULATE WEIGHTED SOIL HUMIDITY
    {        
        WeightedSoilHumidity & wsh (m_resources.getWeightedSoilHumidity());
        m_computer.calculateWeightedSoilHumidity(m_resources.getSoilHumidity(), wsh);
        wsh.syncFromGPU();

        // Calculate min max
        for(int m(1); m <13; m++)
        {
            int min(wsh(m-1, 0, 0)), max(wsh(m-1, 0, 1));

            for(int x(0); x < wsh.width(); x++)
                for(int y(0); y < wsh.height(); y++)
                {
                    int v(wsh(month-1, x, y) );
                    if(v < min)
                        min = v;
                    if(v > max)
                        max = v;
                }
            wsh.setMin(m, min);
            wsh.setMax(m, max);
        }

    }

    if(standing_water.balanced()) // Reenable all edit actions once the water is "balanced"
        set_edit_actions_active(true);
}

void GLWidget::refresh_plants()
{
    m_fps_callback_timer->stop();

    m_simulator.start(m_dialogs.m_plant_placement_dlg.getRunConfigs());

    m_fps_callback_timer->start();
}

void GLWidget::refresh_camera_properties()
{
    m_camera.setTranslationSensitivity(m_dialogs.m_settings_dlg.getTranslationSensitivity());
    m_camera.setRotationSensitivity(m_dialogs.m_settings_dlg.getRotationSensitivity());
}

// THREAD
void GLWidget::enable_continuous_mouse_tracking(bool enable)
{
    if(enable && !m_mouse_tracking_thread)
    {
        m_mouse_tracking_thread = new std::thread(&GLWidget::mouse_tracking_callback, this);
    }
    else if(!enable && m_mouse_tracking_thread)
    {
        m_mouse_tracking_thread_run.store(false);
        m_mouse_tracking_thread->join();
        delete m_mouse_tracking_thread;
        m_mouse_tracking_thread = NULL;
    }
}

#define MAX_ROTATION 2.f
void GLWidget::mouse_tracking_callback()
{
    while(m_mouse_tracking_thread_run.load())
    {
        float width(m_width.load());
        float height(m_height.load());

        float yaw_r ((m_mouse_x.load() - (width/2.f)) / width/2.f);
        float pitch_r ((m_mouse_y.load() - (height/2.f)) / height/2.f);

        float yaw(.0f);
        float pitch(.0f);

        if(fabs(yaw_r ) > .1f)
            yaw = yaw_r * MAX_ROTATION;

        if(fabs(pitch_r) > .1f)
            pitch = pitch_r * MAX_ROTATION;

        m_camera.rotate(yaw, pitch);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

