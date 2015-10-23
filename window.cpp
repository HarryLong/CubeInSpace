#include "window.h"
#include <QtWidgets>
#include "actions.h"

MainWindow::MainWindow() : m_actions(new AllActions)
{
    m_cw = new CentralWidget(m_actions, this);
    establish_connections();
    init_menu_elements();


    setCentralWidget(m_cw); // Takes ownership of widget
    setWindowTitle("");
}

MainWindow::~MainWindow()
{
    // Menu elements
    delete m_file_menu;
    delete m_render_menu;
    delete m_overlay_menu;
    delete m_edit_show_menu;
    delete m_edit_menu;

    // Actions
    delete m_actions;
}

void MainWindow::enable_menus()
{
    menuBar()->addMenu(m_file_menu);
    menuBar()->addMenu(m_render_menu);
    menuBar()->addMenu(m_overlay_menu);
    menuBar()->addMenu(m_edit_show_menu);
    menuBar()->addMenu(m_edit_menu);
}

void MainWindow::disable_menus()
{
    menuBar()->clear();
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

    // RENDER MENU
    {
        m_render_menu = new QMenu("Render");

        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_GRID]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_TERRAIN]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_RAYS]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_SUN]);
    }

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
    enable_menus();
}

void MainWindow::establish_connections()
{
    connect(m_actions->m_base_actions[BaseActionFamily::_CLOSE_APP], SIGNAL(triggered()), this, SLOT(close()));
    connect(m_cw, SIGNAL(disable_menus()), this, SLOT(disable_menus()));
    connect(m_cw, SIGNAL(enable_menus()), this, SLOT(enable_menus()));
}
