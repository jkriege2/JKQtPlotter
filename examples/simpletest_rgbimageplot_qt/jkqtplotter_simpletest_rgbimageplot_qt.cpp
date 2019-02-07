/** \example jkqtplotter_simpletest_rgbimageplot_qt.cpp
 * JKQTPlotter: Examples: `QImage` as a Graph
 *
 * \ref JKQTPlotterImagePlotQImageRGB
 */

#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsimagergb.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    JKQTPlotter plot;


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering


    // 2. now we open a BMP-file and load it into an OpenCV cv::Mat
    QImage image(":/example.bmp");


    // 3. create a graph (JKQTPImage) with a pointer to the QImage-object, generated above
    JKQTPImage* graph=new JKQTPImage(&plot);
    graph->setTitle("");
    // copy the image into the graph (optionally you could also give a pointer to a QImage,
    // but then you need to ensure that the QImage is available as long as the JKQTPImage
    // instace lives)
    graph->setImage(image);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->setX(0);
    graph->setY(0);
    // width/height of the image in plot coordinates
    graph->setWidth(image.width());
    graph->setHeight(image.height());



    // 4. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);

    // 5. set axis labels
    plot.getXAxis()->setAxisLabel("x [pixels]");
    plot.getYAxis()->setAxisLabel("y [pixels]");

    // 6. fix axis aspect ratio to width/height, so pixels are square
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setAspectRatio(double(image.width())/double(image.height()));

    // 7. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 8. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);
    plot.setWindowTitle("JKQTPImage");


    return app.exec();
}
