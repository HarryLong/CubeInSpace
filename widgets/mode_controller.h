#ifndef MODE_CONTROLLER_H
#define LATITUDE_CONTROLLER_H

#include <QWidget>

class QPushButton;
class QLabel;
class ModeControllerWidget : public QWidget
{
    Q_OBJECT
public:
    ModeControllerWidget(int alignment, QString label, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~ModeControllerWidget();

    int alignment();

public slots:
    void setLabel(QString lbl);

signals:
    void triggered();

private slots:
    void emit_triggered();

private:
    void init_layout();
    void init_button();
    int m_alignment;
    QPushButton * m_btn;
};

#endif // LATITUDE_CONTROLLER_H
