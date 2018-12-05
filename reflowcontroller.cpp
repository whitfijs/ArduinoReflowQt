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
    this->ui->leadedButton->setChecked(true);
    connect(ui->leadedButton, SIGNAL(clicked(bool)), this, SLOT(onLeadedSolderChecked(bool)));
    connect(ui->unleadedButton, SIGNAL(clicked(bool)), this, SLOT(onUnleadedSolderChecked(bool)));
    connect(ui->pivaHeatButton, SIGNAL(clicked(bool)), this, SLOT(onPivaHeatChecked(bool)));

}

ReflowController::~ReflowController()
{
    delete ui;
}

void ReflowController::openSerialPort(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    serial.close();
    serial.setPortName("COM11");
    serial.setBaudRate(115200);
    if(serial.open(QIODevice::ReadWrite)){
        reader = new SerialPortReader(&serial);
        updateTimer = new QTimer();
        connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateParameters()));
        updateTimer->start(100);
    }
}

bool ReflowController::openLogFile()
{
    qDebug() << "Open Log File";
    if(logOutput != nullptr)
    {
        logOutput->close();
    }

    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString localPath = QString("/reflow_logs/log_output_%1").arg(QDateTime::currentMSecsSinceEpoch()).append(".log");
    path.append(localPath);

    qDebug() << "Opening file at: " << path;
    logOutput = new QFile(path);
    if(logOutput->open(QIODevice::ReadWrite))
    {
        qDebug() << "Log file opened: " << path;
        return true;
    }

    return false;
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
        openLogFile();
    }else{
        serial.write("2");
        serial.waitForBytesWritten(100);
        this->ui->startButton->setText("Start");
        isStarted = false;
        logOutput->close();
    }
}

void ReflowController::updateParameters(){
    if(reader->dataReady){
        QStringList consoleOutput = reader->getData();
        QString output = (QString)consoleOutput.at(0);
        if(output.isEmpty()){
            return;
        }
        if(logOutput != nullptr)
        {
            logOutput->write(output.toStdString().c_str(), output.length());
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

void ReflowController::onLeadedSolderChecked(bool checked){
    if(serial.isOpen()){
        if(checked){
            serial.write("3");
            this->ui->unleadedButton->setChecked(false);
            this->ui->pivaHeatButton->setChecked(false);
        }
    }
}

void ReflowController::onUnleadedSolderChecked(bool checked){
    if(serial.isOpen()){
        if(checked){
            serial.write("4");
            this->ui->leadedButton->setChecked(false);
            this->ui->pivaHeatButton->setChecked(false);
        }
    }
}

void ReflowController::onPivaHeatChecked(bool checked){
    if(serial.isOpen()){
        if(checked){
            serial.write("5");
            this->ui->unleadedButton->setChecked(false);
            this->ui->leadedButton->setChecked(false);
        }
    }
}
