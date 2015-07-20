#include "window.h"
#include <QtWidgets>
#include "actions.h"

MainWindow::MainWindow() : m_actions(new AllActions)
{
    establish_connections();
    init_menu();

    m_glwidget = new GLWidget(m_actions, this);

    setCentralWidget(m_glwidget); // Takes ownership of widget
    setWindowTitle("");
}

MainWindow::~MainWindow()
{
    // Menu elements
    delete m_file_menu;
    delete m_render_menu;
    delete m_controls_menu;
    delete m_overlay_menu;
    delete m_show_menu;
    delete m_edit_menu;

    // Actions
    delete m_actions;
}

void MainWindow::init_menu()
{
    // BASE ACTIONS
    {
        m_file_menu = menuBar()->addMenu("File");

        m_file_menu->addAction(m_actions->m_base_actions[BaseActionFamily::_LOAD_TERRAIN]);
        m_file_menu->addAction(m_actions->m_base_actions[BaseActionFamily::_OPEN_SETTINGS]);
        m_file_menu->addAction(m_actions->m_base_actions[BaseActionFamily::_CLOSE_APP]);
    }


    // RENDER MENU
    {
        m_render_menu = menuBar()->addMenu("Render");

        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_GRID]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_TERRAIN]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_RAYS]);
        m_render_menu->addAction(m_actions->m_render_actions[RenderActionFamily::_SUN]);
    }

    // CONTROLS MENU
    {
        m_controls_menu = menuBar()->addMenu("Controls");

        m_controls_menu->addAction(m_actions->m_control_actions[ControlActionFamily::_SOFTIMAGE]);
        m_controls_menu->addAction(m_actions->m_control_actions[ControlActionFamily::_FPS]);
    }

    // OVERLAY MENU
    {
        m_overlay_menu = menuBar()->addMenu("Overlay");
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_NONE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_SLOPE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_ALTITUDE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_SHADE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_TEMPERATURE]);
        m_overlay_menu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_SOIL_INFILTRATION_RATE]);

        m_illumination_submenu = m_overlay_menu->addMenu("Illumination");
        m_illumination_submenu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_MIN_DAILY_ILLUMINATION]);
        m_illumination_submenu->addAction(m_actions->m_overlay_actions[OverlayActionFamily::_MAX_DAILY_ILLUMINATION]);
    }

    // SHOW MENU
    {
        m_show_menu = menuBar()->addMenu("Show");

        m_show_menu->addAction(m_actions->m_show_actions[ShowActionFamily::_POINTER_INFO]);
    }

    // EDIT MENU
    {
        m_edit_menu = menuBar()->addMenu("Edit");

        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_TEMPERATURE]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_ORIENTATION]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_HUMIDITY]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_TIME]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_LATITUDE]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_MONTHLY_RAINFALL]);
        m_edit_menu->addAction(m_actions->m_edit_actions[EditActionFamily::_SOIL_INFILTRATION_RATE]);
    }
}

void MainWindow::establish_connections()
{
    connect(m_actions->m_base_actions[BaseActionFamily::_CLOSE_APP], SIGNAL(triggered()), this, SLOT(close()));
}
