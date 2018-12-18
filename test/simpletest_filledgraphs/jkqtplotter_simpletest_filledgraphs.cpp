#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsfilledcurve.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQtPlotter plot;
    JKQTPdatastore* ds=plot.getDatastore();

    // 2. now we create 4 datacolumns with length 256 entries in the datastore
    //    these will later hold the RGB-histogram and a linear x-values vector
    //      - the x-values are directly initialized as linear vector 0..255 in 256 steps
    //      - the other columns are generated and size_t-type indexes are stored for later
    //        reference to these columns in the graphs-
    size_t columnX=ds->addLinearColumn(256, 0, 255, "x");
    size_t columnR=ds->addColumn(256, "historam_R");
    size_t columnG=ds->addColumn(256, "historam_G");
    size_t columnB=ds->addColumn(256, "historam_B");
    //      - in addition JKQTPcolumn objects are generated, which can be used to access
    //        the data in the columns
    JKQTPcolumn cG=ds->getColumn(columnG);
    JKQTPcolumn cR=ds->getColumn(columnR);
    JKQTPcolumn cB=ds->getColumn(columnB);
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
    JKQTPfilledCurveXGraph* graphR=new JKQTPfilledCurveXGraph(&plot);
    JKQTPfilledCurveXGraph* graphG=new JKQTPfilledCurveXGraph(&plot);
    JKQTPfilledCurveXGraph* graphB=new JKQTPfilledCurveXGraph(&plot);

    // set graph titles
    graphR->set_title("R-channel");
    graphG->set_title("G-channel");
    graphB->set_title("B-channel");

    // set graph colors (lines: non-transparent, fill: semi-transparent) and style
    QColor col;
    col=QColor("red"); graphR->set_color(col);
    col.setAlphaF(0.25); graphR->set_fillColor(col);
    col=QColor("green"); graphG->set_color(col);
    col.setAlphaF(0.25); graphG->set_fillColor(col);
    col=QColor("blue"); graphB->set_color(col);
    col.setAlphaF(0.25); graphB->set_fillColor(col);
    graphR->set_lineWidth(1);
    graphG->set_lineWidth(1);
    graphB->set_lineWidth(1);

    // set data
    graphR->set_xColumn(columnX); graphR->set_yColumn(columnR);
    graphG->set_xColumn(columnX); graphG->set_yColumn(columnG);
    graphB->set_xColumn(columnX); graphB->set_yColumn(columnB);


    // add the graphs to the plot, so they are actually displayed
    plot.addGraph(graphB);
    plot.addGraph(graphG);
    plot.addGraph(graphR);

    // 5. set axis labels
    plot.getXAxis()->set_axisLabel("R/G/B-value");
    plot.getYAxis()->set_axisLabel("normalized frequency [%]");


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
