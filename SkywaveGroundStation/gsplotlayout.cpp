#include "gsplotlayout.h"
#include "gscustomplot.h"

GSPlotLayout::GSPlotLayout(QWidget *parent) : QVBoxLayout(parent)
{
    AddRow(new QHBoxLayout);


}

void GSPlotLayout::addWidget(QWidget *widget)
{
    if(LayoutBank.last()->count() >= MAX_ROW_COUNT)
    {
        AddRow(new QHBoxLayout);
        LayoutBank.last()->addWidget(widget);
    }
    else
    {
        LayoutBank.last()->addWidget(widget);
    }
}

GSPlotLayout::~GSPlotLayout()
{

}

void GSPlotLayout::AddRow(QHBoxLayout *layout)
{
    LayoutBank.append(layout);
    addLayout(layout);
}




