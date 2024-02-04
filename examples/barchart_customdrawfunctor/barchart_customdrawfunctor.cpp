/** \example barchart_functorfill.cpp
 * Shows how to draw Barcharts with colors defined by a functor JKQTPlotter
 *
 * \ref JKQTPlotterBarchartsFunctorFilling
 */

#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtcommon/jkqtpmathtools.h"
#include "jkqtpexampleapplication.h"



template <class TCHART>
void doExample(JKQTPlotter& plot, const QString& title)
{
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create two columns for key and value
    size_t columnK=ds->addLinearColumn(7, 0, 6, "k");
    size_t columnV=ds->addCalculatedColumnFromColumn(columnK, [](double x) { return jkqtp_sign(2.0*x-2.5)*qMax(2.0,fabs(2.0*x-2.5)); }, "v");

    // 3. load a stylesheet
    plot.loadCurrentPlotterStyle(QSettings(":/JKQTPlotter/styles/seaborn.ini", QSettings::IniFormat));

    // 4. create graph in the plot, which plots the dataset:
    JKQTPBarGraphBase* graph=new TCHART(&plot);
    graph->setKeyColumn(columnK);
    graph->setValueColumn(columnV);
    // this is the custom draw functor
    graph->setCustomDrawingFunctor(
    [](JKQTPEnhancedPainter& painter, const QRectF& bar_px, const QPointF& datapoint, Qt::Orientation orientation, JKQTPBarGraphBase* graph) {
        painter.save();
        painter.setPen(Qt::NoPen);
        QBrush b=painter.brush();
        QBrush bDeco=b;
        bDeco.setColor(b.color().lighter());
        // we draw the bar with a rounded end and add a lightly colored circle near the top
        if (orientation==Qt::Vertical) {
            // for vertical barcharts
            const double dx=bar_px.width()/2.0;
            const double r=dx*0.85;
            // depending in whether the bar elongates above or below the baseline,
            // we have to align the circle wrt the top or the bottom of the rectangle bar_px
            if (datapoint.y()>=graph->getBaseline()) {
                painter.drawComplexRoundedRect(bar_px, dx, dx, 0, 0, Qt::AbsoluteSize);
                painter.setBrush(bDeco);
                painter.drawEllipse(QPointF(bar_px.center().x(), bar_px.top()+dx), r,r);
            } else {
                painter.drawComplexRoundedRect(bar_px, 0, 0, dx, dx, Qt::AbsoluteSize);
                painter.setBrush(bDeco);
                painter.drawEllipse(QPointF(bar_px.center().x(), bar_px.bottom()-dx), r,r);
            }
        } else {
            // for horizontal barcharts
            const double dx=bar_px.height()/2.0;
            const double r=dx*0.85;
            if (datapoint.x()>=graph->getBaseline()) {
                painter.drawComplexRoundedRect(bar_px, 0, dx, 0, dx, Qt::AbsoluteSize);
                painter.setBrush(bDeco);
                painter.drawEllipse(QPointF(bar_px.right()-dx, bar_px.center().y()), r,r);
            } else {
                painter.drawComplexRoundedRect(bar_px, dx,0,dx,0, Qt::AbsoluteSize);
                painter.setBrush(bDeco);
                painter.drawEllipse(QPointF(bar_px.left()+dx, bar_px.center().y()), r,r);
            }
        }
        painter.restore();
    }
    );
    // enable usage of cutom draw functor
    graph->setUseCustomDrawFunctor(true);
    // set graph color
    graph->setColor(QColor("blue"));
    plot.addGraph(graph);


    // 5. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 6. show plotter and make it a decent size
    plot.setWindowTitle(title);
    plot.show();
    plot.resize(400,400);
}

int main(int argc, char* argv[])
{

    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    JKQTPlotter plotV, plotH;
    doExample<JKQTPBarVerticalGraph>(plotV, "1: JKQTPBarVerticalGraph");
    doExample<JKQTPBarHorizontalGraph>(plotH, "2: JKQTPBarHorizontalGraph");

    return app.exec();
}
