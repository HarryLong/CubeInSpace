#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QComboBox>
#include "glwidget.h"
#include <QMenu>
#include <map>

class Actions;
class TimeControllerDialog;
class PointerInformationDialog;
class SettingsDialog;
class TemperatureEditDialog;

class Dialogs {
public:
    Dialogs(QWidget * parent);

    SettingsDialog * m_settings_dlg;
//    TimeControllerDialog * m_time_controller_dlg;
    TemperatureEditDialog * m_temp_editor_dlg;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void load_terrain_file();
    void show_settings_dlg();
    void show_temp_edit_dlg();

private:
    void init_menu();
    void init_actions();

    Dialogs m_dialogs;

    Actions * m_base_actions;
    Actions * m_render_actions;
    Actions * m_control_actions;
    Actions * m_overlay_actions;
    Actions * m_show_actions;
    Actions * m_edit_actions;

    // Menu elements
    QMenu * m_file_menu;
    QMenu * m_render_menu;
    QMenu * m_controls_menu;
    QMenu * m_overlay_menu;
    QMenu * m_show_menu;
    QMenu * m_edit_menu;

    GLWidget * m_glwidget;
};

#endif
