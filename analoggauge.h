#ifndef ANALOGGAUGE_H
#define ANALOGGAUGE_H

#include <QWidget>

#define ZERO_OFFSET_DEGREES -135.0f
#define MAX_DEGREE          135.0f


class AnalogGauge : public QWidget
{
    Q_OBJECT

public:
    AnalogGauge(QWidget *parent = 0);
    void setMin(float minValue);
    void setMax(float maxValue);
    void updateGauge(float value);

private:
    float needleAngle, minValue, maxValue;
    void setNeedleAngle(float angle);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};


#endif // ANALOGGAUGE_H
