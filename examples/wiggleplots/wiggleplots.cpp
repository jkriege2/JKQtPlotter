/** \example wiggleplots.cpp
 * Shows how to use filled graphs with JKQTPlotter
 *
 * \ref JKQTPlotterwiggleplots
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpfilledcurve.h"
#include "jkqtcommon/jkqtpmathtools.h"
#include <random>

void drawRandomWalkX(JKQTPlotter& plot) {

    // 1. get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create 2 datacolumns with length 1000 (NSteps) entries in the datastore
    //    these will later hold the time-step and position of a random walker
    const size_t NSteps=400;
    const size_t columnT=ds->addLinearColumn(NSteps, 0, NSteps-1, "time");
    const size_t columnX=ds->addColumn(NSteps, "position");

    // 3. now we simulate a simple rendom walk and store the calculated positions
    //    in columnX
    double pos=5;
    const double stepsize=1;
    std::random_device rd; // random number generators:
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> dist(0,1);

    for (size_t t=0; t<NSteps; t++) {
        ds->set(columnX, t, pos);
        pos=pos+stepsize*(static_cast<double>(dist(gen))*2.0-1.0);
    }


    // 4. now we add three semi-transparent, filled curve plots, one for each histogram
    JKQTPFilledCurveXGraph* graph=new JKQTPFilledCurveXGraph(&plot);

    // 5. set graph titles
    graph->setTitle("Random Walk");

    // 6. set data
    graph->setXColumn(columnT); graph->setYColumn(columnX);

    // 7.1 enable wiggle-plot filling
    graph->setFillMode(JKQTPFilledCurveXGraph::TwoColorFilling);
    // 7.2 draw the data also as a black, thin line
    graph->setLineColor(QColor("black"));
    graph->setLineWidth(1);
    // 7.3 fill areas below the baseline with red
    graph->setFillColor(QColor("red"));
    // 7.4 fill areas above the baseline with blue
    graph->fillStyleBelow().setFillColor(QColor("blue"));
    // 7.5 set the baseline to be at 5 (not the default 0
    graph->setBaseline(5);


    // 8. add the graphs to the plot, so they are actually displayed
    plot.addGraph(graph);

    // 9. set axis labels
    plot.getXAxis()->setAxisLabel("time $t$");
    plot.getYAxis()->setAxisLabel("walker position $x(t)$");


    // 10. scale plot automatically
    plot.zoomToFit();

    // 11. show plotter and make it a decent size
    plot.show();
    plot.resize(600,400);
}

void drawWavepacketsY(JKQTPlotter& plot) {

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create a datacolumns with length 1200 (NSteps) entries in the datastore
    //    these will later hold the time-value for each wavepacket
    const size_t NSteps=1200;
    const size_t columnT=ds->addLinearColumn(NSteps, 0, 1, "time");


    // 3. now we calculate several wavepackets and add a graph for each.
    const size_t NWavepackets=7;
    for (size_t nw=0; nw<NWavepackets; nw++) {
        const size_t columnPacket=ds->addColumn(NSteps, "wavepacket1");

        const double packwidth=0.4/static_cast<double>(NWavepackets);
        const double pos=pow(static_cast<double>(nw), 0.6)*packwidth*3.5+5.0*packwidth;
        const double wavelength=packwidth/1.5;
        const double offset=(static_cast<double>(nw)*1.5+1.5);
        for (size_t ti=0; ti<NSteps; ti++) {
            const double t=ds->get(columnT, ti);
            ds->set(columnPacket, ti, offset+sin(2.0*JKQTPSTATISTICS_PI*t/wavelength)*exp(-0.5*jkqtp_sqr(t-pos)/jkqtp_sqr(packwidth)));
        }


        // 4. now we add three semi-transparent, filled curve plots, one for each histogram
        JKQTPFilledCurveYGraph* graph=new JKQTPFilledCurveYGraph(&plot);

        // set graph titles
        graph->setTitle("wave "+QString::number(nw+1));

        // set data
        graph->setYColumn(columnT); graph->setXColumn(columnPacket);

        // enable wiggle-plot filling
        graph->setFillMode(JKQTPFilledCurveXGraph::TwoColorFilling);
        // draw the data also as a black, thin line
        graph->setLineColor(QColor("black"));
        graph->setLineWidth(1);
        // fill areas below the baseline with red
        graph->setFillColor(Qt::transparent);
        // fill areas above the baseline with blue
        graph->fillStyleBelow().setFillColor(QColor("black"));
        // set the baseline to be at 5 (not the default 0
        graph->setBaseline(offset);


        // add the graphs to the plot, so they are actually displayed
        plot.addGraph(graph);
    }

    // 5. set axis labels
    plot.getXAxis()->setAxisLabel("wavepacket amplitude $f(t)$");
    plot.getYAxis()->setAxisLabel("time $t$");


    // 4. scale plot automatically
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(600,600);
}

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);

    JKQTPlotter plotWalk, plotWavepackets;
    drawRandomWalkX(plotWalk);
    drawWavepacketsY(plotWavepackets);

    return app.exec();
}
