#include "window.h"
#include "glwidget.h"
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
    m_settings_dlg(new SettingsDialog(parent)), m_temp_editor_dlg(new TemperatureEditDialog(parent))
{

}

//---------------------------------------------------------------

MainWindow::MainWindow() :
    m_dialogs(this)
{
    init_actions();
    establish_connections();
    init_menu();

    m_glwidget = new GLWidget(ViewControllers(m_dialogs.m_settings_dlg->m_translation_sensitivity_slider, m_dialogs.m_settings_dlg->m_rotation_sensitivity_slider),
                              TerrainControllers(m_dialogs.m_settings_dlg->m_terrain_scale_le, m_dialogs.m_settings_dlg->m_default_scale_cb),
                              m_dialogs.m_temp_editor_dlg,
                              m_render_actions,
                              m_overlay_actions,
                              m_control_actions,
                              m_show_actions,
                              m_edit_actions,
                              m_tmp_actions,
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
    delete m_tmp_menu;

    // Actions
    delete m_base_actions;
    delete m_render_actions;
    delete m_control_actions;
    delete m_overlay_actions;
    delete m_show_actions;
    delete m_edit_actions;
    delete m_tmp_actions;

    // Dialogs
    delete m_dialogs.m_settings_dlg;
}

void MainWindow::init_actions()
{
    m_base_actions = new BaseActions;
    m_render_actions = new RenderActions;
    m_control_actions = new ControlActions;
    m_overlay_actions = new OverlayActions;
    m_show_actions = new ShowActions;
    m_edit_actions = new EditActions;
    m_tmp_actions = new TmpActions;
}

void MainWindow::show_settings_dlg()
{
    m_dialogs.m_settings_dlg->exec();
}

void MainWindow::trigger_temp_edit_dlg(bool checked)
{
    if(checked)
        m_dialogs.m_temp_editor_dlg->exec();
    else
        m_dialogs.m_temp_editor_dlg->hide();
}

void MainWindow::init_menu()
{
    // BASE ACTIONS
    {
        m_file_menu = menuBar()->addMenu("File");
        for(QAction * a : m_base_actions->getActionGroup()->actions())
            m_file_menu->addAction(a);
    }


    // RENDER MENU
    {
        m_render_menu = menuBar()->addMenu("Render");
        for(QAction * a : m_render_actions->getActionGroup()->actions())
            m_render_menu->addAction(a);
    }

    // CONTROLS MENU
    {
        m_controls_menu = menuBar()->addMenu("Controls");
        for(QAction * a : m_control_actions->getActionGroup()->actions())
            m_controls_menu->addAction(a);
    }

    // OVERLAY MENU
    {
;
        m_overlay_menu = menuBar()->addMenu("Overlay");
        for(QAction * a : m_overlay_actions->getActionGroup()->actions())
            m_overlay_menu->addAction(a);
    }

    // SHOW MENU
    {
        m_show_menu = menuBar()->addMenu("Show");
        for(QAction * a : m_show_actions->getActionGroup()->actions())
            m_show_menu->addAction(a);
    }

    // EDIT MENU
    {
        m_edit_menu = menuBar()->addMenu("Edit");
        for(QAction * a : m_edit_actions->getActionGroup()->actions())
            m_edit_menu->addAction(a);
    }

    // TMP MENU
    {
        m_tmp_menu = menuBar()->addMenu("Tmp");
        for(QAction * a : m_tmp_actions->getActionGroup()->actions())
            m_tmp_menu->addAction(a);
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

void MainWindow::establish_connections()
{
    connect((*m_base_actions)(BaseActions::_CLOSE_APP), SIGNAL(triggered()), this, SLOT(close()));
    connect((*m_base_actions)(BaseActions::_LOAD_TERRAIN), SIGNAL(triggered()), this, SLOT(load_terrain_file()));
    connect((*m_base_actions)(BaseActions::_OPEN_SETTINGS), SIGNAL(triggered()), this, SLOT(show_settings_dlg()));
    connect((*m_edit_actions)(EditActions::_TEMPERATURE), SIGNAL(triggered(bool)), this, SLOT(trigger_temp_edit_dlg(bool)));
}
