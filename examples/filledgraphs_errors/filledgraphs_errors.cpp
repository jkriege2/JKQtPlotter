/** \example filledgraphs_errors.cpp
 * Shows how to draw filled graphs with error indicators in a JKQTPlotter
 *
 * \ref JKQTPlotterFilledGraphsErrorBars
 */

#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpfilledcurve.h"
#include "jkqtpexampleapplication.h"
#include "jkqtcommon/jkqtpmathtools.h"



template <class TCHART>
void doExample(JKQTPlotter& plot, const QString& title)
{
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create three columns for key and value
    size_t columnK=ds->addLinearColumn(9, 0.1*JKQTPSTATISTICS_PI, 1.0*JKQTPSTATISTICS_PI,"k");
    size_t columnK2=ds->addLinearColumn(9, 1.1*JKQTPSTATISTICS_PI, 2.0*JKQTPSTATISTICS_PI,"k2");
    size_t columnK3=ds->addLinearColumn(9, 2.1*JKQTPSTATISTICS_PI, 3.0*JKQTPSTATISTICS_PI,"k2");
    size_t columnV=ds->addCalculatedColumnFromColumn(columnK, [](double x) { return sin(x); }, "v");
    size_t columnV2=ds->addCalculatedColumnFromColumn(columnK2, [](double x) { return -sin(x); }, "v2");
    size_t columnV3=ds->addCalculatedColumnFromColumn(columnK3, [](double x) { return sin(x); }, "v3");
    size_t columnE=ds->addCalculatedColumnFromColumn(columnV, [](double x) { return 0.2*x; }, "error");
    size_t columnE2=ds->addCalculatedColumnFromColumn(columnV2, [](double x) { return 0.2*x; }, "error");
    size_t columnE3=ds->addCalculatedColumnFromColumn(columnV3, [](double x) { return 0.2*x; }, "error");

    // 3. create a graph in the plot, which plots the dataset with symmetric:
    TCHART* graph1=new TCHART(&plot);
    graph1->setKeyColumn(columnK);
    graph1->setValueColumn(columnV);
    graph1->setValueErrorColumn(columnE);
    // set error indicator style
    graph1->setValueErrorStyle(JKQTPErrorBars);
    graph1->setTitle(QObject::tr("JKQTPErrorBars"));
    plot.addGraph(graph1);

    // 4. create a second graph in the plot, which plots the second dataset with outer error bars only:
    TCHART* graph2=new TCHART(&plot);
    graph2->setKeyColumn(columnK2);
    graph2->setValueColumn(columnV2);
    graph2->setValueErrorColumn(columnE2);
    // set error indicator style
    graph2->setValueErrorStyle(JKQTPErrorPolygons);
    graph2->setTitle(QObject::tr("JKQTPErrorPolygons"));
    plot.addGraph(graph2);

    // 5. create a third graph in the plot, which plots the second dataset with outer error bars only:
    TCHART* graph3=new TCHART(&plot);
    graph3->setKeyColumn(columnK3);
    graph3->setValueColumn(columnV3);
    graph3->setValueErrorColumn(columnE3);
    // set error indicator style
    graph3->setValueErrorStyle(JKQTPErrorLines);
    graph3->setTitle(QObject::tr("JKQTPErrorLines"));
    plot.addGraph(graph3);


    // 6 autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 7. show plotter and make it a decent size
    plot.getPlotter()->setKeyPosition(JKQTPKeyInsideBottomLeft);
    plot.setWindowTitle(title);
    plot.show();
    plot.resize(400,400);
}

int main(int argc, char* argv[])
{

    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    JKQTPlotter plotV, plotH;
    doExample<JKQTPFilledCurveXErrorGraph>(plotV, "1: JKQTPFilledCurveXErrorGraph");
    doExample<JKQTPFilledCurveYErrorGraph>(plotH, "2: JKQTPFilledCurveYErrorGraph");

    return app.exec();
}
