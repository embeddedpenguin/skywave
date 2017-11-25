#include "gssettingsdialog.h"
#include "ui_gssettingsdialog.h"


static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");


GSSettingsDialog::GSSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GSSettingsDialog)
{
    ui->setupUi(this);
    FillParameters();
    FillPortsInfo();
    UpdateSettings();
    ShowPortInfo(ui->serialPortBox->currentIndex());
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ApplyUpdates()));
    connect(ui->serialPortBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &GSSettingsDialog::ShowPortInfo);

}

GSSettingsDialog::~GSSettingsDialog()
{
    delete ui;
}

Settings &GSSettingsDialog::GetSettings()
{
    return gsSettings;
}

void GSSettingsDialog::UpdatePortName(QString iString)
{
    gsSettings.name = iString;
}

void GSSettingsDialog::ApplySettings(const Settings &iSettings)
{
    gsSettings = iSettings;
}

void GSSettingsDialog::FillParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    for(int x = ui->baudRateBox->count() - 1; x >= 0; x--)
    {
        if(static_cast<QSerialPort::BaudRate>(ui->baudRateBox->itemData(x).toInt()) == gsSettings.baudRate)
        {
            ui->baudRateBox->setCurrentIndex(x);
            break;
        }
    }

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    for(int x = ui->dataBitsBox->count() - 1; x >= 0; x--)
    {
        if(static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(x).toInt()) == gsSettings.dataBits)
        {
            ui->dataBitsBox->setCurrentIndex(x);
            break;
        }
    }

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    for(int x = ui->stopBitsBox->count() - 1; x >= 0; x--)
    {
        if(static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(x).toInt()) == gsSettings.stopBits)
        {
            ui->stopBitsBox->setCurrentIndex(x);
            break;
        }
    }

    //this may seem pointless because there is only 1 option for flow control, but if I ever need to add more options down the line I can
    ui->flowControlBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    for(int x = ui->flowControlBox->count() - 1; x >= 0; x--)
    {
        if(static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(x).toInt()) == gsSettings.flowControl)
        {
            ui->flowControlBox->setCurrentIndex(x);
            break;
        }
    }


    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);
    for(int x = ui->parityBox->count() - 1; x >= 0; x--)
    {
        if(static_cast<QSerialPort::Parity>(ui->parityBox->itemData(x).toInt()) == gsSettings.parity)
        {
            ui->parityBox->setCurrentIndex(x);
            break;
        }
    }




}

void GSSettingsDialog::FillPortsInfo()
{
    ui->serialPortBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortBox->addItem(list.first(), list);
    }
}

void GSSettingsDialog::UpdateSettings()
{
    Settings activeSettings;
    activeSettings.name = ui->serialPortBox->currentText();
    qDebug() << activeSettings.name;
    activeSettings.baudRate = static_cast<QSerialPort::BaudRate>(ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    activeSettings.dataBits = static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    activeSettings.flowControl = static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    activeSettings.parity = static_cast<QSerialPort::Parity>(ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    activeSettings.stopBits = static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    gsSettings = activeSettings;


}

void GSSettingsDialog::ApplyUpdates()
{
    UpdateSettings();
}

void GSSettingsDialog::ShowPortInfo(int ID)
{
    if(ID == -1)
    {
        return;
    }
    QStringList list = ui->serialPortBox->itemData(ID).toStringList();
    ui->description_info->setText(list.count() > 1 ? list.at(1) : tr(blankString));
    ui->manufacturer_info->setText(list.count() > 2 ? list.at(2) : tr(blankString));
    ui->serial_number_info->setText(list.count() > 3 ? list.at(3) : tr(blankString));
    ui->vendor_id_info->setText(list.count() > 4 ? list.at(4) : tr(blankString));
    ui->product_id_info->setText(list.count() > 5 ? list.at(5) : tr(blankString));


}

Settings GSSettingsDialog::gsSettings;
