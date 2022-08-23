/** \example speedtestplot.cpp
 * JKQTPlotter: Examples: Simple line-graph with live-data (speed test)
 *
 * \ref JKQTPlotterSpeedTest
 */

#include "speedtestplot.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"

SpeedTestPlot::SpeedTestPlot():
    JKQTPlotter(), NDATA(500), dx(1.0/500.0*4.0*JKQTPSTATISTICS_PI), x0(0)

{
    // 1. optimize JKQTPlotter for speed (by switching off anti-aliasing)
    getPlotter()->setUseAntiAliasingForGraphs(false);
    getPlotter()->setUseAntiAliasingForSystem(false);
    getPlotter()->setUseAntiAliasingForText(false);


    // 2. now we create data for a simple plot (a sine curve + random[-0.5,0.5])
    for (size_t i=0; i<X.size(); i++) {
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
    size_t columnX=ds->addColumn(X.data(), NDATA, "x");
    size_t columnY=ds->addColumn(Y.data(), NDATA, "y");
    size_t columnY2=ds->addColumn(Y2.data(), NDATA, "y2");

    // 4. create two  graphs in the plot, which plots the dataset X/Y:
    graph=new JKQTPXYLineGraph(this);
    graph->setXColumn(columnX);
    graph->setYColumn(columnY);
    graph->setTitle(QObject::tr("live sin() graph"));
    graph->setLineWidth(1);
    addGraph(graph);

    graph2=new JKQTPXYLineGraph(this);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setTitle(QObject::tr("live cos() graph"));
    graph2->setLineWidth(1);
    addGraph(graph2);

    // 6. scale the plot so the graph is contained
    setX(X[0], X[NDATA-1]);
    setY(-2,2);

    actAntiAliase=new QAction(QObject::tr("Anti-Aliase"));
    actAntiAliase->setCheckable(true);
    actAntiAliase->setChecked(false);
    connect(actAntiAliase, &QAction::triggered, std::bind([](SpeedTestPlot* p){
                p->getPlotter()->setUseAntiAliasingForGraphs(p->actAntiAliase->isChecked());
                p->getPlotter()->setUseAntiAliasingForSystem(p->actAntiAliase->isChecked());
                p->getPlotter()->setUseAntiAliasingForText(p->actAntiAliase->isChecked());
            }, this));

    actTwoGraphs=new QAction(QObject::tr("2 Graphs"));
    actTwoGraphs->setCheckable(true);
    actTwoGraphs->setChecked(true);
    connect(actTwoGraphs, &QAction::triggered, std::bind([](SpeedTestPlot* p, JKQTPXYLineGraph* g){
                g->setVisible(p->actTwoGraphs->isChecked());
            }, this, graph2));

    actFixedXAxis=new QAction(QObject::tr("Fixed X-Axis"));
    actFixedXAxis->setCheckable(true);
    actFixedXAxis->setChecked(false);

    actLines=new QAction(QObject::tr("Show Graph Lines"));
    actLines->setCheckable(true);
    actLines->setChecked(true);
    connect(actLines, &QAction::toggled, std::bind([](bool enabled, JKQTPXYLineGraph* g, JKQTPXYLineGraph* g2){
                g->setDrawLine(enabled);
                g2->setDrawLine(enabled);
            }, std::placeholders::_1, graph, graph2));

    actSymbols=new QAction(QObject::tr("Show Graph Symbols"));
    actSymbols->setCheckable(true);
    actSymbols->setChecked(true);
    connect(actSymbols, &QAction::toggled, std::bind([](bool enabled, JKQTPXYLineGraph* g, JKQTPXYLineGraph* g2){
                g->setSymbolType(enabled?JKQTPCross:JKQTPNoSymbol);
                g2->setSymbolType(enabled?JKQTPCircle:JKQTPNoSymbol);
            }, std::placeholders::_1, graph, graph2));

    menuSizes=new QMenu(QObject::tr("number of datapoints"), this);
    QActionGroup* actGroup=new QActionGroup(menuSizes);
    for (size_t size: {50,100, 500, 1000, 2000, 10000}) {
        QAction* act=actGroup->addAction(QString::number(size));
        act->setCheckable(true);
        act->setChecked(size==500);
        connect(act, &QAction::toggled, std::bind([](bool enabled,SpeedTestPlot* p, size_t size){
                    if (enabled) {
                        p->updateDataSize(size);
                    }
                }, std::placeholders::_1, this, size));
        menuSizes->addAction(act);
    }

    addAction(actAntiAliase);
    addAction(actFixedXAxis);
    addAction(menuSizes->menuAction());
    addAction(actTwoGraphs);
    addAction(actLines);
    addAction(actSymbols);

    // show plotter and make it a decent size
    show();
    resize(1000,500);
}

SpeedTestPlot::~SpeedTestPlot()
{

}

void SpeedTestPlot::plotNewData()
{
    if (actFixedXAxis->isChecked()) {
        // ALTERNATIVE: MOVE data, but keep x-axis range
        x0+=dx;
        for (size_t i=0; i<NDATA-1; i++) {
            Y[i]=Y[i+1];
            Y2[i]=Y2[i+1];
        }
        // add one new data point
        Y[NDATA-1]=sin(X[NDATA-1]+x0)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;
        Y2[NDATA-1]=cos(X[NDATA-1]+x0)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;
    } else {
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

    }


    // set new x-range and replot
    setX(X[0], X[NDATA-1]);
    redrawPlot();

    // calculate and update FPS-rate in window title
    const auto tlastalst=t_lastplot;
    t_lastplot=std::chrono::system_clock::now();
    const double delta_secs=static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(t_lastplot-tlastalst).count())/1000.0;
    setWindowTitle(QString("Live Data Speed Test: %2 datapoint, %1 fps").arg(1.0/delta_secs,0,'f',2).arg(NDATA));
    // enqueue call for next data value
    QTimer::singleShot(1, this, SLOT(plotNewData()));
}

void SpeedTestPlot::updateDataSize(size_t newSize)
{
    NDATA=newSize;
    dx=1.0/double(NDATA)*4.0*JKQTPSTATISTICS_PI;
    // 2. now we create data for a simple plot (a sine curve + random[-0.5,0.5])
    for (size_t i=0; i<X.size(); i++) {
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
    ds->clear();
    size_t columnX=ds->addColumn(X.data(), NDATA, "x");
    size_t columnY=ds->addColumn(Y.data(), NDATA, "y");
    size_t columnY2=ds->addColumn(Y2.data(), NDATA, "y2");

    // 4. create two  graphs in the plot, which plots the dataset X/Y:
    graph->setXColumn(columnX);
    graph->setYColumn(columnY);

    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);

    // 6. scale the plot so the graph is contained
    setX(X[0], X[NDATA]);
    setY(-2,2);
}
