/** \example contourplot.cpp
 * Shows how to plot a contour plot with JKQTPlotter
 *
 * \ref JKQTPlotterContourPlot
 */

#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpcontour.h"

// if deJKQTPSTATISTICS_PId, an animation is shown
//#definJKQTPSTATISTICS_PIIMATE

#ifdef ANIMATE
#include "contourplotanimator.h"

#endif

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering

    // 2. calculate image of the electric field of a quadrupolpol
    JKQTPDatastore* ds=plot.getDatastore();
#ifdef ANIMATE
    const int NX=200; // image dimension in x-direction [pixels]
    const int NY=200; // image dimension in x-direction [pixels]
#else
    const int NX=500; // image dimension in x-direction [pixels]
    const int NY=500; // image dimension in x-direction [pixels]
#endif
    const double w=2.7e-6;
    const double dx=w/static_cast<double>(NX);
    const double h=NY*dx;
    size_t cPotential=ds->addImageColumn(NX, NY, "imagedata");

    double x;
    double y=-h/2.0;
    const double eps0=8.854187e-12;
    const double Q1=1.6e-19;           // charge of charged particle 1
    const double Q1_x0=-0.5e-6;        // x-position of charged particle 1
    const double Q1_y0=-0.5e-6;        // y-position of charged particle 1
    const double Q2=1.6e-19;           // charge of charged particle 2
    const double Q2_x0=0.5e-6;         // x-position of charged particle 2
    const double Q2_y0=0.5e-6;         // y-position of charged particle 2
    const double Q3=-1.6e-19;          // charge of charged particle 3
    const double Q3_x0=-0.5e-6;        // x-position of charged particle 3
    const double Q3_y0=0.5e-6;         // y-position of charged particle 3
    const double Q4=-1.6e-19;          // charge of charged particle 4
    const double Q4_x0=0.5e-6;         // x-position of charged particle 4
    const double Q4_y0=-0.5e-6;        // y-position of charged particle 4
    for (size_t iy=0; iy<NY; iy++ ) {
        x=-w/2.0;
        for (size_t ix=0; ix<NX; ix++ ) {
            const double r1=sqrt((x-Q1_x0)*(x-Q1_x0)+(y-Q1_y0)*(y-Q1_y0));
            const double r2=sqrt((x-Q2_x0)*(x-Q2_x0)+(y-Q2_y0)*(y-Q2_y0));
            const double r3=sqrt((x-Q3_x0)*(x-Q3_x0)+(y-Q3_y0)*(y-Q3_y0));
            const double r4=sqrt((x-Q4_x0)*(x-Q4_x0)+(y-Q4_y0)*(y-Q4_y0));
            ds->setPixel(cPotential, ix, iy, Q1/(4.0*JKQTPSTATISTICS_PI*eps0)/r1+Q2/(4.0*JKQTPSTATISTICS_PI*eps0)/r2+Q3/(4.0*JKQTPSTATISTICS_PI*eps0)/r3+Q4/(4.0*JKQTPSTATISTICS_PI*eps0)/r4);
            x+=dx;
        }
        y+=dx;
    }


    // the following code will plot a dipole JKQTPSTATISTICS_PIntial instead of tJKQTPSTATISTICS_PIuadrupole after 5sJKQTPSTATISTICS_PI // this tests theJKQTPSTATISTICS_PIognition of altered data
#ifdef ANIMATE
    JKQTPlotter* pplot=&plot;
    ContourPlotAnimator animation(ds,pplot,NX,NY,w,h,dx,cPotential);
    animation.start(3000);
#endif


    // 3. create a graph (JKQTPColumnContourPlot) with the column created above as data
    JKQTPColumnContourPlot* graph=new JKQTPColumnContourPlot(&plot);
    graph->setTitle("");
    // image column with the data
    graph->setImageColumn(cPotential);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->setX(-w/2.0);
    graph->setY(-h/2.0);
    // width and height of the image in plot-axis-coordinates
    graph->setWidth(w);
    graph->setHeight(h);
    // color-map is "BlueGreenRed"
    graph->setColorPalette(JKQTPMathImageBlueGreenRed);
    // get coordinate axis of color-bar and set its label
    graph->getColorBarRightAxis()->setAxisLabel("electric potential [V]");
    // add some levels for the contours. These are chosen to be at the actual potential values
    // at several specified relative distance from Q1, i.e. at phi(Q1_x0*reldist) (phi: potential of Q1)
    QVector<double> reldists; reldists<<0.1<<0.25<<0.5<<1<<1.5<<2<<2.5<<3;
    // finally contour levels with +1 and -1 sign are added to show the positive and negative potential:
    for (auto reldist: reldists) {
        const double level=fabs(Q1/(4.0*JKQTPSTATISTICS_PI*eps0)/(Q1_x0*reldist));
        graph->addContourLevel(-level);
        graph->addContourLevel(level);

        // set a special color for some JKQTPSTATISTICS_PIs:
        //if (reldist==1) {
        //    graph->setOverrideColor(-level, QColor("yellow"));
        //    graph->setOverrideColor(level, QColor("yellow"));
        //}
    }
    qDebug()<<graph->getContourLevels();
    graph->setAutoImageRange(false);
    graph->setImageMin(graph->getContourLevels().first());
    graph->setImageMax(graph->getContourLevels().last());

    // all contour lines have the same color:
    //graph->setContourColoringMode(JKQTPColumnContourPlot::SingleColorContours);
    //graph->setLineColor(QColor("magenta"));

    // color contour lines from palette, but wothout taking their actual level value into account:
    //graph->setContourColoringMode(JKQTPColumnContourPlot::ColorContoursFromPalette);

	
    // 4. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);

	
    // 5. set axis labels
    plot.getXAxis()->setAxisLabel("x [m]");
    plot.getYAxis()->setAxisLabel("y [m]");

	
    // 6. fix axis and plot aspect ratio to 1
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setAspectRatio(w/h);
    plot.getPlotter()->setMaintainAxisAspectRatio(true);
    plot.getPlotter()->setAxisAspectRatio(w/h);

    // 7 autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 8. show plotter and make it a decent size
    plot.show();
    plot.resize(800,800);
    plot.setWindowTitle("JKQTPColumnContourPlot");


    return app.exec();
}
