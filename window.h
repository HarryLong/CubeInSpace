#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QComboBox>
#include "glwidget.h"
#include <QMenu>
#include <map>

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(const QGLFormat& format);
    ~Window();

private slots:
    void load_terrain_file();
    void render_grid_toggled();
    void render_assets_toggled();
    void render_terrain_toggled();
    void render_acceleration_structure_toggled();
    void render_rays_toggled();

    void open_settings_dialog();

private slots:
    void refresh_control_style();

private:
    void init_menu();

    GLWidget * m_glwidget;

    // Menu elements
    QMenu * m_file_menu;

    // Actions
    QAction * m_action_close;
    QAction * m_action_load_terrain;
    QAction * m_action_open_settings_dialog;
    // Render actions
    QAction * m_action_render_grid;
    QAction * m_action_render_assets;
    QAction * m_action_render_terrain;
    QAction * m_action_render_acceleration_structure;
    QAction * m_action_render_rays;

    // Control actions
    QActionGroup* m_control_action_group;
    QAction * m_action_control_softimage;
    QAction * m_action_control_fps;
    QAction * m_action_control_experimental;
    // Terrain options
    QActionGroup* m_terrain_overlays_group;
    QAction * m_terrain_options_overlay_none;
    QAction * m_terrain_options_overlay_slope;
    QAction * m_terrain_options_overlay_altitude;

    std::map<ControlStyle, QAction*> m_control_style_to_action_map;
};

#endif
