#ifndef REFLOWCONTROLLER_H
#define REFLOWCONTROLLER_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QCoreApplication>
#include <QStringList>
#include <serialportreader.h>

#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QDateTime>
#include <QString>

#include <QDebug>


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

    QFile * logOutput;

    void openSerialPort();
    QString getStatus(int status);
    bool openLogFile();

private slots:
    void startStopButtonClicked();
    void updateParameters();
    void closeEvent(QCloseEvent * event);
    void onLeadedSolderChecked(bool checked);
    void onUnleadedSolderChecked(bool checked);
    void onPivaHeatChecked(bool checked);
};

#endif // REFLOWCONTROLLER_H
