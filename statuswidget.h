#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <QTableWidget>

class StatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusWidget(QWidget *parent = 0);
    ~StatusWidget();

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setXTranslation(int translation);
    void setYTranslation(int translation);
    void setZTranslation(int translation);
    void setScale(int scale);

protected:
    void setupTable();

private:
    QScopedPointer<QTableWidget> table;
};

#endif // STATUSWIDGET_H
