#include "groundstation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GroundStation groundStation;
    groundStation.CreateNewPlot("Temp", "Time Elapsed", "Temperature in C");
    groundStation.CreateNewPlot("Test2");
    groundStation.CreateNewPlot("Test3");
    groundStation.CreateNewPlot("Test4");



    groundStation.show();

    return a.exec();
}
