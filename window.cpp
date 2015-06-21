#include "window.h"
#include "glwidget.h"
#include "time_controller_dialog.h"
#include "pointer_info_dialog.h"
#include "settings.h"
#include "controllers.h"
#include "temp_edit_dlg.h"
#include <QtWidgets>
#include "actions.h"

/***********
 * DIALOGS *
 ***********/
Dialogs::Dialogs(QWidget *parent) :
    m_settings_dlg(new SettingsDialog(parent)), m_time_controller_dlg(new TimeControllerDialog(parent)),
    m_temp_editor_dlg(new TemperatureEditDialog(parent))
{

}

//---------------------------------------------------------------

MainWindow::MainWindow() :
    m_dialogs(this)
{
    init_actions();
    init_menu();

    m_glwidget = new GLWidget(TimeControllers(m_dialogs.m_time_controller_dlg->m_time_of_day_slider, m_dialogs.m_time_controller_dlg->m_month_of_year_slider),
                            ViewControllers(m_dialogs.m_settings_dlg->m_camera_sensitivity_slider, m_dialogs.m_settings_dlg->m_z_movement_sensitivity_slider,
                                            m_dialogs.m_settings_dlg->m_x_y_movement_sensitivity_slider),
                            TerrainControllers(m_dialogs.m_settings_dlg->m_terrain_scale_le, m_dialogs.m_settings_dlg->m_default_scale_cb),
                            m_dialogs.m_temp_editor_dlg,
                            m_render_actions,
                            m_overlay_actions,
                            m_control_actions,
                            m_show_actions,
                            this);

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
    delete m_base_actions;
    delete m_render_actions;
    delete m_control_actions;
    delete m_overlay_actions;
    delete m_show_actions;
    delete m_edit_actions;

    // Dialogs
    delete m_dialogs.m_settings_dlg;
    delete m_dialogs.m_time_controller_dlg;
}

void MainWindow::init_actions()
{
    m_base_actions = new BaseActions;
    m_render_actions = new RenderActions;
    m_control_actions = new ControlActions;
    m_overlay_actions = new OverlayActions;
    m_show_actions = new ShowActions;
    m_edit_actions = new EditActions;
}

void MainWindow::show_settings_dlg()
{
    m_dialogs.m_settings_dlg->exec();
}

void MainWindow::show_time_controller_dlg()
{
    if(!m_dialogs.m_time_controller_dlg->isVisible())
        m_dialogs.m_time_controller_dlg->show();
}

void MainWindow::show_temp_edit_dlg()
{
    m_dialogs.m_temp_editor_dlg->exec();
}

void MainWindow::init_menu()
{
    // BASE ACTIONS
    {
        (*m_base_actions)(BaseActions::_CLOSE_APP)->setShortcut(QKeySequence::Close);

        connect((*m_base_actions)(BaseActions::_CLOSE_APP), SIGNAL(triggered()), this, SLOT(close()));
        connect((*m_base_actions)(BaseActions::_LOAD_TERRAIN), SIGNAL(triggered()), this, SLOT(load_terrain_file()));
        connect((*m_base_actions)(BaseActions::_OPEN_SETTINGS), SIGNAL(triggered()), this, SLOT(show_settings_dlg()));

        m_file_menu = menuBar()->addMenu("File");
        m_file_menu->addAction((*m_base_actions)(BaseActions::_CLOSE_APP));
        m_file_menu->addAction((*m_base_actions)(BaseActions::_LOAD_TERRAIN));
        m_file_menu->addAction((*m_base_actions)(BaseActions::_OPEN_SETTINGS));
    }


    // RENDER MENU
    {
        m_render_menu = menuBar()->addMenu("Render");
        m_render_menu->addAction((*m_render_actions)(RenderActions::_GRID));
        m_render_menu->addAction((*m_render_actions)(RenderActions::_TERRAIN));
        m_render_menu->addAction((*m_render_actions)(RenderActions::_ACCELERATION_STRUCTURE));
        m_render_menu->addAction((*m_render_actions)(RenderActions::_RAYS));
        m_render_menu->addAction((*m_render_actions)(RenderActions::_SUN));
    }

    // CONTROLS MENU
    {
        m_controls_menu = menuBar()->addMenu("Controls");
        m_controls_menu->addAction((*m_control_actions)(ControlActions::_SOFTIMAGE));
        m_controls_menu->addAction((*m_control_actions)(ControlActions::_FPS));
    }

    // OVERLAY MENU
    {
;
        m_overlay_menu = menuBar()->addMenu("Overlay");
        m_overlay_menu->addAction((*m_overlay_actions)(OverlayActions::_NONE));
        m_overlay_menu->addAction((*m_overlay_actions)(OverlayActions::_ALTITUDE));
        m_overlay_menu->addAction((*m_overlay_actions)(OverlayActions::_SLOPE));
        m_overlay_menu->addAction((*m_overlay_actions)(OverlayActions::_SHADE));
        m_overlay_menu->addAction((*m_overlay_actions)(OverlayActions::_MIN_TEMP));
        m_overlay_menu->addAction((*m_overlay_actions)(OverlayActions::_MAX_TEMP));
        m_overlay_menu->addAction((*m_overlay_actions)(OverlayActions::_MIN_DAILY_ILLUMINATION));
        m_overlay_menu->addAction((*m_overlay_actions)(OverlayActions::_MAX_DAILY_ILLUMINATION));
    }

    // SHOW MENU
    {
        m_show_menu = menuBar()->addMenu("Show");
        m_show_menu->addAction((*m_show_actions)(ShowActions::_POINTER_INFO));
    }

    // EDIT MENU
    {
        m_edit_menu = menuBar()->addMenu("Edit");
        m_edit_menu->addAction((*m_edit_actions)(EditActions::_HUMIDITY));
        m_edit_menu->addAction((*m_edit_actions)(EditActions::_LATITUDE));
        m_edit_menu->addAction((*m_edit_actions)(EditActions::_ORIENTATION));
        m_edit_menu->addAction((*m_edit_actions)(EditActions::_TEMPERATURE));
        m_edit_menu->addAction((*m_edit_actions)(EditActions::_TIME));
    }
}

void MainWindow::load_terrain_file()
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
        m_glwidget->loadTerrain(fileName);
    }
}
