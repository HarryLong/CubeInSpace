#ifndef FLOATINGPOINTSLIDER_H_
#define FLOATINGPOINTSLIDER_H_

#include <QSlider>
#include "glheader.h"

#define MULTIPLIER 10000

class FloatingPointSlider : public QSlider {
    Q_OBJECT
public:
    FloatingPointSlider(Qt::Orientation orientation, QWidget * parent = 0 );
    virtual void setRange(float min, float max);
    virtual void setSingleStep(float step);
    virtual void setPageStep(float step);
    virtual void setValue(float value);
    virtual void setTickInterval(float value);

    virtual void setRange(int min, int max) { setRange((float) min, (float) max);}
    virtual void setSingleStep(int step) { setSingleStep((float) step); }
    virtual void setPageStep(int step) { setPageStep((float) step); }
    virtual void setValue(int value) { setValue((float) value) ;}
    virtual void setTickInterval(int value) { setTickInterval((float) value); }
    virtual float floatingPointValue();

signals:
    void floatingpointValueChanged(float value);

public slots:
    void signalValueChanged(int value);

private:
    int parseToInt(float value);
    float parseToFloat(int value);
};

#endif
