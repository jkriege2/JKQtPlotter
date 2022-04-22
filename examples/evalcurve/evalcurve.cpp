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
        
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
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
        return QPointF(
              sin(t)*(exp(cos(t))-2.0*cos(4.0*t)-jkqtp_pow5(sin(t/12.0))),
              cos(t)*(exp(cos(t))-2.0*cos(4.0*t)-jkqtp_pow5(sin(t/12.0)))
            );
    });
    func1->setTRange(0, 12.0*JKQTPSTATISTICS_PI);
    func1->setTitle("C++-inline function: \"Butterfly Curve\"");
    plot->addGraph(func1);



    // 3. a second JKQTPXYFunctionLineGraph object shows how to use functions that
    //    also take a parameter vector, in addition to the dependent variable t
    JKQTPXYFunctionLineGraph* func2=new JKQTPXYFunctionLineGraph(plot);
    func2->setPlotFunctionFunctor([](double t, const QVector<double>& params) ->QPointF {
        return QPointF(
              3.0*sin(params[0]*t+params[2])+8.0,
              3.0*sin(params[1]*t)
            );
    });
    // now we define the 3 parameters of the function
    func2->setParamsV(5, 4, JKQTPSTATISTICS_PI/4.0);
    // and define the range over which to evaluate
    func2->setTRange(0, 2.0*JKQTPSTATISTICS_PI);
    func2->setTitle("C++-inline function $[ sin(5{\\cdot}t+\\pi/4), sin(4{\\cdot}t) ]$");
    plot->addGraph(func2);


    // 8. set some axis properties (we use LaTeX for nice equation rendering)
    plot->getXAxis()->setAxisLabel(QObject::tr("x-axis"));
    plot->getYAxis()->setAxisLabel(QObject::tr("y-axis"));
    plot->getPlotter()->setKeyPosition(JKQTPKeyOutsideBottomLeft);


    // 4. scale the plot so the graph is contained
    plot->setXY(-3,12,-3.2,3.2);

    // show window and make it a decent size
    mainWin.show();
    mainWin.resize(800,800);

    return app.exec();
}
