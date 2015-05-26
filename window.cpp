#include "window.h"
#include "glwidget.h"
#include "settings.h"
#include <QtWidgets>

Window::Window(const QGLFormat& format)
{
    SettingsEditorDialog settings_editor(this);
    m_glwidget = new GLWidget(settings_editor.getSettings(), format, this);

    init_dialogs();
    init_menu();
    refresh_control_style();
    setCentralWidget(m_glwidget); // Takes ownership of widget
    setWindowTitle("");
}

Window::~Window()
{
    // Menu elements
    delete m_file_menu;
    delete m_render_menu;
    delete m_controls_menu;
    delete m_overlay_menu;
    delete m_mode_menu;
    delete m_show_menu;

    // Misc actions
    delete m_action_close;
    delete m_action_open_settings_dialog;

    // Render actions
    delete m_action_load_terrain;
    delete m_action_render_grid;
    delete m_action_render_terrain;

    // Control style actions
    for(auto it (m_control_style_to_action_map.begin()); it != m_control_style_to_action_map.end(); it++)
        delete it->second;
    delete m_control_action_group;

    // Terrain options
    delete m_terrain_overlays_group;
    delete m_terrain_options_overlay_none;
    delete m_terrain_options_overlay_slope;
    delete m_terrain_options_overlay_altitude;

    // Mode actions
    for(auto it (m_mode_to_action_map.begin()); it != m_mode_to_action_map.end(); it++)
        delete it->second;
    delete m_mode_action_group;

    // Show actions
    delete m_show_time_controller_dialog;

    // Dialogs
    delete m_settings_editor_dlg;
    delete m_time_controller_dlg;
}

void Window::show_settings_dlg()
{
    m_settings_editor_dlg->exec();
    m_glwidget->updateSettings(m_settings_editor_dlg->getSettings());
}

void Window::show_time_controller_dlg()
{
    if(!m_time_controller_dlg->isVisible())
        m_time_controller_dlg->show();
}

void Window::init_dialogs()
{
    // Settings dialog
    m_settings_editor_dlg = new SettingsEditorDialog(this);

    // Time Controller
    m_time_controller_dlg = new TimeControllerDialog(this);
    connect(m_time_controller_dlg->m_time_of_day_slider, SIGNAL(sliderMoved(int)), m_glwidget, SLOT(setTime(int)));
    connect(m_time_controller_dlg->m_month_of_year_slider, SIGNAL(sliderMoved(int)), m_glwidget, SLOT(setMonth(int)));
}

void Window::init_menu()
{
    // FILE MENU
    {
        m_action_close = new QAction("Close", NULL);
        m_action_close->setShortcuts(QKeySequence::Close);

        m_action_load_terrain = new QAction("Load terrain", NULL);

        m_action_open_settings_dialog = new QAction("Open settings", NULL);

        connect(m_action_close, SIGNAL(triggered()), this, SLOT(close()));
        connect(m_action_load_terrain, SIGNAL(triggered()), this, SLOT(load_terrain_file()));
        connect(m_action_open_settings_dialog, SIGNAL(triggered()), this, SLOT(show_settings_dlg()));

        m_file_menu = menuBar()->addMenu("File");
        m_file_menu->addAction(m_action_close);
        m_file_menu->addAction(m_action_load_terrain);
        m_file_menu->addAction(m_action_open_settings_dialog);
    }


    // RENDER MENU
    {
        m_action_render_grid = new QAction("Grid", NULL);
        m_action_render_grid->setCheckable(true);
        m_action_render_grid->setChecked(true);
        connect(m_action_render_grid, SIGNAL(triggered()), this, SLOT(render_grid_toggled()));

        m_action_render_terrain = new QAction("Terrain", NULL);
        m_action_render_terrain->setCheckable(true);
        m_action_render_terrain->setChecked(true);
        connect(m_action_render_terrain, SIGNAL(triggered()), this, SLOT(render_terrain_toggled()));

        m_action_render_acceleration_structure = new QAction("Acceleration structure", NULL);
        m_action_render_acceleration_structure->setCheckable(true);
        m_action_render_acceleration_structure->setChecked(false);
        connect(m_action_render_acceleration_structure, SIGNAL(triggered()), this, SLOT(render_acceleration_structure_toggled()));

        m_action_render_rays = new QAction("Projected rays", NULL);
        m_action_render_rays->setCheckable(true);
        m_action_render_rays->setChecked(false);
        connect(m_action_render_rays, SIGNAL(triggered()), this, SLOT(render_rays_toggled()));

        m_render_menu = menuBar()->addMenu("Render");
        m_render_menu->addAction(m_action_render_grid);
        m_render_menu->addAction(m_action_render_terrain);
        m_render_menu->addAction(m_action_render_acceleration_structure);
        m_render_menu->addAction(m_action_render_rays);
    }

    // CONTROLS MENU
    {
        m_control_action_group = new QActionGroup( this );

        m_action_control_softimage = new QAction("SoftImage", NULL);
        m_action_control_softimage->setCheckable(true);
        m_action_control_softimage->setActionGroup(m_control_action_group);
        connect(m_action_control_softimage, SIGNAL(triggered()), this, SLOT(refresh_control_style()));
        m_control_style_to_action_map.insert(std::pair<ControlStyle,QAction*>(SoftImage, m_action_control_softimage));

        m_action_control_fps = new QAction("FPS", NULL);
        m_action_control_fps->setCheckable(true);
        m_action_control_fps->setActionGroup(m_control_action_group);
        connect(m_action_control_fps, SIGNAL(triggered()), this, SLOT(refresh_control_style()));
        m_control_style_to_action_map.insert(std::pair<ControlStyle,QAction*>(FPS, m_action_control_fps));

        m_action_control_experimental = new QAction("Experimental1", NULL);
        m_action_control_experimental->setCheckable(true);
        m_action_control_experimental->setActionGroup(m_control_action_group);
        connect(m_action_control_experimental, SIGNAL(triggered()), this, SLOT(refresh_control_style()));
        m_control_style_to_action_map.insert(std::pair<ControlStyle,QAction*>(Experimental1, m_action_control_experimental));

        m_action_control_softimage->setChecked(true);

        m_controls_menu = menuBar()->addMenu("Controls");
        m_controls_menu->addAction(m_action_control_softimage);
        m_controls_menu->addAction(m_action_control_fps);
        m_controls_menu->addAction(m_action_control_experimental);
    }

    // OVERLAY MENU
    {
        m_terrain_overlays_group = new QActionGroup( this );;

        m_terrain_options_overlay_none = new QAction("None", NULL);
        m_terrain_options_overlay_none->setCheckable(true);
        m_terrain_options_overlay_none->setActionGroup(m_terrain_overlays_group);
        connect(m_terrain_options_overlay_none, SIGNAL(triggered()), m_glwidget, SLOT(disableOverlays()));

        m_terrain_options_overlay_slope = new QAction("Slope", NULL);
        m_terrain_options_overlay_slope->setCheckable(true);
        m_terrain_options_overlay_slope->setActionGroup(m_terrain_overlays_group);
        connect(m_terrain_options_overlay_slope, SIGNAL(triggered()), m_glwidget, SLOT(enableSlopeOverlay()));

        m_terrain_options_overlay_altitude = new QAction("Altitude", NULL);
        m_terrain_options_overlay_altitude->setCheckable(true);
        m_terrain_options_overlay_altitude->setActionGroup(m_terrain_overlays_group);
        connect(m_terrain_options_overlay_altitude, SIGNAL(triggered()), m_glwidget, SLOT(enableAltitudeOverlay()));

        m_terrain_options_overlay_none->setChecked(true);

        m_overlay_menu = menuBar()->addMenu("Overlay");
        m_overlay_menu->addAction(m_terrain_options_overlay_none);
        m_overlay_menu->addAction(m_terrain_options_overlay_slope);
        m_overlay_menu->addAction(m_terrain_options_overlay_altitude);
    }


    // MODE MENU
    {
        m_mode_action_group = new QActionGroup( this );

        m_mode_none = new QAction("None", NULL);
        m_mode_none->setCheckable(true);
        m_mode_none->setActionGroup(m_mode_action_group);
        connect(m_mode_none, SIGNAL(triggered()), this, SLOT(refresh_mode()));
        m_mode_to_action_map.insert(std::pair<Mode,QAction*>(Mode::None, m_mode_none));

        m_mode_terrain_edit = new QAction("Terrain Edit", NULL);
        m_mode_terrain_edit->setCheckable(true);
        m_mode_terrain_edit->setActionGroup(m_mode_action_group);
        connect(m_mode_terrain_edit, SIGNAL(triggered()), this, SLOT(refresh_mode()));
        m_mode_to_action_map.insert(std::pair<Mode,QAction*>(Mode::TerrainEdit, m_mode_terrain_edit));

        m_mode_selection = new QAction("Selection", NULL);
        m_mode_selection->setCheckable(true);
        m_mode_selection->setActionGroup(m_mode_action_group);
        connect(m_mode_selection, SIGNAL(triggered()), this, SLOT(refresh_mode()));
        m_mode_to_action_map.insert(std::pair<Mode,QAction*>(Mode::Selection, m_mode_selection));

        m_mode_terrain_orientation_edit = new QAction("Terrain orientation", NULL);
        m_mode_terrain_orientation_edit->setCheckable(true);
        m_mode_terrain_orientation_edit->setActionGroup(m_mode_action_group);
        connect(m_mode_terrain_orientation_edit, SIGNAL(triggered()), this, SLOT(refresh_mode()));
        m_mode_to_action_map.insert(std::pair<Mode,QAction*>(Mode::OrientationEdit, m_mode_terrain_orientation_edit));

        m_mode_none->setChecked(true);

        m_mode_menu = menuBar()->addMenu("Mode");
        m_mode_menu->addAction(m_mode_none);
        m_mode_menu->addAction(m_mode_terrain_edit);
        m_mode_menu->addAction(m_mode_selection);
        m_mode_menu->addAction(m_mode_terrain_orientation_edit);
    }

    // SHOW MENU
    {
        m_show_time_controller_dialog = new QAction("Show time controller", NULL);
        connect(m_show_time_controller_dialog, SIGNAL(triggered()), this, SLOT(show_time_controller_dlg()));

        m_show_menu = menuBar()->addMenu("Show");
        m_show_menu->addAction(m_show_time_controller_dialog);
    }
}

void Window::render_grid_toggled()
{
    m_glwidget->renderGrid(m_action_render_grid->isChecked());
}

void Window::render_terrain_toggled()
{
    m_glwidget->renderTerrain(m_action_render_terrain->isChecked());
}

void Window::render_acceleration_structure_toggled()
{
    m_glwidget->renderAccelerationStructure(m_action_render_acceleration_structure->isChecked());
}

void Window::render_rays_toggled()
{
    m_glwidget->renderRays(m_action_render_rays->isChecked());
}

void Window::load_terrain_file()
{
    QFileDialog::Options options;
    QString selectedFilter;

    QString fileName ( QFileDialog::getOpenFileName(this,
                                                    tr("Open Terrain File"),
                                                    "/home/harry/ter-files/",
                                                    tr("Terragen Files (*.ter)"),
                                                    &selectedFilter,
                                                    options));
    if (!fileName.isEmpty())
    {
        m_glwidget->loadTerrain(fileName);
    }
}

void Window::refresh_control_style()
{
    for(auto it (m_control_style_to_action_map.begin()); it != m_control_style_to_action_map.end(); it++)
    {
        if(it->second->isChecked())
            m_glwidget->setControlStyle(it->first);
    }
}

void Window::refresh_mode()
{
    for(auto it (m_mode_to_action_map.begin()); it != m_mode_to_action_map.end(); it++)
    {
        if(it->second->isChecked())
            m_glwidget->setMode(it->first);
    }
}
