/** \example jkqtplotter_simpletest_rgbimageplot_opencv.cpp
 * JKQTPLotter: Examples: Simple RGB image plot, showing a 3-channel OpenCV cv::Mat
 *
 * \ref JKQTPLotterImagePlotRGBOpenCV
 */

#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpopencvinterface.h"
#include <opencv2/imgcodecs.hpp>



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    JKQTPLotter plot;


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    plot.get_plotter()->set_useAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.get_plotter()->set_useAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.get_plotter()->set_useAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* ds=plot.getDatastore();


    // 2. now we open a BMP-file and load it into an OpenCV cv::Mat
    cv::Mat picture = cv::imread("example.bmp");



    // 3. make data available to JKQTPLotter by adding it to the internal datastore.
    //    In this step the contents of each channel of the openCV cv::Mat is copied into a column
    //    of the datastore in row-major order
    size_t cPictureR=JKQTPCopyCvMatToColumn(ds, picture, "R-channel", 2);
    size_t cPictureG=JKQTPCopyCvMatToColumn(ds, picture, "G-channel", 1);
    size_t cPictureB=JKQTPCopyCvMatToColumn(ds, picture, "B-channel", 0);


    // 4. create a graph (JKQTPColumnRGBMathImage) with the columns created above as data
    JKQTPColumnRGBMathImage* graph=new JKQTPColumnRGBMathImage(&plot);
    graph->set_title("");
    // set size of the data (the datastore does not contain this info, as it only manages 1D columns of data and this is used to assume a row-major ordering
    graph->set_Nx(picture.cols);
    graph->set_Ny(picture.rows);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->set_x(0);
    graph->set_y(0);
    // width and height of the image in plot-axis-coordinates
    graph->set_width(picture.cols);
    graph->set_height(picture.rows);
    // image column with the data
    graph->set_imageRColumn(cPictureR);
    graph->set_imageGColumn(cPictureG);
    graph->set_imageBColumn(cPictureB);
    // determine min/max of each channel manually
    graph->set_imageMinR(0);
    graph->set_imageMaxR(255);
    graph->set_imageMinG(0);
    graph->set_imageMaxG(255);
    graph->set_imageMinB(0);
    graph->set_imageMaxB(255);



    // 5. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);

    // 6. set axis labels
    plot.get_xAxis()->set_axisLabel("x [pixels]");
    plot.get_yAxis()->set_axisLabel("y [pixels]");

    // 7. fix axis aspect ratio to width/height, so pixels are square
    plot.get_plotter()->set_maintainAspectRatio(true);
    plot.get_plotter()->set_aspectRatio(double(picture.cols)/double(picture.rows));

    // 8. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);
    plot.setWindowTitle("JKQTPColumnMathImage");


    return app.exec();
}
