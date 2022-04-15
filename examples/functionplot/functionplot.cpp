/** \example functionplot.cpp
 * Shows how to plot Mathematical Functions as Line Graphs with JKQTPlotter (as evaluated C/C++ functions)
 *
 * \ref JKQTPlotterFunctionPlots
 */

#include <QApplication>
#include <QCheckBox>
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

template <class TFUNCGRAPH>
void drawExample(QApplication& app, const QString& name) {
    // 1.1 create a window that contains a line-edit to edit a function
    //    and a JKQTPlotter to display the function, combine everything in a layout
    QWidget* mainWin=new QWidget();
    mainWin->setWindowTitle(name);
    JKQTPlotter* plot=new JKQTPlotter(mainWin);
    QVBoxLayout* layout=new QVBoxLayout;
    mainWin->setLayout(layout);
    layout->addWidget(plot);
    // 1.2 add checkbox that allows to switch the display of sample points
    QCheckBox* chkShowSamples=new QCheckBox(app.tr("display sample points"));
    chkShowSamples->setChecked(false);
    layout->addWidget(chkShowSamples);

    // 2. now we add a JKQTPXFunctionLineGraph object, which will draw a simple function
    //    the function is defined as C++ inline function
    TFUNCGRAPH* func1=new TFUNCGRAPH(plot);
    func1->setPlotFunctionFunctor([](double x) { return 0.2*x*x-0.015*x*x*x; });
    func1->setTitle("C++-inline function $0.2x^2-0.015x^3$");
    QObject::connect(chkShowSamples, &QCheckBox::toggled, [=](bool en) {
        func1->setDisplaySamplePoints(en);
        plot->redrawPlot();
    });
    plot->addGraph(func1);

    // 3. now we add a JKQTPXFunctionLineGraph object, which will draw a simple function
    //    the function is again defined as C++ inline function, but now uses internal
    //    parameters (handed over to the function as a pointer to QVector<double>
    TFUNCGRAPH* func2=new TFUNCGRAPH(plot);
    func2->setPlotFunctionFunctor([](double x, const QVector<double>& p) {
        return p.at(0)*sin(2.0*JKQTPSTATISTICS_PI*x*p.at(1));
    });
    // here we set the parameters p0, p1
    func2->setParamsV(5, 0.2);
    func2->setTitle("C++-inline function with int. params $p_0\\cdot\\sin(x*2.0*\\pi\\cdot p_1)$");
    QObject::connect(chkShowSamples, &QCheckBox::toggled, [=](bool en) {
        func2->setDisplaySamplePoints(en);
        plot->redrawPlot();
    });
    plot->addGraph(func2);

    // 4. of course the function may also be any C+ funtor object:
    TFUNCGRAPH* func4=new TFUNCGRAPH(plot);
    func4->setPlotFunctionFunctor(SincSqr(-8));
    func4->setTitle("C++ functor $-8*\\sin^2(x)/x^2$");
    QObject::connect(chkShowSamples, &QCheckBox::toggled, [=](bool en) {
        func4->setDisplaySamplePoints(en);
        plot->redrawPlot();
    });
    plot->addGraph(func4);


    // 5. now we use a JKQTPXFunctionLineGraph to draw a static C function
    TFUNCGRAPH* func5=new TFUNCGRAPH(plot);
    func5->setPlotFunctionFunctor(&sinc);
    func5->setTitle("static C function $10*\\sin(x)/x$");
    QObject::connect(chkShowSamples, &QCheckBox::toggled, [=](bool en) {
        func5->setDisplaySamplePoints(en);
        plot->redrawPlot();
    });
    plot->addGraph(func5);

    // 6. finally JKQTPXFunctionLineGraph defines a small set of common functions
    TFUNCGRAPH* func6=new TFUNCGRAPH(plot);
    func6->setSpecialFunction(TFUNCGRAPH::Line);
    // here we set offset p0=-1 and slope p1=1.5 of the line p0+p1*x
    func6->setParamsV(-1,1.5);
    func6->setTitle("special function: linear p_0=-1, p_1=1.5");
    QObject::connect(chkShowSamples, &QCheckBox::toggled, [=](bool en) {
        func6->setDisplaySamplePoints(en);
        plot->redrawPlot();
    });
    plot->addGraph(func6);

    // 7. finally JKQTPXFunctionLineGraph defines a small set of common functions
    TFUNCGRAPH* func7=new TFUNCGRAPH(plot);
    func7->setSpecialFunction(TFUNCGRAPH::Line);
    // here we set offset p0=1 and slope p1=-1.5 of the line p0+p1*x by adding these into a column
    // in the internal datastore and then set that column as parameterColumn for the function graph
    QVector<double> params;
    params << /*p0=*/1 << /*p1=*/-1.5;
    size_t paramCol=plot->getDatastore()->addCopiedColumn(params);
    func7->setParameterColumn(paramCol);
    func7->setTitle("special function: linear p_0=1, p_1=-1.5");
    QObject::connect(chkShowSamples, &QCheckBox::toggled, [=](bool en) {
        func7->setDisplaySamplePoints(en);
        plot->redrawPlot();
    });
    plot->addGraph(func7);


    // 8. set some axis properties (we use LaTeX for nice equation rendering)
    plot->getXAxis()->setAxisLabel(QObject::tr("x-axis"));
    plot->getYAxis()->setAxisLabel(QObject::tr("y-axis"));
    plot->getPlotter()->setKeyPosition(JKQTPKeyOutsideBottomLeft);


    // 9. scale the plot so the graph is contained
    plot->setXY(-10,10,-10,10);
    plot->redrawPlot();

    // show window and make it a decent size
    mainWin->show();
    mainWin->resize(800,800);
}

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    drawExample<JKQTPXFunctionLineGraph>(app, "functionplot: JKQTPXFunctionLineGraph");
    drawExample<JKQTPYFunctionLineGraph>(app, "functionplot: JKQTPYFunctionLineGraph");


    return app.exec();
}
