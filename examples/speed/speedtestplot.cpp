/** \example speedtestplot.cpp
 * JKQTPlotter: Examples: Simple line-graph with live-data (speed test)
 *
 * \ref JKQTPlotterSpeedTest
 */

#include "jkqtpexampleapplication.h"
#include "speedtestplot.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtmath/jkqtpstatisticstools.h"

SpeedTestPlot::SpeedTestPlot():
    JKQTPlotter(), NDATA(500), dx(1.0/500.0*4.0*JKQTPSTATISTICS_PI), x0(0)

{
    X.fill(0);
    Y.fill(0);
    Y2.fill(0);

    // 1. optimize JKQTPlotter for speed (by switching off anti-aliasing)
    getPlotter()->setUseAntiAliasingForGraphs(false);
    getPlotter()->setUseAntiAliasingForSystem(false);
    getPlotter()->setUseAntiAliasingForText(false);


    // 2. now we create data for a simple plot (a sine curve + random[-0.5,0.5])
    updateDataSize(NDATA, false);

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
    graph->setSymbolType(JKQTPNoSymbol);
    addGraph(graph);

    graph2=new JKQTPXYLineGraph(this);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setTitle(QObject::tr("live cos() graph"));
    graph2->setLineWidth(1);
    graph2->setSymbolType(JKQTPNoSymbol);

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
                if (!p->actAnimation->isChecked()) p->redrawPlot();
            }, this));

    actTwoGraphs=new QAction(QObject::tr("2 Graphs"));
    actTwoGraphs->setCheckable(true);
    actTwoGraphs->setChecked(true);
    connect(actTwoGraphs, &QAction::triggered, std::bind([](SpeedTestPlot* p, JKQTPXYLineGraph* g){
                g->setVisible(p->actTwoGraphs->isChecked());
                if (!p->actAnimation->isChecked()) p->redrawPlot();
            }, this, graph2));

    actFixedXAxis=new QAction(QObject::tr("Fixed X-Axis"));
    actFixedXAxis->setCheckable(true);
    actFixedXAxis->setChecked(false);

    actLines=new QAction(QObject::tr("Show Graph Lines"));
    actLines->setCheckable(true);
    actLines->setChecked(true);
    connect(actLines, &QAction::toggled, std::bind([](bool enabled, JKQTPXYLineGraph* g, JKQTPXYLineGraph* g2,SpeedTestPlot* p){
                g->setDrawLine(enabled);
                g2->setDrawLine(enabled);
                if (!p->actAnimation->isChecked()) p->redrawPlot();
            }, std::placeholders::_1, graph, graph2,this));

    actSymbols=new QAction(QObject::tr("Show Graph Symbols"));
    actSymbols->setCheckable(true);
    actSymbols->setChecked(false);
    connect(actSymbols, &QAction::toggled, std::bind([](bool enabled, JKQTPXYLineGraph* g, JKQTPXYLineGraph* g2,SpeedTestPlot* p){
                g->setSymbolType(enabled?JKQTPCross:JKQTPNoSymbol);
                g2->setSymbolType(enabled?JKQTPCircle:JKQTPNoSymbol);
                if (!p->actAnimation->isChecked()) p->redrawPlot();
            }, std::placeholders::_1, graph, graph2,this));

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

    actUseNonvisibleLineCompression=new QAction(QObject::tr("use NonvisibleLineCompression"));
    actUseNonvisibleLineCompression->setCheckable(true);
    actUseNonvisibleLineCompression->setChecked(true);
    connect(actUseNonvisibleLineCompression, &QAction::toggled, std::bind([](bool enabled, JKQTPXYLineGraph* g, JKQTPXYLineGraph* g2,SpeedTestPlot* p){
                g->setUseNonvisibleLineCompression(enabled);
                g2->setUseNonvisibleLineCompression(enabled);
                if (!p->actAnimation->isChecked()) p->redrawPlot();
            }, std::placeholders::_1, graph, graph2,this));


    menuUseNonvisibleLineCompressionAgressiveness=new QMenu(QObject::tr("NonvisibleLineCompression level"), this);
    actGroup=new QActionGroup(menuUseNonvisibleLineCompressionAgressiveness);
    for (double a: {0.5, 0.8, 1.0, 1.5, 2.0, 5.0}) {
        QAction* act=actGroup->addAction(QString::number(a));
        act->setCheckable(true);
        act->setChecked(a==1.0);
        connect(act, &QAction::toggled, std::bind([](bool enabled,JKQTPXYLineGraph* g, JKQTPXYLineGraph* g2,SpeedTestPlot* p, double a){
                    g->setNonvisibleLineCompressionAgressiveness(a);
                    g2->setNonvisibleLineCompressionAgressiveness(a);
                    if (!p->actAnimation->isChecked()) p->redrawPlot();
                }, std::placeholders::_1, graph, graph2,this, a));
        menuUseNonvisibleLineCompressionAgressiveness->addAction(act);
    }

    actStepAnimation=new QAction(QObject::tr("Next Animation Step"));
    actStepAnimation->setCheckable(false);
    actStepAnimation->setEnabled(false);
    connect(actStepAnimation, &QAction::triggered, std::bind([](SpeedTestPlot* p){
                p->plotNewData();
            }, this));

    actAnimation=new QAction(QObject::tr("Animation Active"));
    actAnimation->setCheckable(true);
    actAnimation->setChecked(true);
    connect(actAnimation, &QAction::toggled, std::bind([](bool enabled, SpeedTestPlot* p, QAction* actStepAnimation){
                if (enabled) {
                    p->plotNewData();
                }
                actStepAnimation->setEnabled(!enabled);
            }, std::placeholders::_1, this, actStepAnimation));


    addAction(actAntiAliase);
    addAction(actFixedXAxis);
    addAction(menuSizes->menuAction());
    addAction(actTwoGraphs);
    addAction(actLines);
    addAction(actSymbols);
    addAction(actUseNonvisibleLineCompression);
    addAction(menuUseNonvisibleLineCompressionAgressiveness->menuAction());
    addAction(actAnimation);
    addAction(actStepAnimation);

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
        Y[NDATA-1]=sin(X[NDATA-1]+x0)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5+addOutlier(1.0/static_cast<double>(NDATA/5), 2.0);
        Y2[NDATA-1]=cos(X[NDATA-1]+x0)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5+addOutlier(1.0/static_cast<double>(NDATA/7), 2.0);
    } else {
        // move old data to the left
        for (size_t i=0; i<NDATA-1; i++) {
            X[i]=X[i+1];
            Y[i]=Y[i+1];
            Y2[i]=Y2[i+1];
        }
        // add one new data point
        X[NDATA-1]=X[NDATA-2]+dx;
        Y[NDATA-1]=sin(X[NDATA-1])+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5+addOutlier(1.0/static_cast<double>(NDATA/5), 2.0);
        Y2[NDATA-1]=cos(X[NDATA-1])+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5+addOutlier(1.0/static_cast<double>(NDATA/7), 2.0);

    }


    // set new x-range and replot
    setX(X[0], X[NDATA-1]);
    redrawPlot();

    // calculate and update FPS-rate in window title
    const auto tlastalst=t_lastplot;
    t_lastplot=std::chrono::system_clock::now();
    const double delta_secs=static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(t_lastplot-tlastalst).count())/1000.0;
    calctimes.push_back(delta_secs);
    if (delta_secs<0.05) { while (calctimes.size()>60) calctimes.pop_front(); }
    else if (delta_secs<0.1) { while (calctimes.size()>30) calctimes.pop_front(); }
    else if (delta_secs<1) { while (calctimes.size()>10) calctimes.pop_front(); }
    else { while (calctimes.size()>5) calctimes.pop_front(); }
    setWindowTitle(QString("Live Data Speed Test: %2 datapoint, %3 [this: %1] fps").arg(1.0/delta_secs,0,'f',2).arg(NDATA).arg(1.0/jkqtpstatAverage(calctimes.begin(), calctimes.end()),0,'f',2));
    // enqueue call for next data value
    if (actAnimation->isChecked()) QTimer::singleShot(1, this, SLOT(plotNewData()));
}

void SpeedTestPlot::updateDataSize(size_t newSize, bool updatePlots)
{
    NDATA=newSize;
    dx=1.0/double(NDATA)*4.0*JKQTPSTATISTICS_PI;
    // 2. now we create data for a simple plot (a sine curve + random[-0.5,0.5])
    for (size_t i=0; i<X.size(); i++) {
        const double x=static_cast<double>(i)*dx;
        X[i]=x0+x;
        Y[i]=sin(x)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5+addOutlier(1.0/static_cast<double>(NDATA/5), 2.0);
        Y2[i]=cos(x)+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5+addOutlier(1.0/static_cast<double>(NDATA/7), 2.0);
    }

    if (updatePlots) {
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
}

double SpeedTestPlot::addOutlier(double prob, double height)
{
    if (static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)<prob) return height*(2.0*static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-1.0);
    else return 0;
}
