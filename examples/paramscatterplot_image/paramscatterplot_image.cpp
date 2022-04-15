/** \example paramscatterplot_image.cpp
 * JKQTPlotter: Examples: Draw an Artistic Image with a Parametrized Scatter Graph
 *
 * \ref JKQTPlotterParamScatterImage
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpimage.h"


int main(int argc, char* argv[])
{
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. Prepare Data
    // 2.1 load image
    QImage image(":/example.bmp");
    QVector<double> imageVector, pointSizes;
    QVector<double> X,Y;

    // 2.2 convert image to greyscale, stored in a vector in row-major order
    double maxSymbolSize=30; // maximal diameter of symbols in pt
    for (int y=0; y<image.height(); y++) {
        for (int x=0; x<image.width(); x++) {
            // calculate grey-value image vector
            imageVector.push_back(qGray(image.pixel(x,y)));
            // calculate point sizes from inverse grey value and scaling between 0 and maxSymbolSize
            pointSizes.push_back(static_cast<double>(255-qGray(image.pixel(x,y)))/255.0*maxSymbolSize);
            // calculate X/Y-coordinates (y mirrored, so image is upright)
            X.push_back(x);
            Y.push_back(image.height()-1-y);
        }
    }

    // 2.3 and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY=ds->addCopiedColumn(Y, "y");
    size_t columnG=ds->addCopiedColumn(imageVector, "greyscaleImageData");
    size_t columnS=ds->addCopiedColumn(pointSizes, "pointSizes");

    // 3. add graphs to plot
    // 3.1 Now add a parametrized scatter graph with columnX, columnY for the positions of the
    //     scatter points, where the symbol size is given by column columnS and the color of
    //     each symbol is set from column columnG, via a color palette JKQTPMathImageMATLAB
    JKQTPXYParametrizedScatterGraph* graph1=new JKQTPXYParametrizedScatterGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY);
    graph1->setSizeColumn(columnS);
    graph1->setSymbolColumn(columnS);
    graph1->setSymbolType(JKQTPFilledTriangle);
    graph1->setColorColumn(columnG);
    graph1->setColorPalette(JKQTPMathImageOCEAN);
    graph1->setDrawLine(false);
    graph1->setTitle("");
    plot.addGraph(graph1);

    // 3.2 add an image to display the original graphics
    JKQTPImage* graph2=new JKQTPImage(&plot);
    graph2->setImage(image);
    graph2->setX(0);
    graph2->setY(0);
    graph2->setWidth(10);
    graph2->setHeight(10);
    plot.addGraph(graph2);


    // 4. scale the plot so the graph is contained and format the coordinate system
    plot.getXAxis()->setAxisLabel("x-axis");
    plot.getYAxis()->setAxisLabel("y-axis");
    plot.getXAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawGrid(false);
    // max. size is the size of the image
    plot.setXY(0,image.width()-1,0,image.height()-1);
    plot.setAbsoluteXY(0,image.width()-1,0,image.height()-1);
    // ensure that axis aspect ratio and coordinate system aspect ratio are maintained
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setAspectRatio(1);
    plot.getPlotter()->setMaintainAxisAspectRatio(true);
    plot.getPlotter()->setAxisAspectRatio(1);

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(800,800);

    return app.exec();
}
