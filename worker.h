#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
signals:
    void processing(QString description);
    void processingUpdate(int percent_complete);
    void processingComplete();
    void processDescriptionUpdate(QString description);
};

#endif // WORKER_H
