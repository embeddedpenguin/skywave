#ifndef GROUNDSTATION_H
#define GROUNDSTATION_H

#pragma once
#include <QMainWindow>
#include "gs_headers.h"

class GSCustomPlot;
class GSTerminal;
class GSSettingsDialog;
class GSPlotLayout;

namespace Ui {
class GroundStation;
}

enum Status
{
    Disconnected,
    Connected,
};

class GroundStation : public QMainWindow
{
    Q_OBJECT

public:
    explicit GroundStation(QWidget *parent = 0);
    void CreateNewPlot(QString Name, QString xAxisLabel, QString yAxisLabel);
    void CreateNewPlot(QString Name);
    ~GroundStation();
    int GetNumberOfPlots() const;

private slots:
    void ConnectDevice();
    void DisconnectDevice();
    void UpdateAllPlots(const QList<int> &data);
    void readData();
    void OpenSettings();
private:
    Ui::GroundStation *ui;
    QWidget* gsMain;
    QWidget* gsPlotHub;

    QPointer<QVBoxLayout> gsMainLayout;
    static QPointer<GSPlotLayout> gsPlotHubLayout;

    GSTerminal* gsTerminal;

    //Plots


    QSplitter* vertSplit;

    //currently unused, but will be used
    Status gsStatus = Disconnected;
    QSerialPort* serial;
    GSSettingsDialog* settingsWindow;
    QPointer<QLabel> statusLabel;
    void InitActions();
    int seconds;

    QByteArray serialData;
    QVector<GSCustomPlot*> gsPlotBank;



};

#endif // GROUNDSTATION_H
