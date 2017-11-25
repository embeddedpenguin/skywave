#include "gsplot.h"
GSPlot::GSPlot(QWidget* parent) : QCustomPlot(parent)
{
    xAxisRange = 8;
    yAxisRange = 8;
    yAxis->setRange(0, yAxisRange);
    xAxis->setRange(0, xAxisRange);
    makePlot();
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);


}

GSPlot::~GSPlot()
{
}

void GSPlot::makePlot()
{
    addGraph();
    QPen myPen;
    myPen.setWidth(1.f);
    myPen.setColor(QColor(255, 100, 0));
    graph(0)->setPen(myPen);

}

//eventually replace this with a 2d vector with x and y values for each graph needed to update
void GSPlot::UpdatePlot(qint16 xVal, qint32 yVal)
{
    if(xVal >= xAxisRange)
    {
        xAxisRange = xVal * 2;
        xAxis->setRange(0, xAxisRange);
    }

    if(yVal >= yAxisRange)
    {
        yAxisRange = yVal * 2;
        yAxis->setRange(0, yAxisRange);
    }
    graph(0)->addData(xVal, yVal);
    replot();

}


