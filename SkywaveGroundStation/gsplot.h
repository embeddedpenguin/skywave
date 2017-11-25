#ifndef GSPLOT_H
#define GSPLOT_H

#pragma once
#include "qcustomplot.h"
#include "gs_headers.h"


class GSPlot : public QCustomPlot
{
    Q_OBJECT
public:
    void UpdatePlot(qint16 xVal, qint32 yVal);
    GSPlot(QWidget* parent = nullptr);
    ~GSPlot();


private:
    qint32 xAxisRange;
    qint32 yAxisRange;

private slots:
    void makePlot();

public slots:
    //Plots Container





};

#endif // GSPLOT_H
