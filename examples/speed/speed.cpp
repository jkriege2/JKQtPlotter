#include <QApplication>
#include <array>
#include <random>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "speedtestplot.h"
#include "jkqtpexampleapplication.h"

int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    SpeedTestPlot plot;

    QTimer::singleShot(500, &plot, SLOT(plotNewData()));

    return app.exec();
}
