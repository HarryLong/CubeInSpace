#include "monthly_rainfall_edit_dlg.h"
#include <QBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QIntValidator>
#include <QResizeEvent>
#include <QLabel>
#include <QPushButton>

const QString RainfallSlider::_STYLESHEET =
        "QSlider::groove:vertical {\
            background: transparent;\
        }\
        QSlider::handle:vertical {\
            background: black;\
            border: 4px solid #000000;\
            width: 1px;\
            margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\
            border-radius: 3px;\
        }";
#define MIN_RAINFALL 0
#define MAX_RAINFALL 1000
#define RAINFALL_DEFAULT 0
RainfallSlider::RainfallSlider(QWidget * parent) : QSlider(Qt::Orientation::Vertical, parent)
{
    setStyleSheet(RainfallSlider::_STYLESHEET);
    setRange(MIN_RAINFALL, MAX_RAINFALL);
    reset();
}

RainfallSlider::~RainfallSlider()
{

}

QPoint RainfallSlider::getHandlePosition()
{
    return mapToParent(QPoint(width()/2.0f, height() - ((((float)value())/maximum()) * height())));
}

void RainfallSlider::reset()
{
    setValue(RAINFALL_DEFAULT);
}

//-------------------------------------------------------------------------------------
RainfallLineEdit::RainfallLineEdit(QWidget * parent) :
    QLineEdit(parent),
    m_validator(new QIntValidator(MIN_RAINFALL, MAX_RAINFALL, parent))
{
    setValidator(m_validator);
    reset();
}

RainfallLineEdit::~RainfallLineEdit()
{
    delete m_validator;
}

void RainfallLineEdit::focusOutEvent( QFocusEvent * event )
{
    QLineEdit::focusOutEvent(event);
    if(text().isEmpty())
        setText(QString::number(0));

    emit valueChanged(text().toInt());
}

void RainfallLineEdit::setValue(int value)
{
    setText(QString::number(value));
}

void RainfallLineEdit::reset()
{
    setText(QString::number(RAINFALL_DEFAULT));
}

//--------------------------------------------------------------------------------------
#define MIN_RAINFALL_INTENSITY 0
#define MAX_RAINFALL_INTENSITY 50
#define DEFAULT_RAINFALL_INTENSITY 5
IntensityLineEdit::IntensityLineEdit(QWidget * parent) :
    QLineEdit(parent),
    m_validator(new QIntValidator(MIN_RAINFALL_INTENSITY, MAX_RAINFALL_INTENSITY, parent))
{
    setValidator(m_validator);
    reset();
}

IntensityLineEdit::~IntensityLineEdit()
{
    delete m_validator;
}

void IntensityLineEdit::focusOutEvent( QFocusEvent * event )
{
    QLineEdit::focusOutEvent(event);
    if(text().isEmpty())
        setText(QString::number(0));
}

void IntensityLineEdit::reset()
{
    setText(QString::number(DEFAULT_RAINFALL_INTENSITY));
}

//--------------------------------------------------------------------------------------
const QFont SingleMonthRainfallWidget::_LABEL_FONT("Times", 15, QFont::Weight::Bold);
SingleMonthRainfallWidget::SingleMonthRainfallWidget(QString label_txt, QWidget * parent) :
    QWidget(parent),
    m_rainfall_slider(new RainfallSlider(this)),
    m_rainfall_le(new RainfallLineEdit(this)),
    m_rainfall_intensity_le(new IntensityLineEdit(this)),
    m_label_txt(label_txt)
{
    connect(m_rainfall_le, SIGNAL(valueChanged(int)), m_rainfall_slider, SLOT(setValue(int)));
    connect(m_rainfall_le, SIGNAL(valueChanged(int)), this, SLOT(emit_value_changed(int)));

    connect(m_rainfall_slider, SIGNAL(valueChanged(int)), m_rainfall_le, SLOT(setValue(int)));
    connect(m_rainfall_slider, SIGNAL(valueChanged(int)), this, SLOT(emit_value_changed(int)));

    init_layout();
}

SingleMonthRainfallWidget::~SingleMonthRainfallWidget()
{

}

void SingleMonthRainfallWidget::reset()
{
    m_rainfall_slider->reset();
    m_rainfall_le->reset();
    m_rainfall_intensity_le->reset();
}

QPoint SingleMonthRainfallWidget::getHandlePosition()
{
    return mapToParent(m_rainfall_slider->getHandlePosition());
}

void SingleMonthRainfallWidget::emit_value_changed(int value)
{
    emit valueChanged(value);
}

void SingleMonthRainfallWidget::init_layout()
{
    QBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(m_rainfall_slider, 8, Qt::AlignHCenter);
    layout->addWidget(m_rainfall_le, 2, Qt::AlignHCenter);
    layout->addWidget(m_rainfall_intensity_le, 2, Qt::AlignHCenter);

    QLabel * lbl = new QLabel(m_label_txt);
    lbl->setFont(SingleMonthRainfallWidget::_LABEL_FONT);

    layout->addWidget(lbl, 2, Qt::AlignCenter);

    setLayout(layout);
}

int SingleMonthRainfallWidget::getRainfall()
{
    return m_rainfall_slider->value();
}

int SingleMonthRainfallWidget::getRainfallIntensity()
{
    return m_rainfall_intensity_le->text().toInt();
}

void SingleMonthRainfallWidget::push()
{
    m_pushed_intensity = m_rainfall_intensity_le->text().toInt();
    m_pushed_rainfall = m_rainfall_slider->value();
}

void SingleMonthRainfallWidget::pop()
{
    m_rainfall_intensity_le->setText(QString::number(m_pushed_intensity));
    m_rainfall_le->setText(QString::number(m_pushed_rainfall));
    m_rainfall_slider->setValue(m_pushed_rainfall);
}

//-------------------------------------------------------------------------------------
const QString LineEditQLabel::_STYLESHEET =
"QLineEdit {\
border: 0px solid gray;\
border-radius: 00px;\
padding: 0 0px;\
background: transparent;}";
const QFont LineEditQLabel::_LABEL_FONT("Times", 12, QFont::Weight::Bold);
LineEditQLabel::LineEditQLabel(QString text) :
    QLineEdit(text)
{
    setStyleSheet(LineEditQLabel::_STYLESHEET);
    setAlignment(Qt::AlignRight);
    setFont(LineEditQLabel::_LABEL_FONT);
}

SingleMonthRainfallHeaderWidget::SingleMonthRainfallHeaderWidget(QWidget * parent)
{
    init_layout();
}

SingleMonthRainfallHeaderWidget::~SingleMonthRainfallHeaderWidget()
{

}

void SingleMonthRainfallHeaderWidget::init_layout()
{
    QBoxLayout * layout = new QVBoxLayout;

    // Dummy space for Slider in main widgets
    layout->addWidget(new QLabel("", this), 8);

    // Rainfall Heading
    {
        QLineEdit * le = new LineEditQLabel("Rainfall (mm): ");
        le->setReadOnly(true);
        layout->addWidget(le, 2);
    }

    // Rainfall intensity heading
    {
        QLineEdit * le = new LineEditQLabel("Rainfall Intensity (mm/h): ");
        le->setReadOnly(true);
        layout->addWidget(le, 2);
    }

    // Dummy space for the month label in main widgets
    layout->addWidget(new QLabel(" ", this), 2);

    setLayout(layout);
}

//--------------------------------------------------------------------------------------
const std::map<int,QString> MonthlyRainfallEditDialog::_NUM_TO_MONTH = MonthlyRainfallEditDialog::create_month_map();
MonthlyRainfallEditDialog::MonthlyRainfallEditDialog(QWidget * parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    m_header_widget(new SingleMonthRainfallHeaderWidget(this)),
    m_ok(new QPushButton("OK", this)),
    m_cancel(new QPushButton("Cancel", this))
{
    setWindowTitle("Monthly Rainfall");
    m_graph_drawer_pen.setStyle(Qt::SolidLine);
    m_graph_drawer_pen.setWidth(3);
    m_graph_drawer_pen.setBrush(Qt::blue);
    m_graph_drawer_pen.setCapStyle(Qt::RoundCap);
    m_graph_drawer_pen.setJoinStyle(Qt::RoundJoin);

    for(int i = 0; i < 12; i++)
    {
        SingleMonthRainfallWidget * w = new SingleMonthRainfallWidget(_NUM_TO_MONTH.find(i+1)->second, this);
        connect(w, SIGNAL(valueChanged(int)), this, SLOT(update()));
        m_monthly_rainfall_widgets[i] = w;
    }

    init_layout();

    connect(m_ok, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(m_cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

MonthlyRainfallEditDialog::~MonthlyRainfallEditDialog()
{

}

void MonthlyRainfallEditDialog::reset()
{
    for(SingleMonthRainfallWidget * w : m_monthly_rainfall_widgets)
        w->reset();

    update();
}

QSize MonthlyRainfallEditDialog::minimumSizeHint() const
{
    return QSize(1400,800);
}

QSize MonthlyRainfallEditDialog::sizeHint() const
{
    return minimumSizeHint();
}

int MonthlyRainfallEditDialog::getRainfall(int month)
{
    return m_monthly_rainfall_widgets[month-1]->getRainfall();
}

int MonthlyRainfallEditDialog::getRainfallIntensity(int month)
{
    return m_monthly_rainfall_widgets[month-1]->getRainfallIntensity();
}

void MonthlyRainfallEditDialog::showEvent(QShowEvent * event )
{
    QDialog::showEvent(event);

    for(SingleMonthRainfallWidget * w : m_monthly_rainfall_widgets)
        w->push();
}

void MonthlyRainfallEditDialog::paintEvent(QPaintEvent * event)
{
    qCritical() << "PAINTING";

    QPainter painter(this);
    painter.setPen(m_graph_drawer_pen);

    for(int i = 0; i < 11; i++)
    {
        QPoint p1(m_monthly_rainfall_widgets[i]->getHandlePosition());
        QPoint p2(m_monthly_rainfall_widgets[i+1]->getHandlePosition());

        painter.drawLine(p1, p2);
    }

    painter.end();

    QDialog::paintEvent(event);
}

void MonthlyRainfallEditDialog::reject()
{
    for(SingleMonthRainfallWidget * w : m_monthly_rainfall_widgets)
        w->pop();

    QDialog::reject();
}

void MonthlyRainfallEditDialog::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;

    // Sliders
    {
        QHBoxLayout * layout = new QHBoxLayout;

        // Add header widget
        layout->addWidget(m_header_widget, 5);

        for(QWidget * s : m_monthly_rainfall_widgets)
            layout->addWidget(s,2);

        main_layout->addLayout(layout,19);
    }

    // OK/CANCEL Button
    {
        QBoxLayout * layout = new QHBoxLayout;
        layout->addWidget(new QLabel(""),1); // padding
        layout->addWidget(m_ok);
        layout->addWidget(m_cancel);
        layout->addWidget(new QLabel(""),1); // padding
        main_layout->addLayout(layout,1);
    }

    setLayout(main_layout);
}

std::map<int, QString> MonthlyRainfallEditDialog::create_month_map()
{
    std::map<int, QString> ret;
    ret[1] = "Jan";
    ret[2] = "Feb";
    ret[3] = "Mar";
    ret[4] = "Apr";
    ret[5] = "May";
    ret[6] = "Jun";
    ret[7] = "Jul";
    ret[8] = "Aug";
    ret[9] = "Sep";
    ret[10] = "Oct";
    ret[11] = "Nov";
    ret[12] = "Dec";

    return ret;
}
