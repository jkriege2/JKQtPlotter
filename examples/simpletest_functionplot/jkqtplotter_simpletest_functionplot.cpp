/** \example jkqtplotter_simpletest_functionplot.cpp
 * Shows how to plot Mathematical Functions as Line Graphs with JKQTPlotter (as evaluated C/C++ functions)
 *
 * \ref JKQTPlotterFunctionPlots
 */

#include <QApplication>
#include <QVector>
#include <QMap>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpevaluatedfunction.h"

double sinc(double x) {
    return 10.0*sin(x)/x;
}

struct SincSqr {
public:
    inline SincSqr(double amplitude): a(amplitude) {}
    inline double operator()(double x) {
        return a*sin(x)*sin(x)/x/x;
    }
private:
    double a;
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a window that conatins a line-edit to edit a function
    //    and a JKQTPlotter to display the function, combine everything in a layout
    QWidget mainWin;
    JKQTPlotter* plot=new JKQTPlotter(&mainWin);
    QVBoxLayout* layout=new QVBoxLayout;
    mainWin.setLayout(layout);
    layout->addWidget(plot);

    // 2. now we add a JKQTPXFunctionLineGraph object, which will draw a simple function
    //    the function is defined as C++ inline function
    JKQTPXFunctionLineGraph* func1=new JKQTPXFunctionLineGraph(plot);
    func1->setPlotFunctionFunctor([](double x) { return 0.2*x*x-0.015*x*x*x; });
    func1->setTitle("C++-inline function $0.2x^2-0.015x^3$");
    plot->addGraph(func1);

    // 3. now we add a JKQTPXFunctionLineGraph object, which will draw a simple function
    //    the function is again defined as C++ inline function, but now uses internal
    //    parameters (handed over to the function as a pointer to QVector<double>
    JKQTPXFunctionLineGraph* func2=new JKQTPXFunctionLineGraph(plot);
    func2->setPlotFunctionFunctor([](double x, void* params) {
        QVector<double>* p=static_cast<QVector<double>*>(params);
        return p->at(0)*sin(2.0*M_PI*x*p->at(1));
    });
    // here we set the parameters p0, p1
    func2->setParamsV(5, 0.2);
    func2->setTitle("C++-inline function with int. params $p_0\\cdot\\sin(x*2.0*\\pi\\cdot p_1)$");
    plot->addGraph(func2);

    // 4. now we add a JKQTPXFunctionLineGraph object, which will draw a simple function
    //    the function is again defined as C++ inline function, but now uses external
    //    parameters, which may have any type (here QMap<QString,double)
    JKQTPXFunctionLineGraph* func3=new JKQTPXFunctionLineGraph(plot);
    func3->setPlotFunctionFunctor([](double x, void* params) {
        QMap<QString,double>* p=static_cast<QMap<QString,double>*>(params);
        return p->value("amplitude")*sin(2.0*M_PI*x*p->value("frequency"));
    });
    // here we set the parameters p0, p1
    QMap<QString,double> params3;
    params3["amplitude"]=-3;
    params3["frequency"]=0.3;
    func3->setParams(&params3);
    func3->setTitle("C++-inline function with ext. params $p_0\\cdot\\sin(x*2.0*\\pi\\cdot p_1)$");
    plot->addGraph(func3);

    // 5. of course the function may also be any C+ funtor object:
    JKQTPXFunctionLineGraph* func4=new JKQTPXFunctionLineGraph(plot);
    func4->setPlotFunctionFunctor(SincSqr(-8));
    func4->setTitle("C++ functor $-8*\\sin^2(x)/x^2$");
    plot->addGraph(func4);


    // 6. now we use a JKQTPXFunctionLineGraph to draw a static C function
    JKQTPXFunctionLineGraph* func5=new JKQTPXFunctionLineGraph(plot);
    func5->setPlotFunctionFunctor(&sinc);
    func5->setTitle("static C function $10*\\sin(x)/x$");
    plot->addGraph(func5);

    // 7. finally JKQTPXFunctionLineGraph defines a small set of common functions
    JKQTPXFunctionLineGraph* func6=new JKQTPXFunctionLineGraph(plot);
    func6->setSpecialFunction(JKQTPXFunctionLineGraph::Line);
    // here we set offset p0=-1 and slope p1=1.5 of the line p0+p1*x
    func6->setParamsV(-1,1.5);
    func6->setTitle("special function: linear p_0=-1, p_1=1.5");
    plot->addGraph(func6);

    // 7. finally JKQTPXFunctionLineGraph defines a small set of common functions
    JKQTPXFunctionLineGraph* func7=new JKQTPXFunctionLineGraph(plot);
    func7->setSpecialFunction(JKQTPXFunctionLineGraph::Line);
    // here we set offset p0=1 and slope p1=-1.5 of the line p0+p1*x by adding these into a column
    // in the internal datastore and then set that column as parameterColumn for the function graph
    QVector<double> params;
    params << /*p0=*/1 << /*p1=*/-1.5;
    size_t paramCol=plot->getDatastore()->addCopiedColumn(params);
    func7->setParameterColumn(paramCol);
    func7->setTitle("special function: linear p_0=1, p_1=-1.5");
    plot->addGraph(func7);


    // 8. set some axis properties (we use LaTeX for nice equation rendering)
    plot->getXAxis()->setAxisLabel(QObject::tr("x-axis"));
    plot->getYAxis()->setAxisLabel(QObject::tr("y-axis"));
    plot->getPlotter()->setKeyPosition(JKQTPKeyOutsideBottomLeft);


    // 4. scale the plot so the graph is contained
    plot->setXY(-10,10,-10,10);

    // show window and make it a decent size
    mainWin.show();
    mainWin.resize(800,800);

    return app.exec();
}
