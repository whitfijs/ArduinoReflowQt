#ifndef REFLOWCONTROLLER_H
#define REFLOWCONTROLLER_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <serialportreader.h>


namespace Ui {
class ReflowController;
}

class ReflowController : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReflowController(QWidget *parent = 0);
    ~ReflowController();

private:
    QSerialPort serial;
    SerialPortReader *reader;
    Ui::ReflowController *ui;
    QTimer * updateTimer;

    void openSerialPort();
    QString getStatus(int status);

private slots:
    void startStopButtonClicked();
    void updateParameters();
};

#endif // REFLOWCONTROLLER_H
