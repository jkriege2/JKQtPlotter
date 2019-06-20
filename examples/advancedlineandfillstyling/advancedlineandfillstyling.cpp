/** \example advancedlineandfillstyling.cpp
 * Shows how to use advanced line styling and filling options with JKQTPlotter
 *
 * \ref JKQTPlotterAdvancedLineAndFillStyling, JKQTPXYLineGraph, JKQTPSpecialLineHorizontalGraph, JKQTPBarVerticalGraph
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpspecialline.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data a vector of x-values for a simple plot (a sine curve)
    QVector<double> X, Yline, Yfilled, Yfilled2, Ybarsempty, Ybarsfilled;
    const int Ndata=20; // number of plot points in each curve
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*4.0*M_PI;
        X<<x;
        Yfilled<<(2.5-0.7*x/4.0/M_PI+sin(x*1.5));
        Yfilled2<<0.75*(1.5-0.7*x/4.0/M_PI+sin(x*1.5));
        double bar=(1.0+0.65*x/4.0/M_PI+sin(x/2.0));
        Ybarsempty<<(-bar);
        Ybarsfilled<<(-bar)*(cos(x)+1.2)/3.0;
    }
    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnYFilled=ds->addCopiedColumn(Yfilled, "Yfilled");
    size_t columnYFilled2=ds->addCopiedColumn(Yfilled2, "Yfilled2");
    size_t columnYBarsEmpty=ds->addCopiedColumn(Ybarsempty, "Ybarsempty");
    size_t columnYBarsFilled=ds->addCopiedColumn(Ybarsfilled, "Ybarsfilled");


    // 3. now we add three semi-transparent, filled curve plots, one for each histogram
    JKQTPSpecialLineHorizontalGraph* graphF=new JKQTPSpecialLineHorizontalGraph(&plot);
    JKQTPSpecialLineHorizontalGraph* graphF2=new JKQTPSpecialLineHorizontalGraph(&plot);
    JKQTPGeoLine* graphL1=new JKQTPGeoLine(&plot, 0, 3, 12, 3.5, Qt::red);
    JKQTPGeoLine* graphL2=new JKQTPGeoLine(&plot, 0, 3.8, 12, 4.5, Qt::red);
    JKQTPGeoLine* graphL3=new JKQTPGeoLine(&plot, 0, 4.6, 12, 5.5, Qt::red);
    JKQTPBarVerticalGraph* graphBE=new JKQTPBarVerticalGraph(&plot);
    JKQTPBarVerticalGraph* graphBF=new JKQTPBarVerticalGraph(&plot);


    // 4. set data
    graphF->setXColumn(columnX); graphF->setYColumn(columnYFilled);
    graphF2->setXColumn(columnX); graphF2->setYColumn(columnYFilled2);
    graphBE->setXColumn(columnX); graphBE->setYColumn(columnYBarsEmpty);
    graphBF->setXColumn(columnX); graphBF->setYColumn(columnYBarsFilled);

    // 5. style the graphs
    // 5.1 set graph titles
    graphL1->setTitle("line 1 graph");
    graphL2->setTitle("line 2 graph");
    graphL3->setTitle("line 3 graph");
    graphF->setTitle("filled curve");
    graphBF->setTitle("bar charts");


    // 5.2 set advanced line style (see also https://doc.qt.io/qt-5/qpen.html#setDashPattern)
    //     and use a gradient-brush to fill the last (thicker) line
    QVector<qreal> dashes1;
    dashes1 << 2 << 2 << 2 << 2 <<  4 << 4 <<  4 << 4 << 8 << 8 << 8 << 8 ;
    graphL1->setLineDashPattern(dashes1);
    graphL1->setLineWidth(2);
    QVector<qreal> dashes2;
    dashes2 << 1 << 2 << 2 << 2 << 3 << 2 << 4 << 2 << 5 << 2 << 6 << 2 ;
    graphL2->setLineDashPattern(dashes2);
    graphL2->setLineWidth(2);
    QVector<qreal> dashes3;
    dashes3 << 4 << 4 << 4 << 4 << 12 << 4 ;
    graphL3->setLineDashPattern(dashes3);
    graphL3->setLineWidth(4);
    QLinearGradient lineGrad(QPointF(0, 0), QPointF(1, 0));
    lineGrad.setColorAt(0, Qt::red);
    lineGrad.setColorAt(0.5, Qt::blue);
    lineGrad.setColorAt(1, Qt::green);
    lineGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    graphL3->setLineBrush(lineGrad);


    // 5.2 set fill style, using an image, also rotate the images
    graphF->setSpecialLineType(JKQTPDirectLine);
    graphF->setDrawLine(true);
    graphF->setFillTexture(QPixmap(":/example.bmp"));
    graphF->setFillTransform(QMatrix(0.5,0,0,0.5,0,0).rotate(45));
    graphF->setFillCurve(true);
    graphF->setLineWidth(0.5);

    // 5.3 set fill style, using a custom linear gradient, also with changing transparency (alpha) values
    graphF2->setSpecialLineType(JKQTPStepCenter);
    graphF2->setDrawLine(false);
    graphF2->setFillCurve(true);
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(1, 1));
    QColor c1(Qt::yellow);
    c1.setAlphaF(0.5);
    QColor c2(Qt::red);
    QColor c3(Qt::blue);
    QColor c4(Qt::green);
    c4.setAlphaF(0.5);
    linearGrad.setColorAt(0, c1);
    linearGrad.setColorAt(0.3, c2);
    linearGrad.setColorAt(0.7, c3);
    linearGrad.setColorAt(1, c4);
    // use this CoordinateMode, so the gradient fills the whole graph area
    linearGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    graphF2->setFillGradient(linearGrad);

    // 5.4 fill barcharts with transparencies and make the surrounding line invisible (colored transparent)
    QColor c0=QColor("darkgreen").darker();
    c0.setAlphaF(.75);
    graphBE->setFillColor(c0);
    graphBE->setLineColor(Qt::transparent);
    QLinearGradient linearG(QPointF(0, 0), QPointF(0, 1));
    QColor cl11(QColor("darkgreen"));
    QColor cl12(Qt::green);
    cl12.setAlphaF(0.5);
    linearG.setColorAt(0, cl11);
    linearG.setColorAt(1, cl12);
    linearG.setCoordinateMode(QGradient::ObjectBoundingMode);
    graphBF->setFillGradient(linearG);
    graphBF->setLineColor(Qt::transparent);


    // 6. add the graphs to the plot, so they are actually displayed
    plot.addGraph(graphF);
    plot.addGraph(graphF2);
    plot.addGraph(graphL1);
    plot.addGraph(graphL2);
    plot.addGraph(graphL3);
    plot.addGraph(graphBE);
    plot.addGraph(graphBF);

    // 7. set plot background with a gradient
    QLinearGradient backGrad(QPointF(0, 0), QPointF(1, 1));
    backGrad.setColorAt(0, QColor("salmon"));
    backGrad.setColorAt(1, QColor("white"));
    backGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    plot.getPlotter()->setPlotBackgroundGradient(backGrad);

    // 8. set axis labels
    plot.getXAxis()->setAxisLabel("x axis");
    plot.getYAxis()->setAxisLabel("y axis");
    plot.setGrid(true);
    plot.getPlotter()->setShowKey(false);


    // 9. scale plot
    plot.setXY(0,11.9,-2.5,5.5);

    // 10. show plotter and make it a decent size
    plot.show();
    plot.resize(600,600);

    return app.exec();
}
