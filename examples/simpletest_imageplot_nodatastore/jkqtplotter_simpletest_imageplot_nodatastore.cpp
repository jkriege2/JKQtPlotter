#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsimage.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    JKQtPlotter plot;
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    plot.get_plotter()->set_useAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.get_plotter()->set_useAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.get_plotter()->set_useAntiAliasingForText(true); // nicer (but slower) text rendering

    // 2. now we create data for the charts (taken from https://commons.wikimedia.org/wiki/File:Energiemix_Deutschland.svg)
    const int NX=100; // image dimension in x-direction [pixels]
    const int NY=100; // image dimension in x-direction [pixels]
    const double dx=1e-2; // size of a pixel in x-direction [micrometers]
    const double dy=1e-2; // size of a pixel in x-direction [micrometers]
    const double w=static_cast<double>(NX)*dx;
    const double h=static_cast<double>(NY)*dy;
    double airydisk[NX*NY]; // row-major image

    // 2.1 Parameters for airy disk plot (see https://en.wikipedia.org/wiki/Airy_disk)
    double NA=1.1; // numerical aperture of lens
    double wavelength=488e-3; // wavelength of the light [micrometers]

    // 2.2 calculate image of airy disk in a row-major array
    double x, y=-h/2.0;
    for (int iy=0; iy<NY; iy++ ) {
        x=-w/2.0;
        for (int ix=0; ix<NX; ix++ ) {
            const double r=sqrt(x*x+y*y);
            const double v=2.0*M_PI*NA*r/wavelength;
            airydisk[iy*NX+ix] = pow(2.0*j1(v)/v, 2);
            x+=dx;
        }
        y+=dy;
    }


    // 3. create a graph (JKQTPMathImage) with referencing the data created above as data
    //    The data is color-coded with the color-palette JKQTPMathImageBLUEYELLOW
    //    the converted range of data is determined automatically because set_autoImageRange(true)
    JKQTPMathImage* graph=new JKQTPMathImage(&plot);
    graph->set_title("");
    // set array as datasource, with correct size NX*NY and datatype JKQTPMathImageBase::DataType::DoubleArray
    graph->set_data(airydisk, NX, NY, JKQTPMathImageBase::DataType::DoubleArray);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->set_x(-w/2.0);
    graph->set_y(-h/2.0);
    // width and height of the image in plot-axis-coordinates
    graph->set_width(w);
    graph->set_height(h);
    // color-map is "BLUEYELLOW"
    graph->set_palette(JKQTPMathImageBLUEYELLOW);
    // get coordinate axis of color-bar and set its label
    graph->get_colorBarRightAxis()->set_axisLabel("light intensity [A.U.]");
    // determine min/max of data automatically and use it to set the range of the color-scale
    graph->set_autoImageRange(true);
    // you can set the color-scale range manually by using:
    //   graph->set_autoImageRange(false);
    //   graph->set_imageMin(0);
    //   graph->set_imageMax(10);
	
	
    // 4. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);

	
    // 5. set axis labels
    plot.get_xAxis()->set_axisLabel("x [{\\mu}m]");
    plot.get_yAxis()->set_axisLabel("y [{\\mu}m]");

	
    // 6. fix axis and plot aspect ratio to 1
    plot.get_plotter()->set_maintainAspectRatio(true);
    plot.get_plotter()->set_maintainAxisAspectRatio(true);

    // 7. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600,600);
    plot.setWindowTitle("JKQTPMathImage");


    return app.exec();
}
