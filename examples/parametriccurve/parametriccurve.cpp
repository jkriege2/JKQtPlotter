/** \example parametriccurve.cpp
 * Shows how to plot parametric curves from datapoints with JKQTPlotter
 *
 * \ref JKQTPlotterParametricCurves
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"


int main(int argc, char* argv[])
{
        
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    // 1. create two plotter windows that share the same internal datastore (for convenience)
    JKQTPlotter plot, plot2;
    JKQTPDatastore* ds=plot.getDatastore();
    plot2.useExternalDatastore(ds);

    // 2. now we create a vector of x- and y-values of the logarithmic spiral
    //    in addition the radius is stored in column R
    QVector<double> X, Y, R;
    const int Ndata=500; // number of plot points in each curve
    const double phiMax=4.0*JKQTPSTATISTICS_PI;
    const double a=1;
    const double k=0.2;
    for (double phi=-phiMax; phi<=phiMax; phi+=phiMax/double(Ndata)) {
        const double x=a*exp(k*phi)*cos(phi);
        const double y=a*exp(k*phi)*sin(phi);
        X<<x;
        Y<<y;
        R<<sqrt(x*x+y*y);
    }
    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY=ds->addCopiedColumn(Y, "y");
    size_t columnR=ds->addCopiedColumn(R, "r");


    // 3. now we make several graphs. In each one, another property of the graph is controlled by
    //    a datacolumn

    // 3.1 the basic graph
    JKQTPXYLineGraph* graph1=new JKQTPXYLineGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY);
    graph1->setDrawLine(true);
    graph1->setSymbolType(JKQTPNoSymbol);
    graph1->setTitle("one-colored spiral");
    plot.addGraph(graph1);

    // 3.2 for graph2 is the same as graph 1, but in addition, the color of the lines
    //     is set, based on the contents of column R, via the color-palette JKQTPMathImageMATLAB
    JKQTPXYParametrizedScatterGraph* graph2=new JKQTPXYParametrizedScatterGraph(&plot2);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY);
    graph2->setColorColumn(columnR);
    graph2->setColorPalette(JKQTPMathImageMATLAB);
    graph2->setSymbolType(JKQTPNoSymbol);
    graph2->setDrawLine(true);
    graph2->setTitle("colored spiral");
    graph2->getColorBarRightAxis()->setAxisLabel("color scale radius $r(\\phi)$");
    plot2.addGraph(graph2);


    // 4. set the axis scale aspect ratios to 1,
    //    autoscale the plot so the graph is contained
    //    and format the coordinate system and key
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setAspectRatio(1);
    plot.getXAxis()->setAxisLabel("x-axis");
    plot.getYAxis()->setAxisLabel("y-axis");
    plot.getXAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawGrid(false);
    plot.setXY(-15,15,-15,15);
    plot2.getPlotter()->setMaintainAspectRatio(true);
    plot2.getPlotter()->setAspectRatio(1);
    plot2.getXAxis()->setAxisLabel("x-axis");
    plot2.getYAxis()->setAxisLabel("y-axis");
    plot2.getXAxis()->setDrawGrid(false);
    plot2.getYAxis()->setDrawGrid(false);
    plot2.setXY(-15,15,-15,15);


    // 5. show plotters and make them a decent size
    plot.show();
    plot.resize(600,600);
    plot2.show();
    plot2.resize(600,600);

    return app.exec();
}
