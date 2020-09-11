/** \example mandelbrot.cpp
 * Shows how to plot the Mandelbrot set with JKQTPlotter, also providing a zooming feature.
 *
 * \ref JKQTPlotterMandelbrot
 */

#include <QApplication>
#include <cmath>
#include "mandelbrotmainwindow.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MandelbrotMainWindow widMain;

    widMain.show();

    return app.exec();
}
