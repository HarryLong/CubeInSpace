//#include "latitude_controller_widget.h"
//#include <QBoxLayout>
//#include <QLabel>

//#define DEFAULT_LATITUDE 0
//LatitudeSlider::LatitudeSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
//{
//    setRange(-90,90);
//    setValue(DEFAULT_LATITUDE);
//}

//LatitudeSlider::~LatitudeSlider()
//{

//}

////------------------------------------
//#define LATITUDE_CONTROLLER_HEIGHT 400
//#define LATITUDE_CONTROLLER_WIDTH 50
//LatitudeControllerWidget::LatitudeControllerWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent, f),
//    m_latitude_slider(new LatitudeSlider(this)), m_lbl(new QLabel(this))
//{
//    setFixedSize(LATITUDE_CONTROLLER_WIDTH, LATITUDE_CONTROLLER_HEIGHT);
//    init_layout();

//    connect(m_latitude_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
//}

//LatitudeControllerWidget::~LatitudeControllerWidget()
//{

//}

//int LatitudeControllerWidget::getLatitude() const
//{
//    return m_latitude_slider->value();
//}

//void LatitudeControllerWidget::init_layout()
//{
//    QVBoxLayout * main_layout(new QVBoxLayout);

//    // Slider
//    {
//        QHBoxLayout * layout(new QHBoxLayout);
//        layout->addWidget(m_latitude_slider, 1, Qt::AlignCenter);

//        main_layout->addLayout(layout,1);
//    }

//    // Label
//    {
//        QHBoxLayout * layout(new QHBoxLayout);
//        layout->addWidget(m_lbl, 1, Qt::AlignCenter);

//        main_layout->addLayout(layout,0);
//    }
//    setLayout(main_layout);
//}

//void LatitudeControllerWidget::refresh_label(int value)
//{
//    m_lbl->setText(QString::number(value));
//}
