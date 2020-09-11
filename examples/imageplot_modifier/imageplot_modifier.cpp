/** \example imageplot_modifier.cpp
 * Shows how to plot colored math images/matrices modified by a second data-column/image with JKQTPlotter
 *
 * \ref JKQTPlotterImagePlotModifier
 */

#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpimage.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    JKQTPlotter plot;
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data for the charts (taken from https://commons.wikimedia.org/wiki/File:Energiemix_Deutschland.svg)
    const int NX=150; // image dimension in x-direction [pixels]
    const int NY=150; // image dimension in x-direction [pixels]
    double image[NX*NY]; // row-major image
    double modifier[NX*NY]; // row-major modfier image

    // 2 calculate image of airy disk in a row-major array
    double x, y=-static_cast<double>(NY)/2.0;
    for (int iy=0; iy<NY; iy++ ) {
        x=-static_cast<double>(NX)/2.0;
        for (int ix=0; ix<NX; ix++ ) {
            const double r=sqrt(x*x+y*y);
            image[iy*NX+ix] = cos(JKQTPSTATISTICS_PI*r/20.0);
            modifier[iy*NX+ix] = 1.0-r/sqrt(NX*NX/4.0+NY*NY/4.0);
            x+=1;
        }
        y+=1;
    }



    // 3. make data availJKQTPSTATISTICS_PI to JKQTPlotter by adding it to the internal datastore.
    //    In this step the contents of C-array airydisk is copied into a column
    //    of the datastore in row-major order
    size_t cImage=ds->addCopiedImageAsColumn(image, NX, NY, "imagedata");
    size_t cModifier=ds->addCopiedImageAsColumn(modifier, NX, NY, "modifier");

	
    // 4. create a graph (JKQTPColumnMathImage) with the column created above as data
    //    The data is color-coded with the color-palette JKQTPMathImageMATLAB
    //    the converted range of data is determined automatically because setAutoImageRange(true)
    JKQTPColumnMathImage* graph=new JKQTPColumnMathImage(&plot);
    graph->setTitle("");
    // image column with the data
    graph->setImageColumn(cImage);
    // now set the modifier image:
    graph->setModifierColumn(cModifier);
    graph->setAutoModifierRange(true);
    // ... and specify which image property is modified (here the saturation, but ModifyAlpha for the transparency and ModifyValue from the HSV color-model are also possible):
    graph->setModifierMode(JKQTPMathImageModifierMode::ModifySaturation);
    // set size of the data (the datastore does not contain this info, as it only manages 1D columns of data and this is used to assume a row-major ordering
    graph->setNx(NX);
    graph->setNy(NY);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->setX(-NX/2.0);
    graph->setY(-NX/2.0);
    // width and height of the image in plot-axis-coordinates
    graph->setWidth(NX);
    graph->setHeight(NY);
    // color-map is "MATLAB"
    graph->setColorPalette(JKQTPMathImageMATLAB);
    // determine min/max of data automatically and use it to set the range of the color-scale
    graph->setAutoImageRange(true);


	
	
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
    plot.setWindowTitle("JKQTPColumnMathImage");


    return app.exec();
}
