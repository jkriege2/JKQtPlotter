/** \example rgbimageplot_cimg.cpp
 * Simple math image plot, showin a 3-channel CImg image
 *
 * \ref JKQTPlotterImagePlotRGBCImg
 */

#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpimagergb.h"
#include "jkqtplotter/jkqtpinterfacecimg.h"
#include "CImg.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    JKQTPlotter plot;


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* ds=plot.getDatastore();


    // 2. now we open a BMP-file and load it into an OpenCV cv::Mat
    cimg_library::CImg<uint8_t> picture; // CImg<T>-Image for the data
    picture.load_bmp("rgbimageplot_opencv_example.bmp");
    qDebug()<<picture.width()<<"x"<<picture.height()<<"x"<<picture.spectrum();



    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    //    In this step the contents of each channel of the openCV cv::Mat is copied into a column
    //    of the datastore in row-major order
    size_t cPictureR=JKQTPCopyCImgToColumn(ds, picture, "R-channel", 0);
    size_t cPictureG=JKQTPCopyCImgToColumn(ds, picture, "G-channel", 1);
    size_t cPictureB=JKQTPCopyCImgToColumn(ds, picture, "B-channel", 2);


    // 4. create a graph (JKQTPColumnRGBMathImage) with the columns created above as data
    JKQTPColumnRGBMathImage* graph=new JKQTPColumnRGBMathImage(&plot);
    graph->setTitle("");
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->setX(0);
    graph->setY(0);
    // width and height of the image in plot-axis-coordinates
    graph->setWidth(picture.width());
    graph->setHeight(picture.height());
    // image column with the data
    graph->setImageRColumn(static_cast<int>(cPictureR));
    graph->setImageGColumn(static_cast<int>(cPictureG));
    graph->setImageBColumn(static_cast<int>(cPictureB));
    // determine min/max of each channel manually
    graph->setImageMinR(0);
    graph->setImageMaxR(255);
    graph->setImageMinG(0);
    graph->setImageMaxG(255);
    graph->setImageMinB(0);
    graph->setImageMaxB(255);



    // 5. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);

    // 6. set axis labels
    plot.getXAxis()->setAxisLabel("x [pixels]");
    plot.getYAxis()->setAxisLabel("y [pixels]");
    // 6.1 invert y-axis, so image is oriented correctly
    plot.getYAxis()->setInverted(true);

    // 7. fix axis aspect ratio to width/height, so pixels are square
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setAspectRatio(double(picture.width())/double(picture.height()));

    // 8. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);
    plot.setWindowTitle("JKQTPColumnRGBMathImage");



    return app.exec();
}
