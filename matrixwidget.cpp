#include "matrixwidget.h"
#include <QVBoxLayout>

MatrixWidget::MatrixWidget(QWidget *parent, Qt::WindowFlags f, QMatrix4x4 matrix) : table(new QTableWidget(this))
{
    table->setRowCount(4);
    table->setColumnCount(4);
    setMatrix(matrix);

    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->addWidget(table.data());

    setLayout(mainLayout);
}

void MatrixWidget::setMatrix(const QMatrix4x4& mat)
{
    for(int row = 0; row < 4; ++row)
    {
        for(int column = 0; column < 4; ++column)
        {
            table.data()->setItem(row, column, new QTableWidgetItem(std::to_string(mat(row, column)).c_str()));
        }
    }
}
