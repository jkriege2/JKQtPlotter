/** \example jkqtplotter_simpletest_filledgraphs.cpp
 * Shows how to use filled graphs with JKQTPlotter
 *
 * \ref JKQTPlotterFilledGraphs
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsfilledcurve.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create 4 datacolumns with length 256 entries in the datastore
    //    these will later hold the RGB-histogram and a linear x-values vector
    //      - the x-values are directly initialized as linear vector 0..255 in 256 steps
    //      - the other columns are generated and size_t-type indexes are stored for later
    //        reference to these columns in the graphs-
    size_t columnX=ds->addLinearColumn(256, 0, 255, "x");
    size_t columnR=ds->addColumn(256, "historam_R");
    size_t columnG=ds->addColumn(256, "historam_G");
    size_t columnB=ds->addColumn(256, "historam_B");
    //      - in addition JKQTPColumn objects are generated, which can be used to access
    //        the data in the columns
    JKQTPColumn cG=ds->getColumn(columnG);
    JKQTPColumn cR=ds->getColumn(columnR);
    JKQTPColumn cB=ds->getColumn(columnB);
    //      - now all columns for RGB are initialized to 0
    cR.setAll(0);
    cG.setAll(0);
    cB.setAll(0);

    // 3. now we open a BMP-file and load it into a QImage
    QImage image(":/example.bmp");
    // ... and calculate the RGB-histograms
    for (int y=0; y<image.height(); y++) {
        for (int x=0; x<image.width(); x++) {
            QRgb pix=image.pixel(x,y);
            cR.incValue(qRed(pix), 1);
            cG.incValue(qGreen(pix), 1);
            cB.incValue(qBlue(pix), 1);
        }
    }
    // ... and normalize histograms
    cR.scale(100.0/static_cast<double>(image.width()*image.height()));
    cG.scale(100.0/static_cast<double>(image.width()*image.height()));
    cB.scale(100.0/static_cast<double>(image.width()*image.height()));


    // 4. now we add three semi-transparent, filled curve plots, one for each histogram
    JKQTPFilledCurveXGraph* graphR=new JKQTPFilledCurveXGraph(&plot);
    JKQTPFilledCurveXGraph* graphG=new JKQTPFilledCurveXGraph(&plot);
    JKQTPFilledCurveXGraph* graphB=new JKQTPFilledCurveXGraph(&plot);

    // set graph titles
    graphR->setTitle("R-channel");
    graphG->setTitle("G-channel");
    graphB->setTitle("B-channel");

    // set graph colors (lines: non-transparent, fill: semi-transparent) and style
    QColor col;
    col=QColor("red"); graphR->setColor(col);
    col.setAlphaF(0.25); graphR->setFillColor(col);
    col=QColor("green"); graphG->setColor(col);
    col.setAlphaF(0.25); graphG->setFillColor(col);
    col=QColor("blue"); graphB->setColor(col);
    col.setAlphaF(0.25); graphB->setFillColor(col);
    graphR->setLineWidth(1);
    graphG->setLineWidth(1);
    graphB->setLineWidth(1);

    // set data
    graphR->setXColumn(columnX); graphR->setYColumn(columnR);
    graphG->setXColumn(columnX); graphG->setYColumn(columnG);
    graphB->setXColumn(columnX); graphB->setYColumn(columnB);


    // add the graphs to the plot, so they are actually displayed
    plot.addGraph(graphB);
    plot.addGraph(graphG);
    plot.addGraph(graphR);

    // 5. set axis labels
    plot.getXAxis()->setAxisLabel("R/G/B-value");
    plot.getYAxis()->setAxisLabel("normalized frequency [%]");


    // 4. set the maximum size of the plot to 0..100% and 0..256
    plot.setAbsoluteX(0,256);
    plot.setAbsoluteY(0,100);
    // ... and scale plot automatically
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
