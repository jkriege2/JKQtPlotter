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


int main(int argc, char* argv[])
{

    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
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
    plot.resize(600,400);

    return app.exec();
}
