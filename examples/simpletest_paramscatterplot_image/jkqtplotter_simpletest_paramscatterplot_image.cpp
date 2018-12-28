#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsimage.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQtPlotter plot;
    JKQTPdatastore* ds=plot.getDatastore();

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
    JKQTPxyParametrizedScatterGraph* graph1=new JKQTPxyParametrizedScatterGraph(&plot);
    graph1->set_xColumn(columnX);
    graph1->set_yColumn(columnY);
    graph1->set_sizeColumn(columnS);
    graph1->set_symbolColumn(columnS);
    graph1->set_symbol(JKQTPfilledTriangle);
    graph1->set_colorColumn(columnG);
    graph1->set_palette(JKQTPMathImageOCEAN);
    graph1->set_drawLine(false);
    graph1->set_title("");
    plot.addGraph(graph1);

    // 3.2 add an image to display the original graphics
    JKQTPImage* graph2=new JKQTPImage(&plot);
    graph2->set_image(image);
    graph2->set_x(0);
    graph2->set_y(0);
    graph2->set_width(10);
    graph2->set_height(10);
    plot.addGraph(graph2);


    // 4. scale the plot so the graph is contained and format the coordinate system
    plot.get_xAxis()->set_axisLabel("x-axis");
    plot.get_yAxis()->set_axisLabel("y-axis");
    plot.get_xAxis()->set_drawGrid(false);
    plot.get_yAxis()->set_drawGrid(false);
    // max. size is the size of the image
    plot.setXY(0,image.width()-1,0,image.height()-1);
    plot.setAbsoluteXY(0,image.width()-1,0,image.height()-1);
    // ensure that axis aspect ration and coordinate system aspect ratio are maintained
    plot.get_plotter()->set_maintainAspectRatio(true);
    plot.get_plotter()->set_aspectRatio(1);
    plot.get_plotter()->set_maintainAxisAspectRatio(true);
    plot.get_plotter()->set_axisAspectRatio(1);

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(800,800);

    return app.exec();
}
