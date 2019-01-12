# JKQtPlotter: Examples: Simple math image plot, showin a 1-channel OpenCV cv::Mat {#JKQtPlotterImagePlotOpenCV}
This project (see `./examples/simpletest_imageplot_opencv/`) simply creates a JKQtPlotter widget (as a new window) and adds a color-coded image plot of a mathematical function (here the Airy disk). The image is generated as an OpenCV cv::Mat image and then copied into a single column of the internal datasdtore (JKQTPMathImage could be directly used without the internal datastore). 
To copy the data a special OpenCV Interface function `JKQTPcopyCvMatToColumn()` is used, that copies the data from a cv::Mat directly into a column. 

The function `JKQTPcopyCvMatToColumn()` is available from the (non-default) header-only extension from `jkqtplotter/jkqtpopencvinterface.h`. This header provides facilities to interface JKQtPlotter with OPenCV.

The source code of the main application is (see [`jkqtplotter_simpletest_imageplot_opencv.cpp`](../simpletest_imageplot_opencv/jkqtplotter_simpletest_imageplot_opencv.cpp):
```c++
#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpopencvinterface.h"
#include <opencv/cv.h>

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
    JKQTPdatastore* ds=plot.getDatastore();

    
    // 2. now we create data for the charts (taken from https://commons.wikimedia.org/wiki/File:Energiemix_Deutschland.svg)
    cv::Mat airydisk(150, 150, CV_64FC1); // OpenCV-Image for the data
    const double dx=1e-2; // size of a pixel in x-direction [micrometers]
    const double dy=1e-2; // size of a pixel in x-direction [micrometers]
    const double w=static_cast<double>(airydisk.cols)*dx;
    const double h=static_cast<double>(airydisk.rows)*dy;

    // 2.1 Parameters for airy disk plot (see https://en.wikipedia.org/wiki/Airy_disk)
    double NA=1.1; // numerical aperture of lens
    double wavelength=488e-3; // wavelength of the light [micrometers]

    // 2.2 calculate image of airy disk in a row-major array
    double x, y=-h/2.0;
    for (int iy=0; iy<airydisk.rows; iy++ ) {
        x=-w/2.0;
        for (int ix=0; ix<airydisk.cols; ix++ ) {
            const double r=sqrt(x*x+y*y);
            const double v=2.0*M_PI*NA*r/wavelength;
            airydisk.at<double>(iy,ix) = pow(2.0*j1(v)/v, 2);
            x+=dx;
        }
        y+=dy;
    }


    // 3. make data available to JKQtPlotter by adding it to the internal datastore.
    //    In this step the contents of one channel of the openCV cv::Mat is copied into a column
    //    of the datastore in row-major order
    size_t cAiryDisk=JKQTPcopyCvMatToColumn(ds, airydisk, "imagedata");

    
    // 4. create a graph (JKQTPColumnMathImage) with the column created above as data
    //    The data is color-coded with the color-palette JKQTPMathImageMATLAB
    //    the converted range of data is determined automatically because set_autoImageRange(true)
    JKQTPColumnMathImage* graph=new JKQTPColumnMathImage(&plot);
    graph->set_title("");
    // image column with the data
    graph->set_imageColumn(cAiryDisk);
    // set size of the data (the datastore does not contain this info, as it only manages 1D columns of data and this is used to assume a row-major ordering
    graph->set_Nx(airydisk.cols);
    graph->set_Ny(airydisk.rows);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->set_x(-w/2.0);
    graph->set_y(-h/2.0);
    // width and height of the image in plot-axis-coordinates
    graph->set_width(w);
    graph->set_height(h);
    // color-map is "MATLAB"
    graph->set_palette(JKQTPMathImageMATLAB);
    // get coordinate axis of color-bar and set its label
    graph->get_colorBarRightAxis()->set_axisLabel("light intensity [A.U.]");
    // determine min/max of data automatically and use it to set the range of the color-scale
    graph->set_autoImageRange(true);
    // you can set the color-scale range manually by using:
    //   graph->set_autoImageRange(false);
    //   graph->set_imageMin(0);
    //   graph->set_imageMax(10);

    
    // 5. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);

    // 6. set axis labels
    plot.get_xAxis()->set_axisLabel("x [{\\mu}m]");
    plot.get_yAxis()->set_axisLabel("y [{\\mu}m]");

    // 7. fix axis and plot aspect ratio to 1
    plot.get_plotter()->set_maintainAspectRatio(true);
    plot.get_plotter()->set_maintainAxisAspectRatio(true);

    // 8 autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600,600);
    plot.setWindowTitle("JKQTPColumnMathImage");


    return app.exec();
}

```
The result looks like this:

![jkqtplotter_simpletest_imageplot](../../screenshots/jkqtplotter_simpletest_imageplot_opencv.png)

See [`test/simpletest_imageplot`](../simpletest_imageplot) for a detailed description of the other possibilities that the class JKQTPColumnMathImage (and also JKQTPMathImage) offer with respect to determining how an image is plottet.

