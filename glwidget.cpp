#include "glwidget.h"

#define FRAGMENT_SHADER_SRC "./shaders/shader.frag"
#define VERTEX_SHADER_SRC "./shaders/shader.vert"

#include <QOpenGLContext>
#include <iostream>
#include <QMouseEvent>
#include "boost/foreach.hpp"
#include "constants.h"
#include <math.h>

GLWidget::GLWidget(const Settings & settings, const QGLFormat& format, QWidget * p_parent) : QGLWidget(format, p_parent),
  m_draw_grid(true), m_draw_assets(true), m_draw_terrain(true), m_control_style(SoftImage),
  m_mouse_tracking_thread(NULL), m_navigation_enabled(true),
  m_view_manager(new ViewManager(settings.z_movement_sensitivity, settings.x_y_movement_sensitivity, settings.camera_sensitivity)),
  m_scene_manager(new SceneManager(settings.terrain_dimension))
{
    m_mouse_tracking_thread_run.store(true);
    setFocusPolicy(Qt::ClickFocus);
}

GLWidget::~GLWidget()
{
    enable_continuous_mouse_tracking(false);

    delete m_renderer;
    delete m_view_manager;
    delete m_scene_manager;
    delete m_mouse_tracking_thread;
}

void GLWidget::updateSettings(const Settings & settings)
{
    m_view_manager->setNavigationProperties(settings.z_movement_sensitivity, settings.x_y_movement_sensitivity, settings.camera_sensitivity);
    m_scene_manager->setTerrainDim(settings.terrain_dimension);
}

void GLWidget::setControlStyle(ControlStyle control_style)
{
    if(m_control_style != control_style)
    {
        if(control_style == Experimental1)
        {
            enable_continuous_mouse_tracking(true);
        }
        if(m_control_style == Experimental1)
        {
            enable_continuous_mouse_tracking(false);
        }
        if(control_style == FPS)
        {
            show_cursor(false);
            reset_fps_cursor();
        }
        if(m_control_style == FPS)
        {
            show_cursor(true);
        }
        m_control_style = control_style;
        setNavigationEnabled(true);
    }
}

void GLWidget::initializeGL() // Override
{
    std::cout << "** OPEN GL INFO **" << std::endl;
    std::cout << "- Version: " << glGetString(GL_VERSION) << std::endl;
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    grabKeyboard();
    setMouseTracking(true);

    // Initialize rendered, view, etc...
    m_renderer = new Renderer(SHADER_DIR);
    m_renderer->printShaders();
    m_scene_manager->initScene();
}

void GLWidget::paintGL() // Override
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw the grid
    if(m_draw_grid)
    {
        DrawData grid_data ( m_scene_manager->getGridData() );
        m_renderer->drawGrid(m_view_manager, grid_data);
    }

    // Draw the terrain
    if(m_draw_terrain && m_scene_manager->terrainReady())
    {
        m_renderer->drawTerrain(m_view_manager, m_scene_manager->getTerrain());
    }

    // Draw the objects
    if(m_draw_assets)
    {
        BOOST_FOREACH(SceneAsset scene_asset, m_scene_manager->getSceneAssets())
        {
            m_renderer->drawAsset(m_view_manager, scene_asset.m_draw_data, scene_asset.m_mtw_matrix);
        }
    }
}

void GLWidget::resizeGL(int width, int height) // Override
{
    int side = qMin(width, height);
    glViewport(0, 0, width, height);

    m_width.store(width);
    m_height.store(height);
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
    grabKeyboard();
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

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(m_navigation_enabled)
    {
        if(m_control_style == SoftImage)
        {
            float x = event->x(); float y = event->y();
            normalizeScreenCoordinates(x,y);

            m_mouse_position_tracker.start_point_x = x;
            m_mouse_position_tracker.start_point_y = y;
            m_mouse_position_tracker.ctrl_pressed = (event->modifiers() == Qt::ControlModifier);
        }
    }
    else
    {

    }

    QGLWidget::mousePressEvent(event);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_navigation_enabled)
    {
        if(m_control_style == Experimental1)
        {
            m_mouse_x.store(event->x());
            m_mouse_y.store(event->y());
        }
        else if(m_control_style == FPS || (m_control_style == SoftImage &&
                                           event->buttons() == Qt::LeftButton))
        {
            float x = event->x();
            float y = event->y();
            normalizeScreenCoordinates(x,y);

            m_mouse_position_tracker.end_point_x = x;
            m_mouse_position_tracker.end_point_y = y;

            float diff_x(m_mouse_position_tracker.getDiffX());
            float diff_y(m_mouse_position_tracker.getDiffY());

            if(m_control_style == FPS)
            {
                m_view_manager->rotate( diff_y,
                                         diff_x);

                reset_fps_cursor();
                update();
            }
            else // SoftImage
            {
                if(m_mouse_position_tracker.ctrl_pressed)
                    m_view_manager->rotate( -diff_y,
                                            -diff_x);
                else
                {
                    m_view_manager->sideStep(diff_x);
                    m_view_manager->up(diff_y);
                }
                m_mouse_position_tracker.start_point_x = m_mouse_position_tracker.end_point_x;
                m_mouse_position_tracker.start_point_y = m_mouse_position_tracker.end_point_y;

                update();
            }
        }
    }
    else // navigation disabled
    {

    }
    QGLWidget::mouseMoveEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent * wheel)
{
    if(m_navigation_enabled)
    {
        float delta = .0f;

        QPoint pix = wheel->pixelDelta();
        QPoint deg = wheel->angleDelta();

        if(!pix.isNull()) // screen resolution tracking, e.g., from magic mouse
            delta = (float) pix.y() / 360.f;
        else if(!deg.isNull()) // mouse wheel instead
            delta = deg.y() / 360.f;

        m_view_manager->forward(-delta);
        update();
    }
    else
    {

    }
    QGLWidget::wheelEvent(wheel);
}

void GLWidget::reset_fps_cursor()
{
    setMouseTracking(false);
    float screen_x(width()/2);
    float screen_y(height()/2);
    QCursor::setPos(mapToGlobal(QPoint(screen_x,screen_y)));
    normalizeScreenCoordinates(screen_x,screen_y);
    m_mouse_position_tracker.start_point_x = screen_x;
    m_mouse_position_tracker.start_point_y = screen_y;
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
    if(m_control_style == FPS)
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

void GLWidget::disableOverlays()
{
    m_renderer->setOverlay(TerrainOverlayUniforms::OVERLAY_DISABLED);
    update();
}

void GLWidget::enableSlopeOverlay()
{
    m_renderer->setOverlay(TerrainOverlayUniforms::SLOPE_OVERLAY);
    update();
}

void GLWidget::enableAltitudeOverlay()
{
    m_renderer->setOverlay(TerrainOverlayUniforms::ALTITUDE_OVERLAY);
    update();
}

void GLWidget::keyPressEvent ( QKeyEvent * event )
{
    if(event->key() == Qt::Key_R)
    {
        m_view_manager->reset_camera();
        update();
        return;
    }
    if(event->key() == Qt::Key_Space)
    {
        setNavigationEnabled(!m_navigation_enabled);
        return;
    }

    if(m_control_style == FPS)
    {
        switch(event->key()){
        case Qt::Key_W:
            m_view_manager->forward(1);
            break;
        case Qt::Key_S:
            m_view_manager->forward(-1);
            break;
        case Qt::Key_A:
            m_view_manager->sideStep(1);
            break;
        case Qt::Key_D:
            m_view_manager->sideStep(-1);
            break;
        }
    }
    else if(m_control_style == Experimental1)
    {
        switch(event->key()){
        case Qt::Key_Up:
            m_view_manager->up(1);
            break;
        case Qt::Key_Down:
            m_view_manager->up(-1);
            break;
        case Qt::Key_Right:
            m_view_manager->sideStep(-1);
            break;
        case Qt::Key_Left:
            m_view_manager->sideStep(1);
            break;
        }
        update();
    }
    update();
    QGLWidget::keyPressEvent(event);
}

void GLWidget::loadTerrain(QString filename)
{
    m_scene_manager->loadTerrain(filename);
    update();
}

void GLWidget::render_grid(bool enabled)
{
   m_draw_grid = enabled;
   update();
}

void GLWidget::render_assets(bool enabled)
{
    m_draw_assets = enabled;
    update();
}

void GLWidget::render_terrain(bool enabled)
{
    m_draw_terrain = enabled;
    update();
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

        m_view_manager->rotate(pitch, yaw);
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
