#include <QtWidgets>

#include "analoggauge.h"

AnalogGauge::AnalogGauge(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Analog Clock"));
    resize(200, 200);
    this->minValue = 0;
    this->maxValue = 100;
    this->updateGauge(0);
}

void AnalogGauge::setMax(float maxValue){
    this->maxValue = maxValue;
}

void AnalogGauge::setMin(float minValue){
    this->minValue = minValue;
}

void AnalogGauge::setNeedleAngle(float angle){
    this->needleAngle = angle;
}

void AnalogGauge::updateGauge(float value){
    float updateValue = value;
    if(updateValue > this->maxValue){
        updateValue = this->maxValue;
    }else if(updateValue < this->minValue){
        updateValue = this->minValue;
    }

    float range = maxValue - minValue;
    float angleRange = MAX_DEGREE - ZERO_OFFSET_DEGREES;

    float updateAngle = (updateValue/range)*angleRange + ZERO_OFFSET_DEGREES;

    setNeedleAngle(updateAngle);
    this->repaint();
}

void AnalogGauge::paintEvent(QPaintEvent *)
{
    static const QPoint gaugeNeedle[5] = {
        QPoint(7, 6),
        QPoint(8, 7),
        QPoint(-8, 7),
        QPoint(-7, 6),
        QPoint(0, -70)
    };

    QColor needleColor(255, 0, 0, 255);
    QColor needleCapColor(255, 255, 255, 255);
    QColor needleCapColor1(117, 113, 113, 255);

    QConicalGradient needleCapGradient;
    needleCapGradient.setAngle(0);
    needleCapGradient.setCenter(.5, .5);
    needleCapGradient.setColorAt(0.00892857, needleCapColor1);
    needleCapGradient.setColorAt(1, needleCapColor);

    QRectF bounds(0,0,width(), height());

    int side = qMin(width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPixmap pixmap;
    pixmap.load(":/images/gauge_background.png");
    painter.drawPixmap(0,0,width(), height(), pixmap);

    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(Qt::SolidLine);
    painter.setBrush(needleColor);

    painter.rotate(needleAngle);
    painter.drawConvexPolygon(gaugeNeedle, 5);
    painter.restore();

    painter.setPen(Qt::SolidLine);
    QBrush brush(needleCapGradient);
    painter.setBrush(brush);
    painter.drawEllipse(bounds.center(), 7, 7);
}
