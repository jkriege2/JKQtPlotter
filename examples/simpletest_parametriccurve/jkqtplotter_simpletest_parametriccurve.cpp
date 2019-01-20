/** \example jkqtplotter_simpletest_parametriccurve.cpp
 * Shows how to plot parametric curves from datapoints with JKQTPlotter
 *
 * \ref JKQTPlotterParametricCurves
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create two plotter windows that share the same internal datastore (for convenience)
    JKQTPlotter plot, plot2;
    JKQTPDatastore* ds=plot.getDatastore();
    plot2.useExternalDatastore(ds);

    // 2. now we create a vector of x- and y-values of the logarithmic spiral
    //    in addition the radius is stored in column R
    QVector<double> X, Y, R;
    const int Ndata=500; // number of plot points in each curve
    const double phiMax=4.0*M_PI;
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
    graph1->set_xColumn(columnX);
    graph1->set_yColumn(columnY);
    graph1->set_drawLine(true);
    graph1->set_symbol(JKQTPNoSymbol);
    graph1->set_title("one-colored spiral");
    plot.addGraph(graph1);

    // 3.2 for graph2 is the same as graph 1, but in addition, the color of the lines
    //     is set, based on the contents of column R, via the color-palette JKQTPMathImageMATLAB
    JKQTPXYParametrizedScatterGraph* graph2=new JKQTPXYParametrizedScatterGraph(&plot2);
    graph2->set_xColumn(columnX);
    graph2->set_yColumn(columnY);
    graph2->set_colorColumn(columnR);
    graph2->set_palette(JKQTPMathImageMATLAB);
    graph2->set_symbol(JKQTPNoSymbol);
    graph2->set_drawLine(true);
    graph2->set_title("colored spiral");
    graph2->get_colorBarRightAxis()->set_axisLabel("color scale radius $r(\\phi)$");
    plot2.addGraph(graph2);


    // 4. set the axis scale aspect ratios to 1,
    //    autoscale the plot so the graph is contained
    //    and format the coordinate system and key
    plot.get_plotter()->set_maintainAspectRatio(true);
    plot.get_plotter()->set_aspectRatio(1);
    plot.get_xAxis()->set_axisLabel("x-axis");
    plot.get_yAxis()->set_axisLabel("y-axis");
    plot.get_xAxis()->set_drawGrid(false);
    plot.get_yAxis()->set_drawGrid(false);
    plot.setXY(-15,15,-15,15);
    plot2.get_plotter()->set_maintainAspectRatio(true);
    plot2.get_plotter()->set_aspectRatio(1);
    plot2.get_xAxis()->set_axisLabel("x-axis");
    plot2.get_yAxis()->set_axisLabel("y-axis");
    plot2.get_xAxis()->set_drawGrid(false);
    plot2.get_yAxis()->set_drawGrid(false);
    plot2.setXY(-15,15,-15,15);


    // 5. show plotters and make them a decent size
    plot.show();
    plot.resize(600,600);
    plot2.show();
    plot2.resize(600,600);

    return app.exec();
}
