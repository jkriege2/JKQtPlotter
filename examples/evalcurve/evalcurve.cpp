/** \example evalcurve.cpp
 * Shows how to plot Mathematical Functions as Line Graphs with JKQTPlotter (as evaluated C/C++ functions)
 *
 * \ref JKQTPlotterEvalCurves
 */

#include <QApplication>
#include <QVector>
#include <QMap>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpevaluatedparametriccurve.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a window that contains a line-edit to edit a function
    //    and a JKQTPlotter to display the function, combine everything in a layout
    QWidget mainWin;
    JKQTPlotter* plot=new JKQTPlotter(&mainWin);
    QVBoxLayout* layout=new QVBoxLayout;
    mainWin.setLayout(layout);
    layout->addWidget(plot);

    // 2. now we add a JKQTPXYFunctionLineGraph object, which will draw a simple function
    //    the function is defined as C++ inline function
    JKQTPXYFunctionLineGraph* func1=new JKQTPXYFunctionLineGraph(plot);
    func1->setPlotFunctionFunctor([](double t) ->QPointF {
        const double a=5;
        const double b=4;
        const double delta=JKQTPSTATISTICS_PI/4.0;
        return QPointF(sin(a*t+delta), sin(b*t));
    });
    func1->setTRange(0, 2.0*JKQTPSTATISTICS_PI);
    func1->setTitle("C++-inline function $[ sin(5{\\cdot}t+\\pi/4), sin(4{\\cdot}t) ]$");
    plot->addGraph(func1);
    // 8. set some axis properties (we use LaTeX for nice equation rendering)
    plot->getXAxis()->setAxisLabel(QObject::tr("x-axis"));
    plot->getYAxis()->setAxisLabel(QObject::tr("y-axis"));
    plot->getPlotter()->setKeyPosition(JKQTPKeyOutsideBottomLeft);


    // 4. scale the plot so the graph is contained
    plot->setXY(-1.1,1.1,-1.1,1.1);

    // show window and make it a decent size
    mainWin.show();
    mainWin.resize(800,800);

    return app.exec();
}
