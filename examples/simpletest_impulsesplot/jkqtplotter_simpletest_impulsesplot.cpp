/** \example jkqtplotter_simpletest_impulsesplot.cpp
 * Shows how to plot impulse graphs with JKQTPlotter
 *
 * \ref JKQTPlotterImpulsePlots
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsimpulses.h"

// number of datapoints:
#define Ndata 40

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data for a simple plot (a sine curve with lin. increasing errors)
    QVector<double> X, Y;
    for (int i=0; i<Ndata; i++) {
        const double xx=double(i)/double(Ndata)*6.0*M_PI;
        X << xx;
        Y << cos(xx)*exp(-xx/10.0);
    }

    // 3. add data from vectors to the datastore
    size_t columnX=ds->addCopiedColumn(X,  "x");
    size_t columnY=ds->addCopiedColumn(Y,  "y");

    // 4. create a vertical impulse graph in the plot, which plots the dataset X/Y:
    JKQTPImpulsesVerticalGraph* graph=new JKQTPImpulsesVerticalGraph(&plot);
    graph->set_xColumn(columnX);
    graph->set_yColumn(columnY);
    graph->setLineWidth(2);
    graph->set_color(QColor("red"));
    graph->set_title(QObject::tr("$\\cos(x)\\cdot\\exp(-x/10)$"));

    // 5. add the graph to the plot, so it is actually displayed
    plot.addGraph(graph);

    // 6. set some axis properties (we use LaTeX for nice equation rendering)
    plot.getXAxis()->setAxisLabel(QObject::tr("x-axis"));
    plot.getYAxis()->setAxisLabel(QObject::tr("y-axis"));

    // 7. switch the grid off
    plot.getXAxis()->set_drawGrid(false);
    plot.getYAxis()->set_drawGrid(false);

    // 8. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
