/** \example stepplots.cpp
 * JKQTPlotter: Examples: Special/Step Line Plots in Different Styles
 *
 * \ref JKQTPlotterSpecialStepLinePlot
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpspecialline.h"
#include "jkqtplotter/jkqtptools.h"

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data a vector of x-values for a simple plot and several sets of y-values for
    //    cosine curves stacked above each other
    QVector<double> X, Y1, Y2, Y3, Y4, Y5;
    const int Ndata=7; // number of plot points in each curve
    for (int i=0; i<=Ndata; i++) {
        const double x=0.0+double(i)*0.6*JKQTPSTATISTICS_PI;
        X<<x;
        Y1<<sin(x)+1.0*1.5;
        Y2<<sin(x)+3.0*1.5;
        Y3<<sin(x)+5.0*1.5;
        Y4<<sin(x)+7.0*1.5;
        Y5<<sin(x)+9.0*1.5;
    }

    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY1=ds->addCopiedColumn(Y1, "y1");
    size_t columnY2=ds->addCopiedColumn(Y2, "y2");
    size_t columnY3=ds->addCopiedColumn(Y3, "y3");
    size_t columnY4=ds->addCopiedColumn(Y4, "y5");
    size_t columnY5=ds->addCopiedColumn(Y5, "y4");

    // 3 now we make several plots with different step styles, each one also contains a
    //     symbol plot indicating the location of the datapoints themselves
    JKQTPSpecialLineHorizontalGraph* graph;

    //-- JKQTPStepLeft ----------------------------------------------------------------------------------------
    graph=new JKQTPSpecialLineHorizontalGraph(&plot);

    // set data for the graph
    graph->setXColumn(columnX);
    graph->setYColumn(columnY1);

    // set step style
    graph->setSpecialLineType(JKQTPStepLeft);
    graph->setLineWidth(1);
    graph->setFillCurve(true);
    /*QRadialGradient radialGrad(QPointF(0.5, 0.5), 0.5);
    radialGrad.setColorAt(0, Qt::red);
    radialGrad.setColorAt(0.5, Qt::blue);
    radialGrad.setColorAt(1, Qt::green);
    radialGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    graph->setFillGradient(radialGrad);*/
    graph->setDrawLine(true);
    graph->setTitle("JKQTPStepLeft, filled");
    graph->setBaseline(0.0);

    // enable symbols
    graph->setDrawSymbols(true);
    graph->setSymbolType(JKQTPGraphSymbols::JKQTPCircle);

    // add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);


    //-- JKQTPStepCenter ----------------------------------------------------------------------------------------
    graph=new JKQTPSpecialLineHorizontalGraph(&plot);

    // set data for the graph
    graph->setXColumn(columnX);
    graph->setYColumn(columnY2);

    // set step style
    graph->setSpecialLineType(JKQTPStepCenter);
    graph->setLineWidth(1);
    graph->setFillCurve(false);
    graph->setDrawLine(true);
    graph->setTitle("JKQTPStepCenter");

    // enable symbols
    graph->setDrawSymbols(true);
    graph->setSymbolType(JKQTPGraphSymbols::JKQTPCircle);


    // add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);


    //-- JKQTPStepRight ----------------------------------------------------------------------------------------
    graph=new JKQTPSpecialLineHorizontalGraph(&plot);

    // set data for the graph
    graph->setXColumn(columnX);
    graph->setYColumn(columnY3);

    // set step style
    graph->setSpecialLineType(JKQTPStepRight);
    graph->setLineWidth(1);
    graph->setFillCurve(false);
    graph->setDrawLine(true);
    graph->setTitle("JKQTPStepRight");

    // enable symbols
    graph->setDrawSymbols(true);
    graph->setSymbolType(JKQTPGraphSymbols::JKQTPCircle);

    // add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);



    //-- JKQTPStepAverage ----------------------------------------------------------------------------------------
    graph=new JKQTPSpecialLineHorizontalGraph(&plot);

    // set data for the graph
    graph->setXColumn(columnX);
    graph->setYColumn(columnY4);

    // set step style
    graph->setSpecialLineType(JKQTPStepAverage);
    graph->setLineWidth(1);
    graph->setFillCurve(false);
    graph->setDrawLine(true);
    graph->setTitle("JKQTPStepAverage");

    // enable symbols
    graph->setDrawSymbols(true);
    graph->setSymbolType(JKQTPGraphSymbols::JKQTPCircle);

    // add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);


    //-- JKQTPDirectLine ----------------------------------------------------------------------------------------
    graph=new JKQTPSpecialLineHorizontalGraph(&plot);

    // set data for the graph
    graph->setXColumn(columnX);
    graph->setYColumn(columnY5);

    // set step style
    graph->setSpecialLineType(JKQTPDirectLine);
    graph->setLineWidth(1);
    graph->setFillCurve(false);
    graph->setDrawLine(true);
    graph->setTitle("JKQTPDirectLine");

    // enable symbols
    graph->setDrawSymbols(true);
    graph->setSymbolType(JKQTPGraphSymbols::JKQTPCircle);



    // add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);


    // 4. scale plot manually
    plot.setXY(-0.5, X.last()+0.5, -0.5, 18);

    // 5. style  the plot
    plot.setGrid(false);
    plot.getXAxis()->setShowZeroAxis(false);

    // 6. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);

    return app.exec();
}
