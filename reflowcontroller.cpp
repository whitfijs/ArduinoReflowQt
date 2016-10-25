#include "reflowcontroller.h"
#include "ui_reflowcontroller.h"

ReflowController::ReflowController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReflowController)
{
    ui->setupUi(this);

    ui->temperatureGauge->setMin(0.0);
    ui->temperatureGauge->setMax(250.0);

    connect(ui->startButton, SIGNAL(pressed()), this, SLOT(startStopButtonClicked()));
    this->openSerialPort();
    isStarted = false;
}

ReflowController::~ReflowController()
{
    delete ui;
}

void ReflowController::openSerialPort(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    serial.close();
    serial.setPortName("/dev/ttyUSB0");
    serial.setBaudRate(115200);
    if(serial.open(QIODevice::ReadWrite)){
        reader = new SerialPortReader(&serial);
        updateTimer = new QTimer();
        connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateParameters()));
        updateTimer->start(100);
    }
}



QString ReflowController::getStatus(int status){
    QString returnString = "Status: ";
    if(status & 0x10){
        return returnString.append("IDLE");
    }else if(status & 0x08){
        return returnString.append("COOL");
    }else if(status & 0x04){
        return returnString.append("REFLOW");
    }else if(status & 0x02){
        return returnString.append("SOAK");
    }else if(status & 0x01){
        return returnString.append("RAMP UP");
    }
    return "--";
}


/***
 * Slots
 *
 */
void ReflowController::startStopButtonClicked(){
    if(!isStarted){
        serial.write("1");
        serial.waitForBytesWritten(100);
        this->ui->startButton->setText("Stop");
        isStarted = true;
    }else{
        serial.write("0");
        serial.waitForBytesWritten(100);
        this->ui->startButton->setText("Start");
        isStarted = false;
    }
}

void ReflowController::updateParameters(){
    if(reader->dataReady){
        QStringList consoleOutput = reader->getData();
        QString output = (QString)consoleOutput.at(0);
        if(output.isEmpty()){
            return;
        }
        QStringList parameters = output.split("\t");
        if(parameters.length() < 4){
            return;
        }

        float temperature = ((QString)parameters.at(0)).toFloat();
        this->ui->temperatureGauge->updateGauge(temperature);
        this->ui->temperatureLabel->setText(QString("%1 °C").arg(temperature));

        int status = ((QString)parameters.at(1)).toInt();
        this->ui->statusLabel->setText(getStatus(status));

        int powerOutputRaw = ((QString)parameters.at(2)).toInt();
        float powerOutput = (15200.0 - (float)powerOutputRaw)/15200.0;
        powerOutput*=100.0;
        this->ui->powerOutputProgressBar->setValue(powerOutput);
        this->ui->powerOutputLabel->setText(QString("Power Output: %1%").arg(powerOutput));

    }
}

void ReflowController::closeEvent(QCloseEvent *event){
    serial.flush();
    serial.close();
    event->accept();
}
