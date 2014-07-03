#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QScopedPointer>
#include <QTableWidget>
#include <QMatrix4x4>

class MatrixWidget : public QWidget
{
    Q_OBJECT
public:
    MatrixWidget(QWidget * parent = 0, Qt::WindowFlags f = 0, QMatrix4x4 matrix = QMatrix4x4());

public slots:
    void setMatrix(const QMatrix4x4& mat);

private:
    QScopedPointer<QTableWidget> table;
};
#endif
