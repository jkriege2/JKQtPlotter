/** \example rgbimageplot.cpp
 * Shows how to plot colored math images with JKQTPlotter, where different images/matrices are assigned to different color channels
 *
 * \ref JKQTPlotterRGBImagePlot
 */

#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpimagergb.h"



int main(int argc, char* argv[])
{
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    JKQTPlotter plot;
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data for the charts (taken from https://commons.wikimedia.org/wiki/File:Energiemix_Deutschland.svg)
    const int NX=100; // image dimension in x-direction [pixels]
    const int NY=100; // image dimension in x-direction [pixels]
    const double dx=1e-2; // size of a pixel in x-direction [micrometers]
    const double dy=1e-2; // size of a pixel in x-direction [micrometers]
    const double w=static_cast<double>(NX)*dx;
    const double h=static_cast<double>(NY)*dy;
    double airydisk1[NX*NY]; // row-major image
    double airydisk2[NX*NY]; // row-major image

    // 2.1 Parameters for airy disk plot (see https://en.wikipedia.org/wiki/Airy_disk)
    double NA=1.1; // numerical aperture of lens
    double wavelength1=540e-3; // wavelength of the light [micrometers]
    double wavelength2=450e-3; // wavelength of the light [micrometers]

    // 2.2 calculate image of airy disk in a row-major array
    double x, y=-h/2.0;
    for (int iy=0; iy<NY; iy++ ) {
        x=-w/2.0;
        for (int ix=0; ix<NX; ix++ ) {
            const double r=sqrt(x*x+y*y);
            const double v1=2.0*JKQTPSTATISTICS_PI*NA*r/wavelength1;
            airydisk1[iy*NX+ix] = sqrt(pow(2.0*jkqtp_j1(v1)/v1, 2));
            const double v2=2.0*JKQTPSTATISTICS_PI*NA*r/wavelength2;
            airydisk2[iy*NX+ix] = sqrt(pow(2.0*jkqtp_j1(v2)/v2, 2));
            x+=dx;
        }
        y+=dy;
    }



    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    //    In this step the contents of C-array airydisk is copied into a column
    //    of the datastore in row-major order
    size_t cAiryDisk1=ds->addCopiedImageAsColumn(airydisk1, NX, NY, "imagedata1");
    size_t cAiryDisk2=ds->addCopiedImageAsColumn(airydisk2, NX, NY, "imagedata2");

	
    // 4. create a graph (JKQTPColumnMathImage) with the column created above as data
    //    The data is color-coded with the color-palette JKQTPMathImageMATLAB
    //    the converted range of data is determined automatically because s etAutoImageRange(true)
    JKQTPColumnRGBMathImage* graph=new JKQTPColumnRGBMathImage(&plot);
    graph->setTitle("");
    // image column with the data (R/G/B or C/M/Y ...)
    graph->setImageGColumn(cAiryDisk1); // G/M channel
    graph->setImageBColumn(cAiryDisk2); // B/Y channel
    // set size of the data (the datastore does not contain this info, as it only manages 1D columns of data and this is used to assume a row-major ordering
    graph->setNx(NX);
    graph->setNy(NY);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->setX(-w/2.0);
    graph->setY(-h/2.0);
    // width and height of the image in plot-axis-coordinates
    graph->setWidth(w);
    graph->setHeight(h);
    // get coordinate axis of color-bar and set its label
    graph->getColorBarRightAxisB()->setAxisLabel("blue light field strength [AU]");
    graph->getColorBarRightAxisG()->setAxisLabel("green light field strength [AU]");
    // determine min/max of data automatically and use it to set the range of the color-scale
    graph->setAutoImageRange(true);
    // use (subtractive) CMY color model, not RGB
    //graph->setRgbMode(JKQTPRGBMathImageModeCMYMode);

	
    // 5. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);

	
    // 6. set axis labels
    plot.getXAxis()->setAxisLabel("x [{\\mu}m]");
    plot.getYAxis()->setAxisLabel("y [{\\mu}m]");

	
    // 7. fix axis and plot aspect ratio to 1
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setMaintainAxisAspectRatio(true);

    // 8 autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600,600);
    plot.setWindowTitle("JKQTPColumnRGBMathImage");


    return app.exec();
}
