#ifndef REFLOWCONTROLLER_H
#define REFLOWCONTROLLER_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
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
    bool isStarted;

    void openSerialPort();
    QString getStatus(int status);

private slots:
    void startStopButtonClicked();
    void updateParameters();
    void closeEvent(QCloseEvent * event);
};

#endif // REFLOWCONTROLLER_H
