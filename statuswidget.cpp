#include "statuswidget.h"
#include <QString>
#include <QFont>
#include "glheader.h"

#define ROW_COUNT 7
#define COLUMN_COUNT 2

#define ROW_SCALE 0
#define ROW_X_TRAN 1
#define ROW_Y_TRAN 2
#define ROW_Z_TRAN 3
#define ROW_X_ROT 4
#define ROW_Y_ROT 5
#define ROW_Z_ROT 6


StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent), table(new QTableWidget(this))
{
    setupTable();
}

void StatusWidget::setupTable()
{
    QFont headingFont;
    headingFont.setWeight(QFont::Bold);

    table->setRowCount(ROW_COUNT);
    table->setColumnCount(COLUMN_COUNT);

    // SCALE
    {
        QTableWidgetItem * newItem = new QTableWidgetItem(tr("Scale"));
        newItem->setFont(headingFont);
        table->setItem(ROW_SCALE,0, newItem);
        setScale(1);
    }

    // X TRANSLATION
    {
        QTableWidgetItem * newItem = new QTableWidgetItem(tr("X Translation"));
        newItem->setFont(headingFont);
        table->setItem(ROW_X_TRAN,0, newItem);
        setXTranslation(0);
    }

    // Y TRANSLATION
    {
        QTableWidgetItem * newItem = new QTableWidgetItem(tr("Y Translation"));
        newItem->setFont(headingFont);
        table->setItem(ROW_Y_TRAN,0, newItem);
        setYTranslation(0);
    }

    // Z TRANSLATION
    {
        QTableWidgetItem * newItem = new QTableWidgetItem(tr("Z Translation"));
        newItem->setFont(headingFont);
        table->setItem(ROW_Z_TRAN,0, newItem);
        setZTranslation(0);
    }

    // X ROTATION
    {
        QTableWidgetItem * newItem = new QTableWidgetItem(tr("X Rotation"));
        newItem->setFont(headingFont);
        table->setItem(ROW_X_ROT,0, newItem);
        setXRotation(0);
    }

    // Y ROTATION
    {
        QTableWidgetItem * newItem = new QTableWidgetItem(tr("Y Rotation"));
        newItem->setFont(headingFont);
        table->setItem(ROW_Y_ROT,0, newItem);
        setYRotation(0);
    }

    // Z ROTATION
    {
        QTableWidgetItem * newItem = new QTableWidgetItem(tr("X Rotation"));
        newItem->setFont(headingFont);
        table->setItem(ROW_Z_ROT,0, newItem);
        setZRotation(0);
    }
}

StatusWidget::~StatusWidget()
{
}

void StatusWidget::setXRotation(int angle)
{
    DEBUG_MSG
    table->setItem(ROW_X_ROT,1, new QTableWidgetItem(QString::number(angle)));
}

void StatusWidget::setYRotation(int angle)
{
    DEBUG_MSG
    table->setItem(ROW_Y_ROT,1, new QTableWidgetItem(QString::number(angle)));
}

void StatusWidget::setZRotation(int angle)
{
    DEBUG_MSG
    table->setItem(ROW_Z_ROT,1, new QTableWidgetItem(QString::number(angle)));
}

void StatusWidget::setXTranslation(int translation)
{
    DEBUG_MSG
    table->setItem(ROW_X_TRAN,1, new QTableWidgetItem(QString::number(translation)));
}

void StatusWidget::setYTranslation(int translation)
{
    DEBUG_MSG
    table->setItem(ROW_Y_TRAN,1, new QTableWidgetItem(QString::number(translation)));
}

void StatusWidget::setZTranslation(int translation)
{
    DEBUG_MSG
    table->setItem(ROW_Z_TRAN,1, new QTableWidgetItem(QString::number(translation)));
}

void StatusWidget::setScale(int scale)
{
    DEBUG_MSG
    table->setItem(ROW_SCALE,1, new QTableWidgetItem(QString::number(scale)));
}
