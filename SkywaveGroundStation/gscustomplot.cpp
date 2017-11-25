#include "gscustomplot.h"
#include "gsplot.h"


GSCustomPlot::GSCustomPlot(QWidget* parent) : QGroupBox(parent)
{
    name = "Default Name";
    Init();
}

GSCustomPlot::GSCustomPlot(QString iName, QWidget *parent) : QGroupBox(parent)
{
    name = iName;
    Init();
}



GSCustomPlot::~GSCustomPlot()
{
    delete layout;
    --numPlots;
}

int GSCustomPlot::numPlots = 0;

void GSCustomPlot::Init()
{
    layout = new QHBoxLayout;
    customPlot = new GSPlot(this);
    layout->addWidget(customPlot);
    setLayout(layout);
    numPlots++;
    setTitle(name);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

}
