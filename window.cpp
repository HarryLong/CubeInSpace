#include "window.h"
#include <QtWidgets>
#include "actions.h"

MainWindow::MainWindow() : m_actions(new AllActions)
{
    establish_connections();
    init_menu_elements();

    setEditMenu();
    CentralWidget * cw  = new CentralWidget(m_actions, this);

    setCentralWidget(cw); // Takes ownership of widget
    setWindowTitle("");
}

MainWindow::~MainWindow()
{
    // Menu elements
    delete m_file_menu;
    delete m_render_menu;
    delete m_controls_menu;
    delete m_overlay_menu;
    delete m_edit_show_menu;
    delete m_edit_menu;

    // Actions
    delete m_actions;
}

void MainWindow::init_menu_elements()
{
    // BASE ACTIONS
    {
        m_file_menu = new QMenu("File");

        m_file_menu->addAction(m_actions->m_base_actions[BaseActionFamily::_LOAD_TERRAIN]);
        m_file_menu->addAction(m_actions->m_base_actions[BaseActionFamily::_OPEN_SETTINGS]);
        m_file_menu->addAction(m_actions->m_base_actions[BaseActionFamily::_CLOSE_APP]);
    }

    // MODE MENU
    {
        m_mode_menu = new QMenu("Mode");

        m_mode_menu->addAction(m_actions->m_mode_actions[ModeActionFamily::_RESOURCE_EDIT]);
        m_mode_menu->addAction(m_actions->m_mode_actions[ModeActionFamily::_CLUSTERING]);
        m_mode_menu->addAction(m_actions->m_mode_actions[ModeActionFamily::_PLANT_EDIT]);
    }

    // RENDER MENU
    {
        m_render_menu = new QMenu("Render");

        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_GRID]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_TERRAIN]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_RAYS]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_SUN]);
    }

    // CONTROLS MENU
    {
        m_controls_menu = new QMenu("Controls");

        m_controls_menu->addAction(m_actions->m_control_actions[ControlActionFamily::_SOFTIMAGE]);
        m_controls_menu->addAction(m_actions->m_control_actions[ControlActionFamily::_FPS]);
    }

    /**********************
     * EDIT MENU ELEMENTS *
     **********************/
    // OVERLAY MENU
    {
        m_overlay_menu = new QMenu("Overlay");
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_NONE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_SLOPE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_ALTITUDE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_SHADE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_TEMPERATURE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_SOIL_INFILTRATION_RATE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_ILLUMINATION]);

        m_soil_humidity_submenu = new QMenu("Soil humidity");
        m_soil_humidity_submenu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_MONTHLY_SOIL_HUMIDITY]);
        m_soil_humidity_submenu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_WEIGHTED_AVG_SOIL_HUMIDITY]);
        m_overlay_menu->addMenu(m_soil_humidity_submenu);
    }

    // EDIT SHOW MENU
    {
        m_edit_show_menu = new QMenu("Show");
        m_edit_show_menu->addAction(m_actions->m_show_actions[ShowActionFamily::_POINTER_INFO]);
    }

    // EDIT MENU
    {
        m_edit_menu = new QMenu("Edit");

        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_TEMPERATURE]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_ORIENTATION]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_TIME]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_LATITUDE]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_MONTHLY_RAINFALL]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_SOIL_INFILTRATION_RATE]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_FLOOD_FILL]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_ABSOLUTE_AGGREGATE_HEIGHT]);
    }

    /****************************
     * CLUSTERING MENU ELEMENTS *
     ****************************/
    // CLUSTERING SHOW MENU
    {
        m_clustering_show_menu = new QMenu("Show");
        m_clustering_show_menu->addAction(m_actions->m_show_actions[ShowActionFamily::_CLUSTERS_INFO]);
    }

    /************************
     * PLANTS MENU ELEMENTS *
     ************************/
    // PLANTS SHOW MENU
    {
        m_plants_show_menu = new QMenu("Show");
        m_plants_show_menu->addAction(m_actions->m_show_actions[ShowActionFamily::_CLUSTERS_INFO]);
        m_plants_show_menu->addAction(m_actions->m_show_actions[ShowActionFamily::_PLANT_SELECTION]);
    }
}

void MainWindow::setEditMenu()
{
    menuBar()->clear();

    menuBar()->addMenu(m_file_menu);
    menuBar()->addMenu(m_mode_menu);
    menuBar()->addMenu(m_render_menu);
    menuBar()->addMenu(m_controls_menu);
    menuBar()->addMenu(m_overlay_menu);
    menuBar()->addMenu(m_edit_show_menu);
    menuBar()->addMenu(m_edit_menu);
}

void MainWindow::setClusteringMenu()
{
    menuBar()->clear();

    menuBar()->addMenu(m_file_menu);
    menuBar()->addMenu(m_mode_menu);
    menuBar()->addMenu(m_render_menu);
    menuBar()->addMenu(m_controls_menu);
    menuBar()->addMenu(m_clustering_show_menu);
}

void MainWindow::setPlantsMenu()
{
    menuBar()->clear();

    menuBar()->addMenu(m_file_menu);
    menuBar()->addMenu(m_mode_menu);
    menuBar()->addMenu(m_render_menu);
    menuBar()->addMenu(m_controls_menu);
    menuBar()->addMenu(m_plants_show_menu);
}

void MainWindow::establish_connections()
{
    connect(m_actions->m_base_actions[BaseActionFamily::_CLOSE_APP], SIGNAL(triggered()), this, SLOT(close()));

    connect(m_actions->m_mode_actions[ModeActionFamily::_RESOURCE_EDIT], SIGNAL(triggered()), this, SLOT(setEditMenu()));
    connect(m_actions->m_mode_actions[ModeActionFamily::_CLUSTERING], SIGNAL(triggered()), this, SLOT(setClusteringMenu()));
    connect(m_actions->m_mode_actions[ModeActionFamily::_PLANT_EDIT], SIGNAL(triggered()), this, SLOT(setPlantsMenu()));
}
