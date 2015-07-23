#include <QWidget>
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

class SlopeDependentShortcutRow : public ShortcutRow
{
public:
    SlopeDependentShortcutRow(QWidget * parent = 0);
    ~SlopeDependentShortcutRow();
};

class FillShortcutRow : public ShortcutRow
{
public:
    FillShortcutRow(QWidget * parent = 0);
    ~FillShortcutRow();
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
    void fill(int infiltration_rate);

private slots:
    void emit_soil_infiltration_zero_over_slope();
    void emit_fill();

private:
    void init_layout();
    int get_fill_rate();

    int m_alignment;
    ShortcutRow * m_slope_dependant_shortcut;
    ShortcutRow * m_fill_shortcut;
};
