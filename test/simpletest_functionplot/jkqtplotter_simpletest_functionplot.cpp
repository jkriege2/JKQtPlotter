#include <QApplication>
#include <QVector>
#include <QMap>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsevaluatedfunction.h"

double sinc(double x, void* /*params*/) {
    return 10.0*sin(x)/x;
}

struct SincSqr {
public:
    inline SincSqr(double amplitude): a(amplitude) {}
    inline double operator()(double x, void* /*params*/) {
        return a*sin(x)*sin(x)/x/x;
    }
private:
    double a;
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a window that conatins a line-edit to edit a function
    //    and a JKQtPlotter to display the function, combine everything in a layout
    QWidget mainWin;
    JKQtPlotter* plot=new JKQtPlotter(&mainWin);
    QVBoxLayout* layout=new QVBoxLayout;
    mainWin.setLayout(layout);
    layout->addWidget(plot);

    // 2. now we add a JKQTPxFunctionLineGraph object, which will draw a simple function
    //    the function is defined as C++ inline function
    JKQTPxFunctionLineGraph* func1=new JKQTPxFunctionLineGraph(plot);
    func1->set_plotFunction([](double x, void* /*params*/) { return 0.2*x*x-0.015*x*x*x; });
    func1->set_title("C++-inline function $0.2x^2-0.015x^3$");
    plot->addGraph(func1);

    // 3. now we add a JKQTPxFunctionLineGraph object, which will draw a simple function
    //    the function is again defined as C++ inline function, but now uses internal
    //    parameters (handed over to the function as a pointer to QVector<double>
    JKQTPxFunctionLineGraph* func2=new JKQTPxFunctionLineGraph(plot);
    func2->set_plotFunction([](double x, void* params) {
        QVector<double>* p=static_cast<QVector<double>*>(params);
        return p->at(0)*sin(2.0*M_PI*x*p->at(1));
    });
    // here we set the parameters p0, p1
    func2->set_paramsV(5, 0.2);
    func2->set_title("C++-inline function with int. params $p_0\\cdot\\sin(x*2.0*\\pi\\cdot p_1)$");
    plot->addGraph(func2);

    // 4. now we add a JKQTPxFunctionLineGraph object, which will draw a simple function
    //    the function is again defined as C++ inline function, but now uses external
    //    parameters, which may have any type (here QMap<QString,double)
    JKQTPxFunctionLineGraph* func3=new JKQTPxFunctionLineGraph(plot);
    func3->set_plotFunction([](double x, void* params) {
        QMap<QString,double>* p=static_cast<QMap<QString,double>*>(params);
        return p->value("amplitude")*sin(2.0*M_PI*x*p->value("frequency"));
    });
    // here we set the parameters p0, p1
    QMap<QString,double> params3;
    params3["amplitude"]=-3;
    params3["frequency"]=0.3;
    func3->set_params(&params3);
    func3->set_title("C++-inline function with ext. params $p_0\\cdot\\sin(x*2.0*\\pi\\cdot p_1)$");
    plot->addGraph(func3);

    // 5. of course the function may also be any C+ funtor object:
    JKQTPxFunctionLineGraph* func4=new JKQTPxFunctionLineGraph(plot);
    func4->set_plotFunction(SincSqr(-8));
    func4->set_title("C++ functor $-8*\\sin^2(x)/x^2$");
    plot->addGraph(func4);


    // 6. now we use a JKQTPxFunctionLineGraph to draw a static C function
    JKQTPxFunctionLineGraph* func5=new JKQTPxFunctionLineGraph(plot);
    func5->set_plotFunction(&sinc);
    func5->set_title("static C function $10*\\sin(x)/x$");
    plot->addGraph(func5);

    // 6. finally JKQTPxFunctionLineGraph defines a small set of common functions
    JKQTPxFunctionLineGraph* func6=new JKQTPxFunctionLineGraph(plot);
    func6->setSpecialFunction(JKQTPxFunctionLineGraph::Line);
    // here we set offset and slope of the line
    func6->set_paramsV(-1,1.5);
    func6->set_title("special function: linear");
    plot->addGraph(func6);


    // 8. set some axis properties (we use LaTeX for nice equation rendering)
    plot->getXAxis()->set_axisLabel(QObject::tr("x-axis"));
    plot->getYAxis()->set_axisLabel(QObject::tr("y-axis"));
    plot->get_plotter()->set_keyPosition(JKQTPkeyOutsideBottomLeft);


    // 4. scale the plot so the graph is contained
    plot->setXY(-10,10,-10,10);

    // show window and make it a decent size
    mainWin.show();
    mainWin.resize(800,800);

    return app.exec();
}
