/** \example barchart_twocolor.cpp
 * Shows how to draw Barcharts with different colors above and below the baseline in a JKQTPlotter
 *
 * \ref JKQTPlotterBarchartsTwoColorFilling
 */
 
#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtpexampleapplication.h"
#include "jkqtcommon/jkqtpmathtools.h"



template <class TCHART>
void doExample(JKQTPlotter& plot, const QString& title)
{
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create two columns for key and value
    size_t columnK=ds->addLinearColumn(15, 0.5*JKQTPSTATISTICS_PI, 3.0*JKQTPSTATISTICS_PI,"k");
    size_t columnV=ds->addColumnCalculatedFromColumn(columnK, &cos, "v");

    // 3. create graph in the plot, which plots the dataset:
    JKQTPBarGraphBase* graph=new TCHART(&plot);
    graph->setKeyColumn(columnK);
    graph->setValueColumn(columnV);
    // set TwoColor fill Mode
    graph->setFillMode(JKQTPBarGraphBase::FillMode::TwoColorFilling);
    graph->setFillColor(QColor("green"));
    graph->fillStyleBelow().setFillColor(QColor("red"));
    plot.addGraph(graph);

    // 4 autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.setWindowTitle(title);
    plot.show();
    plot.resize(400,400);
}

int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    JKQTPlotter plotV, plotH;
    doExample<JKQTPBarVerticalGraph>(plotV, "1: JKQTPBarVerticalGraph");
    doExample<JKQTPBarHorizontalGraph>(plotH, "2: JKQTPBarHorizontalGraph");

    return app.exec();
}
