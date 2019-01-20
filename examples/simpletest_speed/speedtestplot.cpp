/** \example speedtestplot.cpp
 * JKQTPlotter: Examples: Simple line-graph with live-data (speed test)
 *
 * \ref JKQTPlotterSpeedTest
 */

#include "speedtestplot.h"
#include "jkqtplotter/jkqtpgraphs.h"


SpeedTestPlot::SpeedTestPlot():
    JKQTPlotter(), dx(1.0/double(NDATA)*4.0*M_PI), x0(0)

{
    // 1. optimize JKQTPlotter for speed (by switching off anti-aliasing)
    get_plotter()->set_useAntiAliasingForGraphs(false);
    get_plotter()->set_useAntiAliasingForSystem(false);
    get_plotter()->set_useAntiAliasingForText(false);


    // 2. now we create data for a simple plot (a sine curve + random[-0.5,0.5])
    for (size_t i=0; i<NDATA; i++) {
        const double x=static_cast<double>(i)*dx;
        X[i]=x0+x;
        Y[i]=sin(x)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;
        Y2[i]=cos(x)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;
    }

    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    //    Here the data from the std::array's is not copied, but only the pointer to
    //    the array is added to the datastore. therefore the datastore does not manage
    //    the memory, oly uses the data stored in it!
    JKQTPDatastore* ds=getDatastore();
    size_t columnX=ds->addColumn(X.data(), X.size(), "x");
    size_t columnY=ds->addColumn(Y.data(), Y.size(), "y");
    size_t columnY2=ds->addColumn(Y2.data(), Y2.size(), "y2");

    // 4. create two  graphs in the plot, which plots the dataset X/Y:
    JKQTPXYLineGraph* graph=new JKQTPXYLineGraph(this);
    graph->set_xColumn(columnX);
    graph->set_yColumn(columnY);
    graph->set_title(QObject::tr("live sin() graph"));
    graph->set_lineWidth(1);
    addGraph(graph);

    JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(this);
    graph2->set_xColumn(columnX);
    graph2->set_yColumn(columnY2);
    graph2->set_title(QObject::tr("live cos() graph"));
    graph2->set_lineWidth(1);
    addGraph(graph2);

    // 6. scale the plot so the graph is contained
    setX(X[0], X[NDATA-1]);
    setY(-2,2);

    // show plotter and make it a decent size
    show();
    resize(1000,500);
}

SpeedTestPlot::~SpeedTestPlot()
{

}

void SpeedTestPlot::plotNewData()
{
    // move old data to the left
    for (size_t i=0; i<NDATA-1; i++) {
        X[i]=X[i+1];
        Y[i]=Y[i+1];
        Y2[i]=Y2[i+1];
    }
    // add one new data point
    X[NDATA-1]=X[NDATA-2]+dx;
    Y[NDATA-1]=sin(X[NDATA-1])+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;
    Y2[NDATA-1]=cos(X[NDATA-1])+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;

    /*
    // ALTERNATIVE: MOVE data, but keep x-axis range
    x0+=dx;
    for (size_t i=0; i<NDATA-1; i++) {
        Y[i]=Y[i+1];
        Y2[i]=Y2[i+1];
    }
    // add one new data point
    Y[NDATA-1]=sin(X[NDATA-1]+x0)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;
    Y2[NDATA-1]=cos(X[NDATA-1]+x0)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;
    */


    // set new x-range and replot
    setX(X[0], X[NDATA-1]);
    update_plot();

    // calculate and update FPS-rate in window title
    auto tlastalst=t_lastplot;
    t_lastplot=std::chrono::system_clock::now();
    double delta_secs=static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(t_lastplot-tlastalst).count())/1000.0;
    setWindowTitle(QString("Live Data Speed Test: %2 datapoint, %1 fps").arg(1/delta_secs).arg(NDATA));
    // enqueue call for next data value
    QTimer::singleShot(1, this, SLOT(plotNewData()));
}
