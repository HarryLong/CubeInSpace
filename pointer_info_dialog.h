#ifndef POINTER_INFO_DIALOG_H
#define POINTER_INFO_DIALOG_H

#include <QDialog>

class QLabel;
class PointerInformationDialog : public QDialog
{
    Q_OBJECT
public:
    PointerInformationDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~PointerInformationDialog();

    virtual QSize minimumSizeHint() const;

public slots:
    void update(float altitude, float slope,
                bool shade_set, bool shaded,
                bool temp_set, float min_temp, float max_temp,
                bool daily_illumination_set, int min_daily_illumination, int max_daily_illumination);
    void invalidPoint();

private slots:
    void setAltitude(float altitude);
    void setSlope(float slope);
    void setMinTemp(float min_temp);
    void setMaxTemp(float max_temp);
    void setShaded(bool shaded);
    void setMinDailyIllumination(int min_daily_illumination);
    void setMaxDailyIllumination(int max_daily_illumination);

    void setInvalid(QLabel * lbl);

private:
    void init_layout();
    QLabel * m_altitude_lbl;
    QLabel * m_slope_lbl;
    QLabel * m_shade_lbl;
    QLabel * m_min_temp_lbl;
    QLabel * m_max_temp_lbl;
    QLabel * m_daily_min_illumination_lbl;
    QLabel * m_daily_max_illumination_lbl;
};


#endif // POINTER_INFO_DIALOG_H
