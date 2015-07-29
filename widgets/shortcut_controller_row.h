#ifndef SHORTCUTS_CONTROLLER_ROW_H
#define SHORTCUTS_CONTROLLER_ROW_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "../dialogs/custom_line_edit.h"

class ShortcutRow : public QWidget
{
public:
    ShortcutRow(QString label, int min, int max, QWidget * parent = 0);
    ~ShortcutRow();

    int value();
    QPushButton * m_ok_btn;

private:
    void init_layout();

    QLabel * m_label;
    QIntLineEdit * m_input;
};

#endif //SHORTCUTS_CONTROLLER_ROW_H
