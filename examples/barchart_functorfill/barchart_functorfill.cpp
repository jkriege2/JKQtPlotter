/** \example barchart_functorfill.cpp
 * Shows how to draw Barcharts with colors defined by a functor JKQTPlotter
 *
 * \ref JKQTPlotterBarchartsFunctorFilling
 */

#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtpexampleapplication.h"



template <class TCHART>
void doExample(JKQTPlotter& plot, const QString& title)
{
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create two columns for key and value
    size_t columnK=ds->addLinearColumn(11, 0, 10, "k");
    size_t columnV=ds->addColumnCalculatedFromColumn(columnK, [](double x) { return 5.0+x; }, "v");

    // 3. create graph in the plot, which plots the dataset:
    JKQTPBarGraphBase* graph=new TCHART(&plot);
    graph->setKeyColumn(columnK);
    graph->setValueColumn(columnV);
    // set TwoColor fill Mode
    graph->setFillMode(JKQTPBarGraphBase::FillMode::FunctorFilling);
    graph->setFillBrushFunctor([](double key, double /*value*/) {return QBrush(QColor::fromHsvF(key/12.0, 1.0, 1.0)); });
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
