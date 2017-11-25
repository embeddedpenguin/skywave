#ifndef GSSETTINGSDIALOG_H
#define GSSETTINGSDIALOG_H

#pragma once
#include <QDialog>
#include "gs_headers.h"

struct Settings
{
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::FlowControl flowControl;
    QSerialPort::StopBits stopBits;

    Settings()
    {
        name = "COM1";
        baudRate = 9600;
        dataBits = QSerialPort::Data8;
        parity = QSerialPort::NoParity;
        stopBits = QSerialPort::OneStop;
        flowControl = QSerialPort::NoFlowControl;
    }
};

namespace Ui {
class GSSettingsDialog;
}

class GSSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GSSettingsDialog(QWidget *parent = 0);
    ~GSSettingsDialog();
    static Settings& GetSettings();
    static void UpdatePortName(QString iString);

private:
    Ui::GSSettingsDialog *ui;
    static Settings gsSettings;
    void ApplySettings(const Settings &iSettings);
    void FillParameters();
    void FillPortsInfo();
    void UpdateSettings();

private slots:
    void ApplyUpdates();
    void ShowPortInfo(int ID);
};

#endif // GSSETTINGSDIALOG_H
