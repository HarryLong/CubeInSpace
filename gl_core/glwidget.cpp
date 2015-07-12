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
#include "shaders/shaders.h"
#include "lighting/lighting_manager.h"
#include <QTimer>
#include "terragen_file_manager/terragen_file_manager.h"
#include <QOpenGLFunctions_4_3_Core>
#include <QFileDialog>
#include "../actions.h"

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

const int GLWidget::_TARGET_FPS = 60;
GLWidget::GLWidget(AllActions * actions, QWidget * parent) :
  QOpenGLWidget(parent),
  m_progress_bar_widget(this),
  m_actions(actions),
  m_overlay_widgets(this),
  m_navigation_enabled(false), m_mouse_tracking_thread(NULL), m_authorise_navigation_mode_switch(true),
  m_fps_callback_timer(new QTimer),
  m_dialogs(parent),
  m_active_overlay(Uniforms::Overlay::_NONE),
  m_shaders(this),
  m_orientation_widget(this)
{
    // Set up camera properties
    m_camera.setTranslationSensitivity(m_dialogs.m_settings_dlg.getTranslationSensitivity());
    m_camera.setRotationSensitivity(m_dialogs.m_settings_dlg.getRotationSensitivity());

    // Set up orientation widget
    m_orientation_widget.setCameraDirection(m_camera.getCameraDirection());

    // Set up lighting manager
    m_lighting_manager.setLatitude(m_overlay_widgets.getLatitude());
    m_lighting_manager.setMonth(m_overlay_widgets.getMonth());
    m_lighting_manager.setTime(m_overlay_widgets.getTime());

    m_mouse_tracking_thread_run.store(true);
    m_ctrl_pressed.store(false);
    setFocusPolicy(Qt::ClickFocus);
    establish_connections();
    setNavigationEnabled(true);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    m_fps_callback_timer->stop();
    enable_continuous_mouse_tracking(false);
    delete m_mouse_tracking_thread;
}

void GLWidget::establish_connections()
{
    // LOAD NEW TERRAIN
    connect(m_actions->m_base_actions[BaseActionFamily::_LOAD_TERRAIN], SIGNAL(triggered(bool)), this, SLOT(load_terrain_file()));

    // OVERLAY WIDGETS
    connect(m_actions->m_edit_actions[EditActionFamily::_TIME], SIGNAL(triggered(bool)), &m_overlay_widgets, SLOT(trigger_time_controllers(bool)));
    connect(m_actions->m_edit_actions[EditActionFamily::_LATITUDE], SIGNAL(triggered(bool)), &m_overlay_widgets, SLOT(trigger_latitude_controllers(bool)));

    // TERRAIN DIMENSTIONS CHANGED
    connect(&m_terrain, SIGNAL(terrainDimensionsChanged(int,int,int,int)), &m_lighting_manager, SLOT(setTerrainDimensions(int,int,int,int)));
    connect(&m_terrain, SIGNAL(terrainDimensionsChanged(int,int,int,int)), this, SLOT(refresh_water()));
    connect(&m_terrain, SIGNAL(terrainDimensionsChanged(int,int,int,int)), &m_resources, SLOT(terrainChanged()));

    // CAMERA ORIENTATION CHANGE
    connect(&m_camera, SIGNAL(cameraDirectionChanged(float,float,float)), &m_orientation_widget, SLOT(setCameraDirection(float,float,float)));

    // CONTROL STYLE CHANGE
    connect(m_actions->m_control_actions.getActionGroup(), SIGNAL(triggered(QAction*)), this, SLOT(control_changed()));

    // DISPLAY DIALOGS
    connect(m_actions->m_show_actions[ShowActionFamily::_POINTER_INFO], SIGNAL(triggered(bool)), &m_dialogs, SLOT(triggerPointerInfoDialog(bool)));
    connect(m_actions->m_base_actions[BaseActionFamily::_OPEN_SETTINGS], SIGNAL(triggered(bool)), &m_dialogs, SLOT(showSettingsDialog()));
    connect(m_actions->m_edit_actions[EditActionFamily::_TEMPERATURE], SIGNAL(triggered(bool)), &m_dialogs, SLOT(showTempDialog()));
    connect(m_actions->m_edit_actions[EditActionFamily::_RAINFALL], SIGNAL(triggered(bool)), &m_dialogs, SLOT(showRainfallDialog()));

    // UPDATE PROGRESS BAR
    connect(&m_resources, SIGNAL(processing(QString)), &m_progress_bar_widget, SLOT(processing(QString)));
    connect(&m_resources, SIGNAL(processingUpdate(int)), &m_progress_bar_widget, SLOT(updateProgress(int)));
    connect(&m_resources, SIGNAL(processingComplete()), &m_progress_bar_widget, SLOT(finishedProcessing()));
    connect(&m_resources, SIGNAL(processDescriptionUpdate(QString)), &m_progress_bar_widget, SLOT(updateInfoLabel(QString)));

    // CLEAR RENDERED RAYS
    connect(m_actions->m_render_actions[RenderActionFamily::_RAYS], SIGNAL(triggered(bool)), this, SLOT(clear_rays()));

    // When overlay is changed
    connect(m_actions->m_overlay_actions.getActionGroup(), SIGNAL(triggered(QAction*)), this, SLOT(overlay_changed()));

    // When controller widgets are activated
    connect(&m_overlay_widgets, SIGNAL(latitudeControllersStateChanged(bool)), this, SLOT(latitude_controllers_state_changed(bool)));
    connect(&m_overlay_widgets, SIGNAL(timeControllersStateChanged(bool)), this, SLOT(time_controllers_state_changed(bool)));

    // When sun position changed
    connect(&m_lighting_manager, SIGNAL(sunPositionChanged(float,float,float)), this, SLOT(sunPositionChanged(float,float,float)));
    connect(&m_lighting_manager, SIGNAL(sunPositionChanged(float,float,float)), &m_resources, SLOT(sunPositionChanged()));

    // When latitude changes
    connect(&m_overlay_widgets, SIGNAL(latitudeChanged(int)), &m_resources, SLOT(latitudeChanged()));
    connect(&m_overlay_widgets, SIGNAL(latitudeChanged(int)), &m_lighting_manager, SLOT(setLatitude(int)));

    // MONTH CHANGE
    connect(&m_overlay_widgets, SIGNAL(monthChanged(int)), &m_lighting_manager, SLOT(setMonth(int)));

    // TIME CHANGE
    connect(&m_overlay_widgets, SIGNAL(timeChanged(int)), &m_lighting_manager, SLOT(setTime(int)));

    // ORIENTATION CHANGE
    connect(&m_orientation_widget, SIGNAL(northOrientationChanged(float,float,float)), &m_lighting_manager, SLOT(setNorthOrientation(float,float,float)));

    // ORIENTATION EDIT
    connect(m_actions->m_edit_actions[EditActionFamily::_ORIENTATION], SIGNAL(triggered(bool)), &m_orientation_widget, SLOT(toggle_edit_mode(bool)));

    // CAMERA SENSITIVITY
    connect(&m_dialogs.m_settings_dlg, SIGNAL(rotationSensitivityChanged(float)), &m_camera, SLOT(setRotationSensitivity(float)));
    connect(&m_dialogs.m_settings_dlg, SIGNAL(translationSensitivityChanged(float)), &m_camera, SLOT(setTranslationSensitivity(float)));

    // TEMPERATURE
    connect(&m_dialogs.m_temp_editor_dlg, SIGNAL(accepted()), this, SLOT(refresh_temperature()));

    // WATER
    connect(&m_dialogs.m_rainfall_editor_dlg, SIGNAL(accepted()), this, SLOT(refresh_water()));

    // FPS CALLBACK
    connect(m_fps_callback_timer, SIGNAL(timeout()), this, SLOT(update()));

    // WHEN A RESOUCE IS INVALIDATED CHECK REFRESH IT IF NECESSARY
    connect(&m_resources, SIGNAL(resourceInvalidated()), this, SLOT(overlay_changed()));
}

void GLWidget::control_changed()
{
    if(fps())
    {
        show_cursor(false);
        reset_fps_cursor();
    }
    else // SOFTIMAGE
    {
        show_cursor(true);
        setNavigationEnabled(true);
    }
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    setMouseTracking(true);

    m_shaders.compileAndLink();
    m_terrain.loadBaseTerrain(); // Load the base terrain

    m_fps_callback_timer->start((int)(1000.0f/GLWidget::_TARGET_FPS));
}

void GLWidget::paintGL() // Override
{
    static int i(0);
    if(!m_fps_callback_timer->isActive())
        return;

    m_resources.syncTextures(); // Needs openGL context to be active    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CE();

    glm::mat4x4 proj_mat;
    glm::mat4x4 view_mat;

    m_camera.getTransforms(proj_mat, view_mat);
    Transform tranform(view_mat, proj_mat);

    std::vector<Asset*> assets_to_render;

    // Draw the grid
    if(render_grid())
    {
        std::vector<Asset*> grid_assets (m_grid.getAssets());
        assets_to_render.insert(assets_to_render.end(), grid_assets.begin(), grid_assets.end());CE();
    }

    // Draw the terrain
    if(render_terrain())
    {
        TerrainWater & terrain_water (m_resources.getTerrainWater());
        // Check if we need calculate water balance
        if(!terrain_water.balanced())
        {
            // Jun water balance
            m_renderer.balanceWater(m_shaders.m_water_flux_generator, m_terrain, terrain_water.getJunTextureId());

            // Dec water balance
//            m_renderer.balanceWater(m_shaders.m_water_flux_generator, terrain->getDrawableTerrain(), terrain_water->getDecTextureId());

//            terrain_water->syncFromGPU();
//            terrain_water->setBalanced(true);
        }

        // CALCULATE NORMALS
        if(!m_terrain.normalsValid())
        {
//            context()->swapBuffers(m_terrain.getNormals().getOffscreenSurface()); CE();
            m_renderer.calculateNormals(m_shaders.m_normals_generator, m_terrain);
            makeCurrent();
        }

        // RENDER TERRAIN
        m_renderer.renderTerrain(m_shaders.m_terrain, m_terrain, m_resources, tranform, m_lighting_manager.getSunlightProperties(), m_active_overlay, month());

        // Terrain elements
        std::vector<Asset*> terrain_elements_to_render;
        terrain_elements_to_render.push_back(m_terrain.getSelectionRect());
        m_renderer.renderTerrainElements(m_shaders.m_terrain_elements, m_terrain, tranform, terrain_elements_to_render) ;
    }

    // Draw the rays
    if(render_rays())
        assets_to_render.push_back(&m_rays);

    // Draw the sun
    if(render_sun())
        assets_to_render.push_back(&m_sun);

    m_renderer.renderAssets(m_shaders.m_base, tranform, assets_to_render);
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
            m_terrain_position_tracker.setStartPosition(intersection_point[0],0,intersection_point[2]);
        }
    }
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
                    m_camera.rotate(diff_x, diff_y);
                }
                else
                {
                    m_camera.move(diff_x > 0 ? Camera::Direction::RIGHT : Camera::Direction::LEFT);
                    m_camera.move(diff_y > 0 ? Camera::Direction::UP : Camera::Direction::DOWN);
                }

                float end_x, end_y, end_z;
                m_mouse_position_tracker.getEndPosition(end_x, end_y, end_z);
                m_mouse_position_tracker.setStartPosition(end_x, end_y, end_z);
            }
        }
    }
    else // navigation disabled
    {
        if(event->buttons() == Qt::LeftButton)
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

                m_terrain.setSelectionRectangle(rect_min, rect_max);
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


    if(m_navigation_enabled)
    {
        m_camera.move(delta > 0 ? Camera::Direction::FORWARD : Camera::Direction::BACK);
    }
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
}

void GLWidget::load_terrain_file()
{
    QFileDialog::Options options;
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
        reset_overlay();
        makeCurrent();
        m_terrain.setTerrain(parsed_terragen_file);
    }
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
}

void GLWidget::update_info_pointer_dlg(const glm::vec2 &screen_pos)
{
    glm::vec3 intersection_point;
    if(get_intersection_point_with_terrain(screen_pos[0], screen_pos[1], intersection_point))
    {                
        glm::vec2 intersecion_point_2d(intersection_point[0], intersection_point[2]);
        // Altitude
        float altitude(m_terrain.getAltitude(intersecion_point_2d));

        // Slope
        float slope(m_terrain.getSlope(intersecion_point_2d));

        bool shaded;
        int water_height, min_daily_illumination, max_daily_illumination;
        float temperature;
        m_resources.getResourceInfo(intersecion_point_2d, month(), water_height, shaded, min_daily_illumination, max_daily_illumination, temperature);
        bool shade_valid, temp_valid, illumination_valid;
        m_resources.valid(shade_valid,illumination_valid,temp_valid);

        m_dialogs.m_pointer_info_dlg.update(altitude, slope, water_height,
                                   shade_valid, shaded,
                                   temp_valid, temperature,
                                   illumination_valid, min_daily_illumination, max_daily_illumination);
    }
    else
        m_dialogs.m_pointer_info_dlg.invalidPoint();
}
void GLWidget::overlay_changed()
{
    bool shade_valid, temp_valid, illumination_valid;
    m_resources.valid(shade_valid,illumination_valid,temp_valid);

    if(overlay_none())
        m_active_overlay = Uniforms::Overlay::_NONE;
    else if(overlay_slope())
        m_active_overlay = Uniforms::Overlay::_SLOPE;
    else if(overlay_altitude())
        m_active_overlay = Uniforms::Overlay::_ALTITUDE;
    else if(overlay_shade())
    {
        if(!shade_valid)
            refresh_shade();
        m_active_overlay = Uniforms::Overlay::_SHADE;
    }
    else if(overlay_temperature())
    {
        if(!temp_valid)
            refresh_temperature();
        m_active_overlay = Uniforms::Overlay::_TEMPERATURE;
    }
    else if(overlay_min_illumination())
    {
        if(!illumination_valid)
            refresh_illumination();
        m_active_overlay = Uniforms::Overlay::_MIN_DAILY_ILLUMINATION;
    }
    else if(overlay_max_illumination())
    {
        if(!illumination_valid)
            refresh_illumination();
        m_active_overlay = Uniforms::Overlay::_MAX_DAILY_ILLUMINATION;
    }
}

void GLWidget::refresh_temperature()
{
    // Disable fps callback temporarily
    m_fps_callback_timer->stop();

    TemperatureEditDialog::TemperatureAttributes jun_temp_attributes(m_dialogs.m_temp_editor_dlg.getJunTemperatureAttributes());
    TemperatureEditDialog::TemperatureAttributes dec_temp_attributes(m_dialogs.m_temp_editor_dlg.getDecTemperatureAttributes());
    m_resources.refreshTemperature(m_terrain, jun_temp_attributes.temperature_at_zero_meters, jun_temp_attributes.lapse_rate,
                                              dec_temp_attributes.temperature_at_zero_meters, dec_temp_attributes.lapse_rate);

    // Disable fps callback temporarily
    m_fps_callback_timer->start();
}

void GLWidget::refresh_water()
{
    // Disable fps callback temporarily
    m_fps_callback_timer->stop();

    RainfallEditDialog::RainfallAttributes jun_attributes ( m_dialogs.m_rainfall_editor_dlg.getJunRainfallAttributes() );
    RainfallEditDialog::RainfallAttributes dec_attributes ( m_dialogs.m_rainfall_editor_dlg.getDecRainfallAttributes() );

    m_resources.refreshWater(m_terrain.getWidth(), m_terrain.getDepth(), jun_attributes.monthly_rainfall, jun_attributes.rainfall_intensity,
                                                                         dec_attributes.monthly_rainfall, dec_attributes.rainfall_intensity);

    // Disable fps callback temporarily
    m_fps_callback_timer->start();
}

void GLWidget::refresh_illumination()
{
    // Disable fps callback temporarily
    m_fps_callback_timer->stop();

    // Temporarily disable signaling on sun position change (as it will recursively call this function)
    m_lighting_manager.signal_sun_position_change(false);
    m_resources.refreshDailyIllumination(m_lighting_manager, m_terrain);
    m_lighting_manager.signal_sun_position_change(true); // re-enable signaling

    m_fps_callback_timer->start();
}

void GLWidget::refresh_shade()
{
    // Disable fps callback temporarily
    m_fps_callback_timer->stop();

    m_resources.refreshShade(m_terrain, m_lighting_manager.getSunlightProperties().getPosition());

    m_fps_callback_timer->start();
}

void GLWidget::sunPositionChanged(float pos_x, float pos_y, float pos_z)
{
    glm::mat4x4 transform ( glm::translate(glm::mat4x4(), glm::vec3(pos_x, pos_y, pos_z)) );
    m_sun.setTranformation(transform);
}

void GLWidget::time_controllers_state_changed(bool active)
{
    enablePositionDependentOverlays(!active);
}

void GLWidget::latitude_controllers_state_changed(bool active)
{
    enablePositionDependentOverlays(!active);
}

void GLWidget::enablePositionDependentOverlays(bool enable)
{
    if(!enable &&
            (m_active_overlay == Uniforms::Overlay::_MIN_DAILY_ILLUMINATION ||
             m_active_overlay == Uniforms::Overlay::_MAX_DAILY_ILLUMINATION ||
             m_active_overlay == Uniforms::Overlay::_SHADE))
    {
        reset_overlay();
    }

    m_actions->m_overlay_actions[OverlayActionFamily::_SHADE]->setEnabled(enable);
    m_actions->m_overlay_actions[OverlayActionFamily::_MIN_DAILY_ILLUMINATION]->setEnabled(enable);
    m_actions->m_overlay_actions[OverlayActionFamily::_MAX_DAILY_ILLUMINATION]->setEnabled(enable);
}

void GLWidget::clear_rays()
{
    m_rays.clear();
}

void GLWidget::reset_overlay()
{
    m_actions->m_overlay_actions[OverlayActionFamily::_NONE]->setChecked(true);
    overlay_changed();
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

bool GLWidget::overlay_min_illumination()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_MIN_DAILY_ILLUMINATION]->isChecked();
}

bool GLWidget::overlay_max_illumination()
{
    return m_actions->m_overlay_actions[OverlayActionFamily::_MAX_DAILY_ILLUMINATION]->isChecked();
}

bool GLWidget::fps()
{
    return m_actions->m_control_actions[ControlActionFamily::_FPS]->isChecked();
}

bool GLWidget::softimage()
{
    return m_actions->m_control_actions[ControlActionFamily::_SOFTIMAGE]->isChecked();
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
