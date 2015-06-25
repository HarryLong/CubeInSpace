#include "glwidget.h"

#include <QOpenGLContext>
#include <iostream>
#include <QMouseEvent>
#include "constants.h"
#include <math.h>
#include "geom.h"
#include "glm_rotations.h"
#include "pointer_info_dialog.h"
#include "controllers.h"
#include "settings.h"
#include "actions.h"
#include "glheader.h"
#include "shader_programs.h"

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

/*******************
 * SHADER PROGRAMS *
 *******************/
ShaderPrograms::ShaderPrograms(QObject * parent) :
    m_base(new BaseShader(parent)), m_terrain(new TerrainShader(parent)),
    m_terrain_elements(new TerrainElementsShader(parent)),
    m_normals_generator(new NormalsGeneratorShader(parent))
{

}

ShaderPrograms::~ShaderPrograms()
{
    delete m_base;
    delete m_terrain;
    delete m_terrain_elements;
    delete m_normals_generator;
}


//----------------------------------------------------------------------------------------------


GLWidget::GLWidget(ViewControllers view_controllers, TerrainControllers terrain_controllers, TemperatureEditDialog * temp_edit_dlg,
                   Actions * render_actions, Actions* overlay_actions, Actions * control_actions, Actions * show_actions,
                   Actions * edit_actions, QWidget * parent) :
  QOpenGLWidget(parent),
  m_progress_bar_widget(new ProgressBarWidget(this)),
  m_pointer_info_dlg(new PointerInformationDialog(this)),
  m_render_actions(render_actions), m_control_actions(control_actions), m_show_actions(show_actions),
  m_edit_actions(edit_actions),
  m_overlay_widgets((*m_edit_actions)(EditActions::_LATITUDE), (*m_edit_actions)(EditActions::_TIME_OF_DAY), (*m_edit_actions)(EditActions::_MONTH_OF_YEAR), this),
  m_scene_manager(new SceneManager(PositionControllers(m_overlay_widgets(ControllerWidgetsWrapper::Type::_LATITUDE)->getSlider()),
                                   TimeControllers(m_overlay_widgets(ControllerWidgetsWrapper::Type::_TIME)->getSlider(),
                                                   m_overlay_widgets(ControllerWidgetsWrapper::Type::_MONTH)->getSlider()),
                                   terrain_controllers, temp_edit_dlg, overlay_actions)),
  m_navigation_enabled(false), m_mouse_tracking_thread(NULL), m_authorise_navigation_mode_switch(true),
  m_view_manager(view_controllers),
  m_orientation_widget(m_view_manager.getCameraPitch(), m_view_manager.getCameraYaw(), (*m_edit_actions)(EditActions::_ORIENTATION), this)
{
    m_mouse_tracking_thread_run.store(true);
    m_ctrl_pressed.store(false);
    setFocusPolicy(Qt::ClickFocus);
    establish_connections();
    setNavigationEnabled(true);
    display_info_pointer_dlg(false);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    enable_continuous_mouse_tracking(false);
    delete m_scene_manager;
    delete m_progress_bar_widget;
    delete m_mouse_tracking_thread;
    delete m_pointer_info_dlg;
}

void GLWidget::establish_connections()
{
    // Refresh render upon request from the scene manager
    connect(m_scene_manager, SIGNAL(refreshRender()), this, SLOT(update()));

    connect(&m_view_manager, SIGNAL(cameraOrientationChanged(float,float)), &m_orientation_widget, SLOT(setCameraOrientation(float,float)));

    // When the control style changes
    connect(m_control_actions->getActionGroup(), SIGNAL(triggered(QAction*)), this, SLOT(control_changed()));

    // Show pointer info dialog
    connect((*m_show_actions)(ShowActions::_POINTER_INFO), SIGNAL(toggled(bool)), this, SLOT(display_info_pointer_dlg(bool)));

    // Progress dialog updates
    connect(m_scene_manager, SIGNAL(processing(QString)), m_progress_bar_widget, SLOT(processing(QString)));
    connect(m_scene_manager, SIGNAL(processingUpdate(int)), m_progress_bar_widget, SLOT(updateProgress(int)));
    connect(m_scene_manager, SIGNAL(processingComplete()), m_progress_bar_widget, SLOT(finishedProcessing()));

    // When a new render element is selected, we must rerender
    connect(m_render_actions->getActionGroup(), SIGNAL(triggered(QAction*)), this, SLOT(update()));

    // Clear rendered rays when action is toggles
    connect((*m_render_actions)(RenderActions::_RAYS), SIGNAL(triggered(bool)), this, SLOT(clear_rays()));

    connect(&m_orientation_widget, SIGNAL(northOrientationChanged(float,float,float)), m_scene_manager, SLOT(setNorthOrientation(float,float,float)));
}

void GLWidget::control_changed()
{
    if((*m_control_actions)(ControlActions::_FPS)->isChecked())
    {
        show_cursor(false);
        reset_fps_cursor();
    }
    else if((*m_control_actions)(ControlActions::_SOFTIMAGE)->isChecked())
    {
        show_cursor(true);
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
    setMouseTracking(true);

    m_shaders = new ShaderPrograms(this);
    emit GLReady(); // Other things need to be setup...
    m_scene_manager->initScene();
}

void GLWidget::paintGL() // Override
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<Asset*> assets_to_render;
    Terrain * terrain (m_scene_manager->getTerrain());

    // Draw the grid
    if(render_grid())
    {
        std::vector<Asset*> grid_assets (m_scene_manager->getGrid().getAssets());
        assets_to_render.insert(assets_to_render.end(), grid_assets.begin(), grid_assets.end());
    }

    // Draw the terrain
    if(render_terrain())
    {
        // Check if normals need recalculating
        if(!terrain->normalsValid())
        {
//            setFormat(terrain->getNormals().getOffscreenSurface());
            context()->swapBuffers(terrain->getNormals()->getOffscreenSurface());
            m_renderer.calculateNormals(m_shaders->m_normals_generator, terrain);
            makeCurrent();
        }

        const LightProperties & sunlight_properties (m_scene_manager->getLightingManager().getSunlightProperties());
        m_renderer.renderTerrain(m_shaders->m_terrain, m_view_manager, terrain, sunlight_properties);

        // Terrain elements
        std::vector<Asset*> terrain_elements_to_render;
        terrain_elements_to_render.push_back(terrain->getSelectionRect());
        m_renderer.renderTerrainElements(m_shaders->m_terrain_elements, m_view_manager, terrain_elements_to_render, terrain) ;
    }

    // Draw the acceleration structure
    if(render_acceleration_structure())
    {
        assets_to_render.push_back(m_scene_manager->getAccelerationStructure());
    }

    // Draw the rays
    if(render_rays())
        assets_to_render.push_back(&m_rays);

    // Draw the sun
    if(render_sun())
        assets_to_render.push_back(m_scene_manager->getSun());

    assets_to_render.push_back(m_water_flow_analyzer.asAsset());

    m_renderer.renderAssets(m_shaders->m_base, m_view_manager, assets_to_render);
}

void GLWidget::resizeGL(int w, int h) // Override
{
    QOpenGLWidget::resizeGL(w ,h);
    m_width.store(width());
    m_height.store(height());
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
    glm::vec3 start(m_view_manager.toWorld(glm::vec3(screen_x, screen_y, 0.0f), viewport, m_width.load(), m_height.load()));
    glm::vec3 end(m_view_manager.toWorld(glm::vec3(screen_x, screen_y, 1.f), viewport, m_width.load(), m_height.load()));
    glm::vec3 direction(glm::normalize(Geom::diff(end,start)));
    return m_scene_manager->getTerrain()->traceRay(start, direction, intersection_point);
}

bool GLWidget::get_intersection_point_with_base_plane(int screen_x, int screen_y, glm::vec3 & intersection_point)
{
    int w(width());
    int h(height());

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::vec3 start(m_view_manager.toWorld(glm::vec3(screen_x, screen_y, .0f), viewport, m_width.load(), m_height.load()));
    glm::vec3 end(m_view_manager.toWorld(glm::vec3(screen_x, screen_y, 1.f), viewport, m_width.load(), m_height.load()));
    glm::vec3 direction(glm::normalize(Geom::diff(end,start)));

    return Geom::rayPlaneIntersection(m_scene_manager->getTerrain()->getBaseHeight(true), start, direction, intersection_point);
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
            glm::vec3 start(m_view_manager.toWorld(glm::vec3(event->x(), event->y(), .0f), viewport, m_width.load(), m_height.load()));
            glm::vec3 end(m_view_manager.toWorld(glm::vec3(event->x(), event->y(), 1.f), viewport, m_width.load(), m_height.load()));
            m_rays.add(start, end) ;
            update();
        }

        if(intersection)
        {
            if(m_ctrl_pressed.load()) // Waterflow
            {
                std::cout << "WATERFLOW!" << std::endl;
                Terrain * terrain(m_scene_manager->getTerrain());
                float scale(terrain->getScale());
//                std::cout << "Scale: " << scale << std::emdl;
                m_water_flow_analyzer.generateWaterFlow(terrain->getHeightMap(),
                                                        glm::vec2(intersection_point[0]/scale,intersection_point[2]/scale));
                update();
            }
            else // Selection
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

                float end_x, end_y, end_z;
                m_mouse_position_tracker.getEndPosition(end_x, end_y, end_z);
                m_mouse_position_tracker.setStartPosition(end_x, end_y, end_z);

                update();
            }
        }
    }
    else // navigation disabled
    {
        if(event->buttons() == Qt::LeftButton)
        {
            Terrain * terrain (m_scene_manager->getTerrain());
            glm::vec3 intersection_point;
            if(get_intersection_point_with_terrain(event->x(), event->y(), intersection_point) ||
                get_intersection_point_with_base_plane(event->x(), event->y(), intersection_point))
            {
                float start_point_x, start_point_y, start_point_z;
                m_terrain_position_tracker.getStartPosition(start_point_x, start_point_y, start_point_z);

                glm::vec3 rect_min( std::max(0.f,std::min(start_point_x, intersection_point[0])),
                        0,
                        std::max(0.f,std::min(start_point_z, intersection_point[2])));

                glm::vec3 rect_max( std::min((float)terrain->getWidth(true)-1, std::max(start_point_x, intersection_point[0])),
                        0,
                        std::min((float)terrain->getDepth(true)-1, std::max(start_point_z, intersection_point[2])));

                terrain->setSelectionRectangle(rect_min, rect_max);
                update();
            }
        }

        if(m_pointer_info_dlg->isVisible())
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
        m_view_manager.forward(-delta);
        update();
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
        if(fps())
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
            default:
                break;
            }
            update();
        }
    }
    if(m_orientation_widget.editMode())
    {
        switch(event->key()){
        case Qt::Key_Left:
            m_orientation_widget.rotateNorth(m_ctrl_pressed.load() ? 10 : 1);
            break;
        case Qt::Key_Right:
            m_orientation_widget.rotateNorth(m_ctrl_pressed.load() ? -10 : -1);
            break;
        default:
            break;
        }
        update();
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

void GLWidget::loadTerrain(QString filename)
{
    makeCurrent();
    m_scene_manager->loadTerrain(filename);
    update();
}

void GLWidget::set_center_camera_position()
{
    int width, depth, base_height, max_height;
    m_scene_manager->getTerrain()->getTerrainDimensions(width, depth, base_height, max_height);

    m_view_manager.reset_camera();
    m_view_manager.forward(width/2.0f, true);
    m_view_manager.sideStep(-depth/2.0f, true);
    m_view_manager.up(max_height + 1000, true);
    m_view_manager.rotate(90, 0, true);
}

#define PROGRESS_BAR_HEIGHT 100
#define ORIENTATION_WIDGET_HEIGHT 25
void GLWidget::resizeEvent(QResizeEvent *event)
{
    QOpenGLWidget::resizeEvent(event);
    QSize new_size(event->size());
    int h(new_size.height());
    int w(new_size.width());

    m_progress_bar_widget->setFixedSize(w, PROGRESS_BAR_HEIGHT);
    m_progress_bar_widget->move(0, h-PROGRESS_BAR_HEIGHT);

    m_orientation_widget.setFixedSize(w/2.0f, ORIENTATION_WIDGET_HEIGHT);
    m_orientation_widget.move(w/4.0f, 0);

    m_overlay_widgets.resize(w,h);
}

void GLWidget::update_info_pointer_dlg(const glm::vec2 &screen_pos)
{
    Terrain * terrain (m_scene_manager->getTerrain());
    glm::vec3 intersection_point;
    if(get_intersection_point_with_terrain(screen_pos[0], screen_pos[1], intersection_point))
    {
        // Altitude
        float altitude(terrain->getAltitude(intersection_point));

        // Slope
        float slope(terrain->getSlope(intersection_point));

        // Shade
        bool shaded;
        bool shade_data_valid(terrain->isShaded(intersection_point, shaded));

        // Temperature
        float min_temp, max_temp;
        bool temp_data_valid(terrain->getTemperatures(intersection_point, min_temp, max_temp));

        // Daily illumination
        int min_daily_illumination, max_daily_illumination;
        bool daily_illumination_valid(terrain->getDailyIlluminations(intersection_point, min_daily_illumination, max_daily_illumination));

        m_pointer_info_dlg->update(altitude, slope,
                                   shade_data_valid, shaded,
                                   temp_data_valid, min_temp, max_temp,
                                   daily_illumination_valid, min_daily_illumination, max_daily_illumination);
    }
    else
        m_pointer_info_dlg->invalidPoint();
}

void GLWidget::display_info_pointer_dlg(bool display)
{
    if(display && !m_pointer_info_dlg->isVisible())
        m_pointer_info_dlg->show();
    else if(!display && m_pointer_info_dlg->isVisible())
        m_pointer_info_dlg->hide();
}

void GLWidget::clear_rays()
{
    m_rays.clear();
}

bool GLWidget::render_rays()
{
    return (*m_render_actions)(RenderActions::_RAYS)->isChecked();
}

bool GLWidget::render_grid()
{
    return (*m_render_actions)(RenderActions::_GRID)->isChecked();
}

bool GLWidget::render_sun()
{
    return (*m_render_actions)(RenderActions::_SUN)->isChecked();
}

bool GLWidget::render_terrain()
{
    return (*m_render_actions)(RenderActions::_TERRAIN)->isChecked();
}

bool GLWidget::render_acceleration_structure()
{
    return (*m_render_actions)(RenderActions::_ACCELERATION_STRUCTURE)->isChecked();
}

bool GLWidget::fps()
{
    return (*m_control_actions)(ControlActions::_FPS)->isChecked();
}

bool GLWidget::softimage()
{
    return (*m_control_actions)(ControlActions::_SOFTIMAGE)->isChecked();
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
