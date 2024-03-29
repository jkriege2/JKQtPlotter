/** \example vectorfield.cpp
 * Display a vector field
 *
 * \ref JKQTPlotterVectorFieldExample
 */
 
#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpvectorfield.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtpexampleapplication.h"


#define NX 9
#define NY 9

int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    // 1. setup a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* ds=plot.getDatastore();



    // 2. make up some arbitrary data to be used for plotting
    //    this generates a 2D grid of x/y-coordinates and then calculates dx=cos(y)*sqrt(x/3.0) and dy=sin(x)*sqrt(x/3.0)
    const auto columnXY=ds->addLinearGridColumns(NX, 0, 6, NY, -3, 3,"x","y");
    const auto columnDX=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return sin(y)*sqrt(x/3.0); });
    const auto columnDY=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return cos(x)*sqrt(x/3.0); });
    // now we also calulate a column that encodes some other information that can be color-coded
    const auto columnC=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return sqrt(fabs(y)); });


    // 3. create JKQTPVectorFieldGraph to display the data:
    JKQTPParametrizedVectorFieldGraph* graph1=new JKQTPParametrizedVectorFieldGraph(&plot);
    graph1->setXYColumns(columnXY);
    graph1->setDxColumn(columnDX);
    graph1->setDyColumn(columnDY);
    graph1->setColorColumn(columnC);
    graph1->setTitle(QObject::tr("$\\vec{f}(x,y)=\\bigl[\\sin(y)\\cdot\\sqrt{x/3}, \\cos(x)\\cdot\\sqrt{x/3}\\bigr]^\\mathrm{T}$"));

    // 4. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph1);

    // 5. scale the plot so the graph is contained
    plot.getPlotter()->setAxisAspectRatio(1);
    plot.getPlotter()->setAspectRatio(1);
    plot.getPlotter()->setMaintainAxisAspectRatio(true);
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.zoomToFit();


    // show plotter and make it a decent size
    plot.setWindowTitle("JKQTPVectorFieldGraph example");
    plot.show();
    plot.resize(400/plot.devicePixelRatioF(),430/plot.devicePixelRatioF());



    app.addExportStepFunctor([&](){
        graph1->setVectorColorMode(JKQTPParametrizedVectorFieldGraph::ColorFromMagnitude);
        plot.redrawPlot();
    });

    app.addExportStepFunctor([&](){
        graph1->setVectorColorMode(JKQTPParametrizedVectorFieldGraph::ColorFromAngle);
        plot.redrawPlot();
    });

    app.addExportStepFunctor([&](){
        graph1->setVectorColorMode(JKQTPParametrizedVectorFieldGraph::DefaultColor);
        plot.redrawPlot();
    });


    return app.exec();
}
