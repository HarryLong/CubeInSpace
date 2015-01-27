#include "window.h"
#include "glwidget.h"
#include "settings.h"
#include <QtWidgets>

Window::Window(const QGLFormat& format)
{
    SettingsEditor settings_editor(this);
    m_glwidget = new GLWidget(settings_editor.getSettings(), format, this);

    init_menu();
    refresh_control_style();
    setCentralWidget(m_glwidget); // Takes ownership of widget
    setWindowTitle("CUBES IN SPACE");
}

Window::~Window()
{
    delete m_file_menu;
    delete m_action_close;
    delete m_action_open_settings_dialog;
    delete m_action_load_terrain;
    delete m_action_render_grid;
    delete m_action_render_assets;
    delete m_action_render_terrain;

    for(auto it (m_control_style_to_action_map.begin()); it != m_control_style_to_action_map.end(); it++)
        delete it->second;
    delete m_control_action_group;

    // Terrain options
    delete m_terrain_overlays_group;
    delete m_terrain_options_overlay_none;
    delete m_terrain_options_overlay_slope;
    delete m_terrain_options_overlay_altitude;
}

void Window::open_settings_dialog()
{
    SettingsEditor settings_editor(this);
    settings_editor.setModal(true);

    settings_editor.exec();
    m_glwidget->updateSettings(settings_editor.getSettings());
}

void Window::init_menu()
{
    m_action_close = new QAction("Close", NULL);
    m_action_close->setShortcuts(QKeySequence::Close);

    m_action_load_terrain = new QAction("Load terrain", NULL);

    m_action_open_settings_dialog = new QAction("Open settings", NULL);

    connect(m_action_close, SIGNAL(triggered()), this, SLOT(close()));
    connect(m_action_load_terrain, SIGNAL(triggered()), this, SLOT(load_terrain_file()));
    connect(m_action_open_settings_dialog, SIGNAL(triggered()), this, SLOT(open_settings_dialog()));

    m_file_menu = menuBar()->addMenu("File");
    m_file_menu->addAction(m_action_close);
    m_file_menu->addAction(m_action_load_terrain);
    m_file_menu->addAction(m_action_open_settings_dialog);

    // RENDERING MENU
    m_action_render_grid = new QAction("Grid", NULL);
    m_action_render_grid->setCheckable(true);
    m_action_render_grid->setChecked(true);
    connect(m_action_render_grid, SIGNAL(triggered()), this, SLOT(render_grid_toggled()));

    m_action_render_assets = new QAction("Assets", NULL);
    m_action_render_assets->setCheckable(true);
    m_action_render_assets->setChecked(true);
    connect(m_action_render_assets, SIGNAL(triggered()), this, SLOT(render_assets_toggled()));

    m_action_render_terrain = new QAction("Terrain", NULL);
    m_action_render_terrain->setCheckable(true);
    m_action_render_terrain->setChecked(true);
    connect(m_action_render_terrain, SIGNAL(triggered()), this, SLOT(render_terrain_toggled()));

    m_file_menu = menuBar()->addMenu("Render");
    m_file_menu->addAction(m_action_render_grid);
    m_file_menu->addAction(m_action_render_assets);
    m_file_menu->addAction(m_action_render_terrain);

    // CONTROLS MENU
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

    m_file_menu = menuBar()->addMenu("Controls");
    m_file_menu->addAction(m_action_control_softimage);
    m_file_menu->addAction(m_action_control_fps);
    m_file_menu->addAction(m_action_control_experimental);

    // Terrain options
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

    m_file_menu = menuBar()->addMenu("Overlay");
    m_file_menu->addAction(m_terrain_options_overlay_none);
    m_file_menu->addAction(m_terrain_options_overlay_slope);
    m_file_menu->addAction(m_terrain_options_overlay_altitude);
}

void Window::render_grid_toggled()
{
    m_glwidget->render_grid(m_action_render_grid->isChecked());
}

void Window::render_assets_toggled()
{
    m_glwidget->render_assets(m_action_render_assets->isChecked());
}

void Window::render_terrain_toggled()
{
    m_glwidget->render_terrain(m_action_render_terrain->isChecked());
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
