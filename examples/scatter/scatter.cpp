/** \example scatter.cpp
 * A very basic example for the usage of JKQTPScatterGraphh
 *
 * \ref JKQTPlotterscatter
 */
 
#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtpexampleapplication.h"
#include <random>

void createScatterGraph(JKQTPlotter&plot) {
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. add two columns to the JKQTPDatastore and obtain back-inserter iterators for these
    size_t columnX=ds->addColumn("x");
    auto colXInserter=ds->backInserter(columnX);
    size_t columnY=ds->addColumn("y");
    auto colYInserter=ds->backInserter(columnY);

    // 3. now we create data for a simple plot (a sine curve with random noise)
    std::default_random_engine generator(123);
    std::normal_distribution<double> distribution(0,0.5);
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        // put data
        const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
        *colXInserter=x;
        *colYInserter=sin(x)+distribution(generator);
        // advance back-inserters
        colXInserter++;
        colYInserter++;
    }


    // 4. create a graph in the plot, which plots the dataset X/Y:
    JKQTPXYScatterGraph* graph1=new JKQTPXYScatterGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY);
    graph1->setTitle(QObject::tr("sine graph"));

    // 5. add the graph to the plot, so it is actually displayed
    plot.addGraph(graph1);

    // 6. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.getPlotter()->setPlotLabel(QObject::tr("Scatter Example"));
    plot.show();
    plot.resize(600/plot.devicePixelRatioF(),400/plot.devicePixelRatioF());
}


void createErrorScatterGraph(JKQTPlotter&plot) {
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. add two columns to the JKQTPDatastore and obtain back-inserter iterators for these
    size_t columnX=ds->addColumn("x");
    auto colXInserter=ds->backInserter(columnX);
    size_t columnY=ds->addColumn("y");
    auto colYInserter=ds->backInserter(columnY);
    size_t columnY2=ds->addColumn("y2");
    auto colY2Inserter=ds->backInserter(columnY2);
    size_t columnEX=ds->addColumn("xerr");
    auto colEXInserter=ds->backInserter(columnEX);
    size_t columnEY=ds->addColumn("yerr");
    auto colEYInserter=ds->backInserter(columnEY);

    // 3. now we create data for a simple plot (a sine curve with random noise)
    std::default_random_engine generator(123);
    std::normal_distribution<double> distribution(0,0.5);
    const int Ndata=15;
    for (int i=0; i<Ndata; i++) {
        // put data
        const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
        *colXInserter=x;
        *colYInserter=sin(x)+2.0;
        *colY2Inserter=cos(x)-2.0;
        *colEXInserter=distribution(generator);
        *colEYInserter=distribution(generator);
        // advance back-inserters
        colXInserter++;
        colYInserter++;
        colY2Inserter++;
        colEXInserter++;
        colEYInserter++;
    }


    // 4. create a graph in the plot, which plots the dataset X/Y:
    JKQTPXYScatterErrorGraph* graph1=new JKQTPXYScatterErrorGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY);
    graph1->setYErrorColumn(columnEY);
    graph1->setYErrorStyle(JKQTPErrorSimpleBars);
    graph1->setSymbolType(JKQTPCross);
    graph1->setTitle(QObject::tr("y-errorlines"));
    plot.addGraph(graph1);

    JKQTPXYScatterErrorGraph* graph2=new JKQTPXYScatterErrorGraph(&plot);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setXErrorColumn(columnEX);
    graph2->setYErrorColumn(columnEY);
    graph2->setXErrorStyle(JKQTPErrorBars);
    graph2->setYErrorStyle(JKQTPErrorBars);
    graph2->setTitle(QObject::tr("xy-errorbars"));
    graph2->setSymbolLineWidth(3);
    plot.addGraph(graph2);

    // 6. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.getPlotter()->setPlotLabel(QObject::tr("Error Scatter Example"));
    plot.setGrid(false);
    plot.show();
    plot.resize(600/plot.devicePixelRatioF(),400/plot.devicePixelRatioF());
}

int main(int argc, char* argv[])
{

    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot, plotErr;

    createScatterGraph(plot);
    plot.setWindowTitle("1: JKQTPXYScatterGraph");
    createErrorScatterGraph(plotErr);
    plotErr.setWindowTitle("2: JKQTPXYScatterErrorGraph");


    return app.exec();
}
