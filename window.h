#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QComboBox>
#include "gl_core/central_widget.h"
#include <QMenu>
#include <map>
#include "actions.h"

class ActionFamily;
class TimeControllerDialog;
class PointerInformationDialog;
class SettingsDialog;
class TemperatureEditDialog;
class RainfallEditDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void init_menu();
    void init_actions();
    void establish_connections();

    AllActions * m_actions;

    // Menu elements
    QMenu * m_file_menu;
    QMenu * m_render_menu;
    QMenu * m_controls_menu;
    QMenu * m_overlay_menu;
    QMenu * m_show_menu;
    QMenu * m_edit_menu;

    // Submenu elements
    QMenu * m_illumination_submenu;
    QMenu * m_soil_humidity_submenu;
};

#endif
