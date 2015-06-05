#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QComboBox>
#include "glwidget.h"
#include <QMenu>
#include <map>

class TimeControllerDialog;
class LatitudeControllerDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QGLFormat& format);
    ~MainWindow();

private slots:
    void load_terrain_file();
    void render_grid_toggled();
    void render_terrain_toggled();
    void render_acceleration_structure_toggled();
    void render_rays_toggled();
    void render_sun_toggled();
    void refresh_mode();
    void show_time_controller_dlg();
    void show_latitude_controller_dlg();
    void show_settings_dlg();
    void overlay_action_toggled(QAction* action);

private slots:
    void refresh_control_style();

private:
    void init_menu();

    // Menu elements
    QMenu * m_file_menu;
    QMenu * m_render_menu;
    QMenu * m_controls_menu;
    QMenu * m_overlay_menu;
    QMenu * m_mode_menu;
    QMenu * m_show_menu;

    // Actions
    QAction * m_action_close;
    QAction * m_action_load_terrain;
    QAction * m_action_open_settings_dialog;
    // Render actions
    QAction * m_action_render_grid;
    QAction * m_action_render_terrain;
    QAction * m_action_render_acceleration_structure;
    QAction * m_action_render_rays;
    QAction * m_action_render_sun;

    // Control actions
    QActionGroup* m_control_action_group;
    QAction * m_action_control_softimage;
    QAction * m_action_control_fps;
    QAction * m_action_control_experimental;
    std::map<ControlStyle, QAction*> m_control_style_to_action_map;

    // Overlay
    QActionGroup* m_overlays_action_group;
    QAction * m_terrain_options_overlay_none;
    QAction * m_terrain_options_overlay_slope;
    QAction * m_terrain_options_overlay_altitude;
    QAction * m_terrain_options_overlay_shade;

    // Mode options
    QActionGroup* m_mode_action_group;
    QAction * m_mode_none;
    QAction * m_mode_terrain_edit;
    QAction * m_mode_selection;
    QAction * m_mode_terrain_orientation_edit;
    std::map<Mode, QAction*> m_mode_to_action_map;

    // Show options
    QAction * m_show_time_controller_dialog;
    QAction * m_show_latitude_controller_dialog;

    // Dialogs
    SettingsEditorDialog * m_settings_editor_dlg;
    TimeControllerDialog * m_time_controller_dlg;
    LatitudeControllerDialog * m_latitude_controller_dlg;

    GLWidget * m_glwidget;
    QAction * m_overlay_selected_action;
};

#endif
