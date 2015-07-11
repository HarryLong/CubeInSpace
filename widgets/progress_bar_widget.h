#ifndef PROGRESS_BAR_WIDGET_H
#define PROGRESS_BAR_WIDGET_H

#include <QWidget>

class QProgressBar;
class QLabel;
class ProgressBarWidget : public QWidget
{
    Q_OBJECT
public:
    ProgressBarWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~ProgressBarWidget();

    virtual QSize minimumSizeHint() const;

public slots:
    void processing(QString info);
    void updateProgress(int progress);
    void finishedProcessing();
    void updateInfoLabel(QString info);

protected:
     void paintEvent(QPaintEvent* event);

private:
    void init_layout();
    QProgressBar * m_progress_bar;
    QLabel * m_info_lbl;
};


#endif // PROGRESS_BAR_WIDGET_H
