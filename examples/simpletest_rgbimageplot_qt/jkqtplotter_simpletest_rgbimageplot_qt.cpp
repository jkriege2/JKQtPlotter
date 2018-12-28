#include <QApplication>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsimage.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    JKQtPlotter plot;


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    plot.get_plotter()->set_useAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.get_plotter()->set_useAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.get_plotter()->set_useAntiAliasingForText(true); // nicer (but slower) text rendering


    // 2. now we open a BMP-file and load it into an OpenCV cv::Mat
    QImage image(":/example.bmp");


    // 3. create a graph (JKQTPImage) with a pointer to the QImage-object, generated above
    JKQTPImage* graph=new JKQTPImage(&plot);
    graph->set_title("");
    // copy the image into the graph (optionally you could also give a pointer to a QImage,
    // but then you need to ensure that the QImage is available as long as the JKQTPImage
    // instace lives)
    graph->set_image(image);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->set_x(0);
    graph->set_y(0);
    // width/height of the image in plot coordinates
    graph->set_width(image.width());
    graph->set_height(image.height());



    // 4. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);

    // 5. set axis labels
    plot.get_xAxis()->set_axisLabel("x [pixels]");
    plot.get_yAxis()->set_axisLabel("y [pixels]");

    // 6. fix axis aspect ratio to width/height, so pixels are square
    plot.get_plotter()->set_maintainAspectRatio(true);
    plot.get_plotter()->set_aspectRatio(double(image.width())/double(image.height()));

    // 7. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 8. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);
    plot.setWindowTitle("JKQTPImage");


    return app.exec();
}
