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
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);

    MandelbrotMainWindow widMain;

    widMain.show();

    return app.exec();
}
