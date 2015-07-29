#ifndef WATER_SHORTCUTS_CONTROLLER_H
#define WATER_SHORTCUTS_CONTROLLER_H

#include <QWidget>
#include "shortcut_controller_row.h"

class AbsoluteHeightShortcutRow : public ShortcutRow
{
public:
    AbsoluteHeightShortcutRow(QWidget * parent = 0);
    ~AbsoluteHeightShortcutRow();
};

class WaterShortcutWidget : public QWidget
{
Q_OBJECT
public:
    WaterShortcutWidget(int alignment, QWidget * parent = 0);
    ~WaterShortcutWidget();
    int alignment();
    int getAbsoluteHeight();

signals:
    void absoluteHeight(int absolute_height);

private slots:
    void emit_absolute_height();

private:
    void init_layout();
    int get_fill_rate();

    int m_alignment;
    ShortcutRow * m_absolute_height_shortcut;
};

#endif // WATER_SHORTCUTS_CONTROLLER_H
