#include "progress_bar_widget.h"
#include <QProgressBar>
#include <QBoxLayout>
#include <QLabel>
#include <QPainter>

ProgressBarWidget::ProgressBarWidget(QWidget * parent, Qt::WindowFlags f) :
    QWidget(parent, f), m_progress_bar(new QProgressBar(this)), m_info_lbl(new QLabel(this))
{
    setVisible(false);
    init_layout();
}

ProgressBarWidget::~ProgressBarWidget()
{

}

void ProgressBarWidget::paintEvent(QPaintEvent* /*event*/)
{
    QColor backgroundColor (Qt::lightGray);
    QPainter customPainter(this);
    customPainter.fillRect(rect(),backgroundColor);
}

QSize ProgressBarWidget::minimumSizeHint() const
{
    return QSize(100,100);
}

void ProgressBarWidget::processing(QString info)
{
    updateInfoLabel(info);
    m_progress_bar->setRange(0, 100);
    m_progress_bar->setValue(0);
    setVisible(true);
    repaint();
}

void ProgressBarWidget::updateInfoLabel(QString info)
{
    m_info_lbl->setText("<span style=color:black;> <b>" + info + "</b> </span>");
}

void ProgressBarWidget::updateProgress(int progress)
{
    m_progress_bar->setValue(progress);
}

void ProgressBarWidget::finishedProcessing()
{
    setVisible(false);
}

void ProgressBarWidget::init_layout()
{
    QVBoxLayout * v_layout (new QVBoxLayout);

    m_progress_bar->setFixedWidth(width());

    v_layout->addWidget(m_progress_bar, 1, Qt::AlignCenter | Qt::AlignTop);
    v_layout->addWidget(m_info_lbl, 0, Qt::AlignCenter | Qt::AlignBottom);

    setLayout(v_layout);
}
