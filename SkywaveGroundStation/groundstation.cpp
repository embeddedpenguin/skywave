#include "groundstation.h"
#include "ui_groundstation.h"
#include "gscustomplot.h"
#include "gsterminal.h"
#include "gsplot.h"
#include "gssettingsdialog.h"
#include <QTextStream>
#include <QStringList>
#include <QByteArray>
#include <QThread>
#include "gsplotlayout.h"


//This needs to be broken down into seperate initializers. Constructor is WAY too big atm
GroundStation::GroundStation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GroundStation)
{
    ui->setupUi(this);
    setMinimumSize(640, 480);
    statusLabel = new QLabel;
    setWindowTitle("Skywave Groundstation");
    //widget containers
    gsMain = new QWidget(this);
    gsPlotHub = new QWidget(this);

    //change port name from default
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();
    QString newPortName = serialPorts.count() > 0 ? serialPorts.at(0).portName() : "COM1";
    GSSettingsDialog::UpdatePortName(newPortName);


    //layouts for widgets
    gsMainLayout = new QVBoxLayout;
    gsPlotHubLayout = new GSPlotLayout;

    //terminal creation
    gsTerminal = new GSTerminal(this);



    //splitter so we can resize our terminal
    vertSplit = new QSplitter(Qt::Vertical);


    setCentralWidget(gsMain);
    gsMain->setLayout(gsMainLayout);
    gsMainLayout->addWidget(vertSplit);

    vertSplit->addWidget(gsPlotHub);
    vertSplit->addWidget(gsTerminal);

    //Make sure our graphs are not collapsible
    vertSplit->setCollapsible(0, false);
    vertSplit->setCollapsible(1, true);

    //add plots to layout
    gsPlotHub->setLayout(gsPlotHubLayout);

    QList<int> sizes;
    sizes << gsPlotHub->size().height() << 50;
    vertSplit->setSizes(sizes);
    //qDebug() << GetNumberOfPlots();
    gsPlotHubLayout->setMargin(0);

    gsTerminal->setEnabled(true);
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    ui->statusBar->addWidget(statusLabel);
    seconds = 0;
    InitActions();
}

GroundStation::~GroundStation()
{
    delete ui;
    if(serial->isOpen())
    {
        serial->close();
    }
    gsPlotBank.clear();
}

int GroundStation::GetNumberOfPlots() const
{
    return GSCustomPlot::GetNumberOfPlots();
}

void GroundStation::ConnectDevice()
{
    const Settings &iSettings = GSSettingsDialog::GetSettings();
    serial->setBaudRate(iSettings.baudRate);
    serial->setParity(iSettings.parity);
    serial->setDataBits(iSettings.dataBits);
    serial->setFlowControl(iSettings.flowControl);
    serial->setStopBits(iSettings.stopBits);
    serial->setPortName(iSettings.name);
    if(serial->open(QIODevice::ReadOnly))
    {
        //I've found that some MCUs need to have "setDataTerminalReady" to true.
        //From the SAMD21 user guide/documentation thing:
//        The virtual COM port in the EDBG requires the terminal software to set the data terminal
//        ready (DTR) signal to enable the UART pins connected to the ATSAMD21J18A. If the DTR
//        signal is not enabled the UART pins on the EDBG is kept in high-z (tristate) rendering the COM
//        port unusable. The DTR signal is set automatically by some terminal software, but it may have
//        to be manually enabled in your terminal.
        if(!serial->isDataTerminalReady())
        {
            serial->setDataTerminalReady(true);
        }
        ui->actionConnect->setEnabled(false);

        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        gsStatus = Connected;
        statusLabel->setText(tr("Status: Connected to %1. Baud: %2").arg(iSettings.name).arg(iSettings.baudRate));
    }
    else
    {
        QMessageBox::critical(this, tr("Error opening device"), serial->errorString());
        //showStatusMessage(tr("Error"));
        serial->close();

    }
}

void GroundStation::DisconnectDevice()
{
    if(serial->isOpen())
    {
        serial->close();
    }
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
}

void GroundStation::UpdateAllPlots(const QList<int> &data)
{

    for(int x = 0; x < data.count(); x++)
    {
        if(x < gsPlotBank.count())
        {
            gsPlotBank[x]->GetPlot().UpdatePlot( seconds, data.at(x));
        }
    }
    seconds++;
}

void GroundStation::readData()
{
    //this is the old method of streaming data. I ditched it because it would occasionally drop pieces of numbers and thats NO GOOD
//    QTextStream stream(serial);
//    serialBuffer += stream.read(1);
//    if(serialBuffer.endsWith("\n"))
//    {
//        QStringList serialList = serialBuffer.split(",", QString::SkipEmptyParts);
//        qDebug() << serialList;
//        serialBuffer.clear();
//    }


    //this is the new old method of streaming data. It's essentially the same thing as the current code but with more code.
    //keeping these notes here for future reference
//    serialData = serial->read(serial->bytesAvailable());
//    serialBuffer += serialData;
//    if(serialBuffer.endsWith("\n"))
//    {
//        qDebug() << serial->bytesAvailable();
//        QStringList serialList = serialBuffer.split(",", QString::SkipEmptyParts);
//        gsTerminal->putData(serialBuffer.toLocal8Bit());
//        qDebug() << serialList;
//        serialBuffer.clear();
//    }

    //current method of reading in data from our MCU
    if(serial->canReadLine())
    {
        serialData = serial->readLine();
        gsTerminal->putData(serialData);
        //convert serial data to a string. Use trimmed() to cut the \n character
        QString translatedData = QString(serialData).trimmed();
        //Create a string list we can use to convert to graphable values
        QStringList translatedDataList = translatedData.split(",", QString::SkipEmptyParts);
        //int list to store values
        QList<int> graphValues;
        for(int x = 0; x < translatedDataList.count(); x++)
        {
            if(QString(translatedDataList.at(x)).contains("."))
            {
                graphValues << qRound(QString(translatedDataList.at(x)).toFloat());
            }
            else
            {
                graphValues << QString(translatedDataList.at(x)).toInt();
            }

        }
        //Update graphs
        UpdateAllPlots(graphValues);
    }


}
void GroundStation::OpenSettings()
{
    settingsWindow = new GSSettingsDialog(this);
    settingsWindow->show();
}
void GroundStation::InitActions()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(ConnectDevice()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(DisconnectDevice()));
    connect(ui->actionConfigure, SIGNAL(triggered()), this, SLOT(OpenSettings()));
    ui->actionDisconnect->setEnabled(false);
}

void GroundStation::CreateNewPlot(QString Name, QString xAxisLabel, QString yAxisLabel)
{

    gsPlotBank.push_back(new GSCustomPlot(Name, this));
    gsPlotBank.last()->GetPlot().xAxis->setLabel(xAxisLabel);
    gsPlotBank.last()->GetPlot().yAxis->setLabel(yAxisLabel);
    gsPlotHubLayout->addWidget(gsPlotBank.last());

    QList<int> sizes;
    sizes << gsPlotHub->size().height() << 50;
    vertSplit->setSizes(sizes);



}

void GroundStation::CreateNewPlot(QString Name)
{

    gsPlotBank.push_back(new GSCustomPlot(Name, this));
    gsPlotHubLayout->addWidget(gsPlotBank.last());

    QList<int> sizes;
    sizes << gsPlotHub->size().height() << 50;
    vertSplit->setSizes(sizes);



}

QPointer<GSPlotLayout> GroundStation::gsPlotHubLayout;
