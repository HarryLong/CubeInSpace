#include "glwidget.h"

#define FRAGMENT_SHADER_SRC "./shaders/shader.frag"
#define VERTEX_SHADER_SRC "./shaders/shader.vert"

#include <QOpenGLContext>
#include <iostream>
#include <QMouseEvent>
#include "constants.h"
#include <math.h>
#include "geom.h"
#include "glm_rotations.h"

GLWidget::GLWidget(const Settings & settings, const QGLFormat& format, QWidget * p_parent) : QGLWidget(format, p_parent),
  m_scene_manager(settings.terrain_scaler),
  m_control_style(ControlStyle::SoftImage), m_active_mode(Mode::None), m_navigation_enabled(false),
  m_mouse_tracking_thread(NULL), m_view_manager(settings.z_movement_sensitivity, settings.x_y_movement_sensitivity, settings.camera_sensitivity),
  m_authorise_navigation_mode_switch(true)
{
    m_mouse_tracking_thread_run.store(true);
    m_ctrl_pressed.store(false);
    setFocusPolicy(Qt::ClickFocus);
}

GLWidget::~GLWidget()
{
    enable_continuous_mouse_tracking(false);

    delete m_mouse_tracking_thread;
}

void GLWidget::updateSettings(const Settings & settings)
{
    m_view_manager.setNavigationProperties(settings.z_movement_sensitivity, settings.x_y_movement_sensitivity, settings.camera_sensitivity);
    m_scene_manager.setTerrainScaler(settings.terrain_scaler);
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

//    grabKeyboard();
    setMouseTracking(true);

    // Initialize rendered, view, etc...

    m_renderer.init();
    m_renderer.printShaders();
    m_scene_manager.initScene();
}

void GLWidget::paintGL() // Override
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<const Asset*> assets_to_render;

    // Draw the grid
    if(m_render_options.m_render_grid)
    {
        std::vector<const Asset*> grid_assets (m_scene_manager.getGrid().getAssets());
        assets_to_render.insert(assets_to_render.end(), grid_assets.begin(), grid_assets.end());
    }

    // Draw the terrain
    if(m_render_options.m_render_terrain)
    {
        Terrain & terrain (m_scene_manager.getTerrain());
        const LightProperties & sunlight_properties (m_scene_manager.getLightingManager().getSunlightProperties());

        m_renderer.renderTerrain(m_view_manager, terrain, sunlight_properties);
        const std::vector<const Asset*> terrain_elements(terrain.getTerrainElements());
        if(terrain_elements.size() > 0)
        {
            m_renderer.renderTerrainElements(m_view_manager, terrain_elements, terrain.getHeightMapTextureUnit());
        }
    }

    // Draw the acceleration structure
    if(m_render_options.m_render_acceleration_structure)
    {
        std::vector<const Asset*> sphere_acceleration_structure (m_scene_manager.getAccelerationStructure());
        assets_to_render.insert(assets_to_render.end(), sphere_acceleration_structure.begin(), sphere_acceleration_structure.end());
    }

    if(m_active_mode == Mode::OrientationEdit)
    {
        OrientationCompass & compass (m_scene_manager.getOrientationCompass());
        std::vector<Asset*> compass_assets(compass.getAssets());

        Terrain & terrain (m_scene_manager.getTerrain());
        glm::vec2 center(terrain.getCenter());

        glm::mat4x4 translation_mat (glm::translate(glm::mat4x4(), glm::vec3(center[0], terrain.getMaxHeight() + 10, center[1])));
        glm::mat4x4 rotation_mat( compass.getNorthRotationMatrix() );
        glm::mat4x4 transformation_mat( translation_mat * rotation_mat );

        for(Asset * asset : compass_assets)
            asset->setMtwMat(transformation_mat);

        assets_to_render.insert(assets_to_render.end(), compass_assets.begin(), compass_assets.end());
    }

    // Draw the rays
    if(m_render_options.m_render_rays)
        assets_to_render.push_back(&m_rays);

    // Draw the sun
    if(m_render_options.m_render_sun)
        assets_to_render.push_back(m_scene_manager.getSun());

    m_renderer.renderAssets(m_view_manager, assets_to_render);
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
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::vec3 start(m_view_manager.toWorld(glm::vec3(screen_x, screen_y, .0f), viewport));
    glm::vec3 end(m_view_manager.toWorld(glm::vec3(screen_x, screen_y, 1.f), viewport));
    glm::vec3 direction(glm::normalize(Geom::diff(end,start)));
    return m_scene_manager.getTerrain().traceRay(start, direction, intersection_point);
}

bool GLWidget::get_intersection_point_with_base_plane(int screen_x, int screen_y, glm::vec3 & intersection_point)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::vec3 start(m_view_manager.toWorld(glm::vec3(screen_x, screen_y, .0f), viewport));
    glm::vec3 end(m_view_manager.toWorld(glm::vec3(screen_x, screen_y, 1.f), viewport));
    glm::vec3 direction(glm::normalize(Geom::diff(end,start)));

    return Geom::rayPlaneIntersection(m_scene_manager.getTerrain().getBaseHeight(), start, direction, intersection_point);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    QGLWidget::mousePressEvent(event);

    if(m_navigation_enabled)
    {
        if(m_control_style == ControlStyle::SoftImage)
        {
            float x = event->x(); float y = event->y();
            normalizeScreenCoordinates(x,y);

            m_mouse_position_tracker.start_point_x = x;
            m_mouse_position_tracker.start_point_y = y;
            m_mouse_position_tracker.ctrl_pressed = (event->modifiers() == Qt::ControlModifier);
        }
    }
    else // Navigation disabled - Perform ray tracing to get intersection point
    {
        glm::vec3 intersection_point;
        bool intersection(get_intersection_point_with_terrain(event->x(), event->y(), intersection_point));

        if(m_render_options.m_render_rays)
        {
            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            glm::vec3 start(m_view_manager.toWorld(glm::vec3(event->x(), event->y(), .0f), viewport));
            glm::vec3 end(m_view_manager.toWorld(glm::vec3(event->x(), event->y(), 1.f), viewport));
            m_rays.add(start, end) ;
            update();
        }

        switch(m_active_mode)
        {
        case Mode::TerrainEdit: {
            int hump_size (10);
            m_active_points.clear();
            if(intersection)
            {
                for(int x(std::max(0.f, intersection_point[0]-hump_size)); x <= std::min(m_scene_manager.getTerrain().getWidth()-1,
                                                                                         (int)intersection_point[0]+hump_size); x++)
                {
                    for(int z(std::max(0.f, intersection_point[2]-hump_size)); z <= std::min(m_scene_manager.getTerrain().getDepth()-1,
                                                                                             (int)intersection_point[2]+hump_size); z++)
                    {
                        m_active_points.push_back(glm::vec3(x, 0, z));
                    }
                }
                Terrain & terrain(m_scene_manager.getTerrain());
                terrain.clearTerrainElements();
                terrain.addTerrainRect(m_active_points[0], m_active_points[m_active_points.size()-1]);
                update();
            } break;
        }
        case Mode::Selection: {
            // Remove the rectangles
            Terrain & terrain(m_scene_manager.getTerrain());
            terrain.clearTerrainElements();
            if(intersection)
            {
                m_mouse_position_tracker.start_point_x = intersection_point[0];
                m_mouse_position_tracker.start_point_z = intersection_point[2];
            }
        } break;
        default:
            break; // Do nothing
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QGLWidget::mouseMoveEvent(event);

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
                m_view_manager.rotate( diff_y,
                                        diff_x);

                reset_fps_cursor();
                update();
            }
            else // SoftImage
            {
                if(m_mouse_position_tracker.ctrl_pressed)
                    m_view_manager.rotate( -diff_y,
                                            -diff_x);
                else
                {
                    m_view_manager.sideStep(diff_x);
                    m_view_manager.up(diff_y);
                }
                m_mouse_position_tracker.start_point_x = m_mouse_position_tracker.end_point_x;
                m_mouse_position_tracker.start_point_y = m_mouse_position_tracker.end_point_y;

                update();
            }
        }
    }
    else // navigation disabled
    {
        switch(m_active_mode){
        case Mode::Selection: {
            if(event->buttons() == Qt::LeftButton)
            {
                Terrain & terrain (m_scene_manager.getTerrain());
                glm::vec3 intersection_point;
                bool intersection(get_intersection_point_with_terrain(event->x(), event->y(), intersection_point));
                if(intersection || get_intersection_point_with_base_plane(event->x(), event->y(), intersection_point))
                {
                    terrain.clearTerrainElements();
                    glm::vec3 rect_min( std::max(0.f,std::min(m_mouse_position_tracker.start_point_x, intersection_point[0])),
                                        0,
                                        std::max(0.f,std::min(m_mouse_position_tracker.start_point_z, intersection_point[2])));

                    glm::vec3 rect_max( std::min((float)terrain.getWidth(), std::max(m_mouse_position_tracker.start_point_x, intersection_point[0])),
                                        0,
                                        std::min((float)terrain.getDepth(), std::max(m_mouse_position_tracker.start_point_z, intersection_point[2])));

                    terrain.addTerrainRect(rect_min, rect_max);
                    update();
                }
            }
        } break;
        default:
            break; // Do nothing
        }
    }
}

void GLWidget::wheelEvent(QWheelEvent * wheel)
{
    QGLWidget::wheelEvent(wheel);

    float delta = .0f;

    QPoint pix = wheel->pixelDelta();
    QPoint deg = wheel->angleDelta();

    if(!pix.isNull()) // screen resolution tracking, e.g., from magic mouse
        delta = (float) pix.y() / 360.f;
    else if(!deg.isNull()) // mouse wheel instead
        delta = deg.y() / 360.f;


    if(m_navigation_enabled)
    {
        m_view_manager.forward(-delta);
        update();
    }
    else
    {
        switch(m_active_mode)
        {
        case Mode::TerrainEdit: {
            int increment(delta * 10);
            m_scene_manager.getTerrain().incrementHeights(m_active_points, increment);
            m_scene_manager.refreshAccelerationStructureViewer();
            update();
        } break;
        case Mode::OrientationEdit:
            if(m_ctrl_pressed.load())
            {
                m_view_manager.forward(-delta);
            }
            else
            {
                m_scene_manager.getOrientationCompass().rotateNorth(delta*5);
                m_scene_manager.refreshNorthOrientation();
            }
            update();
            break;
        default: // Do nothing
            break;
        }
    }
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
    m_renderer.setOverlay(TerrainOverlayUniforms::OVERLAY_DISABLED);
    update();
}

void GLWidget::enableSlopeOverlay()
{
    m_renderer.setOverlay(TerrainOverlayUniforms::SLOPE_OVERLAY);
    update();
}

void GLWidget::enableAltitudeOverlay()
{
    m_renderer.setOverlay(TerrainOverlayUniforms::ALTITUDE_OVERLAY);
    update();
}

void GLWidget::keyPressEvent ( QKeyEvent * event )
{
    QGLWidget::keyPressEvent(event);

    if(event->key() == Qt::Key_R)
    {
        m_view_manager.reset_camera();
        update();
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
        if(m_control_style == FPS)
        {
            switch(event->key()){
            case Qt::Key_W:
                m_view_manager.forward(1);
                break;
            case Qt::Key_S:
                m_view_manager.forward(-1);
                break;
            case Qt::Key_A:
                m_view_manager.sideStep(1);
                break;
            case Qt::Key_D:
                m_view_manager.sideStep(-1);
                break;
            }
            update();
        }
        else if(m_control_style == Experimental1)
        {
            switch(event->key()){
            case Qt::Key_Up:
                m_view_manager.up(1);
                break;
            case Qt::Key_Down:
                m_view_manager.up(-1);
                break;
            case Qt::Key_Right:
                m_view_manager.sideStep(-1);
                break;
            case Qt::Key_Left:
                m_view_manager.sideStep(1);
                break;
            }
            update();
        }
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent * event)
{
    QGLWidget::keyReleaseEvent(event);
    if(event->key() == Qt::Key_Control)
    {
        m_ctrl_pressed.store(false);
        return;
    }
}

void GLWidget::loadTerrain(QString filename)
{
    m_scene_manager.loadTerrain(filename);
    update();
}

void GLWidget::renderGrid(bool enabled)
{
   m_render_options.m_render_grid = enabled;
   update();
}

void GLWidget::renderTerrain(bool enabled)
{
    m_render_options.m_render_terrain = enabled;
    update();
}

void GLWidget::renderAccelerationStructure(bool enabled)
{
    m_render_options.m_render_acceleration_structure = enabled;
    update();
}

void GLWidget::renderRays(bool enabled)
{
    m_rays.clear();
    m_render_options.m_render_rays = enabled;
    update();
}

void GLWidget::renderSun(bool enabled)
{
    m_render_options.m_render_sun = enabled;
    update();
}

void GLWidget::setMode(Mode mode)
{
    if(mode != m_active_mode)
    {
        m_scene_manager.getTerrain().clearTerrainElements();

        if(m_active_mode == Mode::TerrainEdit)
        {
            m_active_points.clear();
        }
        else if(m_active_mode == Mode::OrientationEdit)
        {
            m_scene_manager.refreshNorthOrientation();
            m_view_manager.popTransforms();
        }

        m_active_mode = mode;

        m_authorise_navigation_mode_switch = (m_active_mode != Mode::OrientationEdit);

        if(m_active_mode == Mode::OrientationEdit)
        {
            // Deactivate navigation
            setNavigationEnabled(false);
            m_view_manager.pushTransforms();
            m_view_manager.reset_camera();

            Terrain & terrain(m_scene_manager.getTerrain());
            m_view_manager.forward(terrain.getWidth()/2.0f, true);
            m_view_manager.sideStep(-terrain.getDepth()/2.0f, true);
            m_view_manager.up(std::max(terrain.getWidth(), terrain.getDepth()) * 5, true);
            m_view_manager.rotate(90, 0, true);
        }
        update();
    }
}

void GLWidget::setMonth(int month)
{
    m_scene_manager.getLightingManager().setMonth(month);
    update();
}

void GLWidget::setTime(int hour_of_day)
{
    m_scene_manager.getLightingManager().setTime(hour_of_day);
    update();
}

void GLWidget::setLatitude(int latitude)
{
    m_scene_manager.getLightingManager().setLatitude(latitude);
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

        m_view_manager.rotate(pitch, yaw);
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
