#include "reflowcontroller.h"
#include "ui_reflowcontroller.h"

ReflowController::ReflowController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReflowController)
{
    ui->setupUi(this);

    ui->temperatureGauge->setMin(0.0);
    ui->temperatureGauge->setMax(250.0);
}

ReflowController::~ReflowController()
{
    delete ui;
}

void ReflowController::openSerialPort(){
    serial.close();
    serial.setPortName("COM11");
    serial.setBaudRate(115200);
    if(serial.open(QIODevice::ReadWrite)){
        reader = new SerialPortReader(&serial);
        updateTimer = new QTimer();
        serial.write("1");
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
}


/***
 * Slots
 *
 */
void ReflowController::startStopButtonClicked(){
    if(!serial.isOpen()){
        this->openSerialPort();
    }else{
        serial.flush();
        serial.close();
    }
}

void ReflowController::updateParameters(){
    if(reader->dataReady){
        QStringList consoleOutput = reader->getData();
        QString output = consoleOutput.at(0);
        QStringList parameters = output.split("\t");

        float temperature = ((QString)parameters.at(0)).toFloat();
        this->ui->temperatureGauge->updateGauge(temperature);
        this->ui->temperatureLabel->setText(QString("%1 °C").arg(temperature));

        int status = ((QString)parameters.at(1)).toInt();
        this->ui->statusLabel->setText(getStatus(status));
    }
}
