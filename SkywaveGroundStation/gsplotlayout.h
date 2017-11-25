#ifndef GSPLOTLAYOUT_H
#define GSPLOTLAYOUT_H

#include "gs_headers.h"

#define MAX_ROW_COUNT 2
class GSPlotLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit GSPlotLayout(QWidget* parent = nullptr);
    virtual void addWidget(QWidget* widget);

    ~GSPlotLayout();
private:
    QVector<QPointer<QHBoxLayout>> LayoutBank;

    void AddRow(QHBoxLayout* layout);
};

#endif // GSPLOTLAYOUT_H
