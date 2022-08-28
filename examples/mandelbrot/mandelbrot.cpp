/** \example mandelbrot.cpp
 * Shows how to plot the Mandelbrot set with JKQTPlotter, also providing a zooming feature.
 *
 * \ref JKQTPlotterMandelbrot
 */

#include "jkqtpexampleapplication.h"
#include <QApplication>
#include <cmath>
#include "mandelbrotmainwindow.h"



int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv, true);

    MandelbrotMainWindow widMain;
    widMain.connect(&widMain, SIGNAL(readyForScreenshot()), &app, SLOT(notifyReadyForScreenshot()));

    widMain.show();

    return app.exec();
}
