#ifndef GSCUSTOMPLOT_H
#define GSCUSTOMPLOT_H

#pragma once
#include <QGroupBox>
#include "gs_headers.h"

class GSPlot;


class GSCustomPlot : public QGroupBox
{
    Q_OBJECT
public:

    GSCustomPlot(QWidget* parent = nullptr);
    GSCustomPlot(QString iName, QWidget* parent = nullptr);
    ~GSCustomPlot();

    static int GetNumberOfPlots() { return numPlots; }
    QString GetName() const { return name; }
    void SetName(QString iName) { name = iName; }
    GSPlot& GetPlot() { return *customPlot; }
private:
    GSPlot* customPlot;
    QHBoxLayout* layout;
    QString name;
    static int numPlots;
    void Init();
};

#endif // GSCUSTOMPLOT_H
