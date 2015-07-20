#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "../dialogs/custom_line_edit.h"

class ShortcutRow : public QWidget
{
public:
    ShortcutRow(QString label, QWidget * parent = 0);
    ~ShortcutRow();

    int value();
    QPushButton * m_ok_btn;

private:
    void init_layout();

    QLabel * m_label;
    QIntLineEdit * m_input;
};

class SoilInfiltrationShortcutWidget : public QWidget
{
Q_OBJECT
public:
    SoilInfiltrationShortcutWidget(int alignment, QWidget * parent = 0);
    ~SoilInfiltrationShortcutWidget();
    int alignment();
    int getSoilInfiltrationZeroOverSlope();

signals:
    void soilInfiltrationZeroOverSlope(int min_slope);

private slots:
    void emit_soil_infiltration_zero_over_slope();

private:
    void init_layout();

    int m_alignment;
    ShortcutRow * m_slope_dependant_shortcut;
};
