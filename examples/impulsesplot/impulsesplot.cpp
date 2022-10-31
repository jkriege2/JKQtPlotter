/** \example impulsesplot.cpp
 * Shows how to plot impulse graphs with JKQTPlotter
 *
 * \ref JKQTPlotterImpulsePlots
 */

#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpimpulses.h"

// number of datapoints:

template<class TGRAPH>
TGRAPH* drawPlot(JKQTPlotter& plot, int NData=40, double baseline=0.0) {
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data for a simple plot (a sine curve with lin. increasing errors)
    QVector<double> X, Y, E;
    for (int i=0; i<NData; i++) {
        const double xx=double(i)/40.0*8.0*JKQTPSTATISTICS_PI;
        X << xx;
        Y << cos(xx)*exp(-xx/10.0);
        E << 0.2*Y.last();
    }

    // 3. add data from vectors to the datastore
    size_t columnX=ds->addCopiedColumn(X,  "x");
    size_t columnY=ds->addCopiedColumn(Y,  "y");
    size_t columnE=ds->addCopiedColumn(E,  "e");

    // 4. create a vertical impulse graph in the plot, which plots the dataset X/Y:
    TGRAPH* graph=new TGRAPH(&plot);
    graph->setKeyColumn(columnX);
    graph->setValueColumn(columnY);
    graph->setLineWidth(2);
    graph->setBaseline(baseline);
    graph->setColor(QColor("red"));
    graph->setTitle(QObject::tr("$\\cos(x)\\cdot\\exp(-x/10)$"));
    if (dynamic_cast<JKQTPYGraphErrors*>(graph)) {
        dynamic_cast<JKQTPYGraphErrors*>(graph)->setYErrorColumn(columnE);
        dynamic_cast<JKQTPYGraphErrors*>(graph)->setYErrorStyle(JKQTPErrorBars);
    }
    if (dynamic_cast<JKQTPXGraphErrors*>(graph)) {
        dynamic_cast<JKQTPXGraphErrors*>(graph)->setXErrorColumn(columnE);
        dynamic_cast<JKQTPXGraphErrors*>(graph)->setXErrorStyle(JKQTPErrorBars);
    }
    //graph->setDrawSymbols(true);
    //graph->setSymbolType(JKQTPGraphSymbols::JKQTPFilledStar);
    //graph->setBaseline(0.25);

    // 5. add the graph to the plot, so it is actually displayed
    plot.addGraph(graph);

    // 6. set some axis properties (we use LaTeX for nice equation rendering)
    plot.getXAxis()->setAxisLabel(QObject::tr("x-axis"));
    plot.getYAxis()->setAxisLabel(QObject::tr("y-axis"));

    // 7. switch the grid off
    plot.getXAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawGrid(false);

    // 8. autoscale the plot so the graph is contained
    plot.zoomToFit();
    plot.getPlotter()->setShowKey(false);

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600/plot.devicePixelRatioF(),400/plot.devicePixelRatioF());

    return graph;
}

int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    auto grV=drawPlot<JKQTPImpulsesVerticalGraph>(plot);
    plot.setWindowTitle("1: JKQTPImpulsesVerticalGraph");

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plotS;
    auto grS=drawPlot<JKQTPImpulsesVerticalGraph>(plotS, 25);
    plotS.setWindowTitle("2: JKQTPImpulsesVerticalGraph + Symbols");
    grS->setDrawSymbols(true);
    grS->setSymbolType(JKQTPFilledStar);
    grS->setSymbolFillColor(QColor("cornflowerblue"));
    grS->setSymbolColor(QColor("blue"));
    grS->setSymbolSize(12);

    JKQTPlotter plotH;
    auto grH=drawPlot<JKQTPImpulsesHorizontalGraph>(plotH);
    plotH.setWindowTitle("3: JKQTPImpulsesHorizontalGraph");
    plotH.resize(400/plot.devicePixelRatioF(),600/plot.devicePixelRatioF());

    JKQTPlotter plotE;
    auto grE=drawPlot<JKQTPImpulsesVerticalErrorGraph>(plotE,10);
    plotE.setWindowTitle("4: JKQTPImpulsesVerticalErrorGraph");
    grE->setLineWidth(5);

    JKQTPlotter plotEH;
    auto grEH=drawPlot<JKQTPImpulsesHorizontalErrorGraph>(plotEH,10);
    plotEH.setWindowTitle("5: JKQTPImpulsesHorizontalErrorGraph");
    grEH->setLineWidth(4);
    plotEH.resize(400/plot.devicePixelRatioF(),600/plot.devicePixelRatioF());

    app.addExportStepFunctor([&]() {
        grV->setDrawBaseline(false);
        grS->setDrawBaseline(false);
        grH->setDrawBaseline(false);
        grE->setDrawBaseline(false);
        grEH->setDrawBaseline(false);

        plot.getXAxis()->setShowZeroAxis(false);
        plot.getYAxis()->setShowZeroAxis(false);

        plotE.getXAxis()->setShowZeroAxis(false);
        plotE.getYAxis()->setShowZeroAxis(false);

        plotEH.getXAxis()->setShowZeroAxis(false);
        plotEH.getYAxis()->setShowZeroAxis(false);

        plotH.getXAxis()->setShowZeroAxis(false);
        plotH.getYAxis()->setShowZeroAxis(false);

        plotS.getXAxis()->setShowZeroAxis(false);
        plotS.getYAxis()->setShowZeroAxis(false);
    });
    app.addExportStepFunctor([&]() {
        grV->setDrawBaseline(true);
        grS->setDrawBaseline(true);
        grH->setDrawBaseline(true);
        grE->setDrawBaseline(true);
        grEH->setDrawBaseline(true);
    });

    return app.exec();
}
