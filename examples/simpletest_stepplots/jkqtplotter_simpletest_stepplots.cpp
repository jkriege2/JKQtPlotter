/** \example jkqtplotter_simpletest_stepplots.cpp
 * JKQTPlotter: Examples: Step Line Plots in Different Styles
 *
 * \ref JKQTPlotterStepPlot
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplottertools/jkqtptools.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data a vector of x-values for a simple plot and several sets of y-values for
    //    cosine curves stacked above each other
    QVector<double> X, Y1, Y2, Y3;
    const int Ndata=10; // number of plot points in each curve
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*2.0*M_PI;
        X<<x;
        Y1<<cos(x)+1.0*1.5;
        Y2<<cos(x)+2.0*1.5;
        Y3<<cos(x)+3.0*1.5;
    }
    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY1=ds->addCopiedColumn(Y1, "y1");
    size_t columnY2=ds->addCopiedColumn(Y2, "y2");
    size_t columnY3=ds->addCopiedColumn(Y3, "y3");

    // 3 now we make several plots with different step styles, each one also contains a
    //     symbol plot indicating the location of the datapoints themselves
    JKQTPStepHorizontalGraph* graph;
    JKQTPXYLineGraph* graphL;

    //-- JKQTPStepLeft ----------------------------------------------------------------------------------------
    graph=new JKQTPStepHorizontalGraph(&plot);
    graphL=new JKQTPXYLineGraph(&plot);

    // set data for both graphs
    graph->setXColumn(columnX); graphL->setXColumn(graph->getXColumn());
    graph->setYColumn(columnY1); graphL->setYColumn(graph->getYColumn());

    // set step style
    graph->setStepType(JKQTPStepLeft);
    graph->setLineWidth(1);
    graph->setFillCurve(true);
    graph->setDrawLine(true);
    graph->setTitle("JKQTPStepLeft, filled");

    // set symbol + pen style and color for the added circles, copy color
    graphL->setSymbol(JKQTPGraphSymbols::JKQTPCircle);
    graphL->setDrawLine(false);
    graphL->setSymbolSize(9);
    graphL->setSymbolWidth(1);
    graphL->setColor(graph->getColor());

    // add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);
    plot.addGraph(graphL);


    //-- JKQTPStepCenter ----------------------------------------------------------------------------------------
    graph=new JKQTPStepHorizontalGraph(&plot);
    graphL=new JKQTPXYLineGraph(&plot);

    // set data for both graphs
    graph->setXColumn(columnX); graphL->setXColumn(graph->getXColumn());
    graph->setYColumn(columnY2); graphL->setYColumn(graph->getYColumn());

    // set step style
    graph->setStepType(JKQTPStepCenter);
    graph->setLineWidth(1);
    graph->setFillCurve(false);
    graph->setDrawLine(true);
    graph->setTitle("JKQTPStepCenter");

    // set symbol + pen style and color for the added circles, copy color
    graphL->setSymbol(JKQTPGraphSymbols::JKQTPCircle);
    graphL->setDrawLine(false);
    graphL->setSymbolSize(9);
    graphL->setSymbolWidth(1);
    graphL->setColor(graph->getColor());

    // add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);
    plot.addGraph(graphL);


    //-- JKQTPStepRight ----------------------------------------------------------------------------------------
    graph=new JKQTPStepHorizontalGraph(&plot);
    graphL=new JKQTPXYLineGraph(&plot);

    // set data for both graphs
    graph->setXColumn(columnX); graphL->setXColumn(graph->getXColumn());
    graph->setYColumn(columnY3); graphL->setYColumn(graph->getYColumn());

    // set step style
    graph->setStepType(JKQTPStepRight);
    graph->setLineWidth(1);
    graph->setFillCurve(false);
    graph->setDrawLine(true);
    graph->setTitle("JKQTPStepRight");

    // set symbol + pen style and color for the added circles, copy color
    graphL->setSymbol(JKQTPGraphSymbols::JKQTPCircle);
    graphL->setDrawLine(false);
    graphL->setSymbolSize(9);
    graphL->setSymbolWidth(1);
    graphL->setColor(graph->getColor());

    // add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);
    plot.addGraph(graphL);




    // 4. scale plot manually
    plot.setXY(-0.5, X.last()+0.5, -0.5, 7);

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);

    return app.exec();
}
