#ifndef MONTHLY_RAINFALL_EDIT_DLG
#define MONTHLY_RAINFALL_EDIT_DLG

#include <QDialog>
#include <vector>
#include <QSlider>
#include <QPen>
#include <QLineEdit>
#include <QFont>

class RainfallSlider : public QSlider
{
Q_OBJECT
public:
    RainfallSlider(QWidget * parent = 0);
    ~RainfallSlider();
    QPoint getHandlePosition();
    static const QString _STYLESHEET;
};

//-------------------------------------------------------------------------------------

class QIntValidator;
class RainfallLineEdit : public QLineEdit
{
Q_OBJECT
public:
    RainfallLineEdit(QWidget * parent = 0);
    ~RainfallLineEdit();

signals:
    void valueChanged(int);

public slots:
    void setValue(int);

protected:
    void focusOutEvent ( QFocusEvent * event ) override;
private:
    QIntValidator * m_validator;
};

//-------------------------------------------------------------------------------------

class IntensityLineEdit : public QLineEdit
{
Q_OBJECT
public:
    IntensityLineEdit(QWidget * parent = 0);
    ~IntensityLineEdit();

protected:
    void focusOutEvent ( QFocusEvent * event ) override;

private:
        QIntValidator * m_validator;
};

//-------------------------------------------------------------------------------------

class SingleMonthRainfallWidget : public QWidget
{
Q_OBJECT
public:
    SingleMonthRainfallWidget(QString, QWidget * parent = 0);
    ~SingleMonthRainfallWidget();
    QPoint getHandlePosition();

    int getRainfall();
    int getRainfallIntensity();

    void push();
    void pop();

public slots:
    void emit_value_changed(int);

signals:
    void valueChanged(int);

private:
    void init_layout();
    RainfallSlider * m_rainfall_slider;
    RainfallLineEdit * m_rainfall_le;
    IntensityLineEdit * m_rainfall_intensity_le;
    QString m_label_txt;

    int m_pushed_rainfall;
    int m_pushed_intensity;

    static const QFont _LABEL_FONT;
};

//-------------------------------------------------------------------------------------

class LineEditQLabel : public QLineEdit
{
public:
    LineEditQLabel(QString text);

private:
    static const QString _STYLESHEET;
    static const QFont _LABEL_FONT;
};

class SingleMonthRainfallHeaderWidget : public QWidget
{
Q_OBJECT
public:
    SingleMonthRainfallHeaderWidget(QWidget * parent = 0);
    ~SingleMonthRainfallHeaderWidget();

private:
    void init_layout();
};

//-------------------------------------------------------------------------------------
class QPushButton;
class MonthlyRainfallEditDialog : public QDialog
{
Q_OBJECT
public:
    MonthlyRainfallEditDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~MonthlyRainfallEditDialog();
    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

    static std::map<int, QString> create_month_map();

public slots:
    void reject() override;

protected:
    virtual void paintEvent(QPaintEvent * event) override;
    virtual void showEvent(QShowEvent * event ) override ;

private:
//    QPoint get_slider_position(QSlider * slider);
    void init_layout();
    SingleMonthRainfallWidget* m_monthly_rainfall_widgets[12];
    QPen m_graph_drawer_pen;
    SingleMonthRainfallHeaderWidget * m_header_widget;

    QPushButton * m_ok;
    QPushButton * m_cancel;

    static const std::map<int,QString> _NUM_TO_MONTH;
};

#endif
