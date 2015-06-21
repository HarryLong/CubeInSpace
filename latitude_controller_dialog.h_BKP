#ifndef LATITUDE_CONTROLLER_DIALOG_H
#define LATITUDE_CONTROLLER_DIALOG_H

#include <QSlider>
#include <QDialog>

class LatitudeSlider : public QSlider
{
public:
    LatitudeSlider();
};

class QLabel;
class LatitudeControllerDialog : public QDialog {
    Q_OBJECT
public:
    LatitudeControllerDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~LatitudeControllerDialog();

    int getLatitude();

    LatitudeSlider * m_latitude_slider;

    QLabel * m_latitude_lbl;

protected:
    QSize sizeHint() const;//Override

private slots:
    void refresh_labels();

private:
    void init_layout();
    QString format_latitude(int latitude);
    QString get_two_number_digit(int digit);
};
#endif // LATITUDE_CONTROLLER_DIALOG_H
