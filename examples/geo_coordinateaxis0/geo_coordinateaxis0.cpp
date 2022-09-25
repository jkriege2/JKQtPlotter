/** \example geo_coordinateaxis0.cpp
 * This poriject generates a geometric plot with coordinate axes through zero
 *
 * \ref JKQTPlotterGeometricCoordinateAxis0
 */
#include "jkqtpexampleapplication.h"
#include <QCheckBox>
#include <QComboBox>
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"



int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    JKQTPlotter plot;


    // 4. create a graph in the plot, which plots the dataset X/Y:
    JKQTPGeoPolygon* gp;
    plot.addGraph(gp=new JKQTPGeoPolygon(&plot));
    gp->appendPoint(1,1);
    gp->appendPoint(5,1);
    gp->appendPoint(3,4);
    gp->setLineColor(Qt::blue);
    gp->setLineWidth(1);
    gp->setFillColor(QColor("lightblue"), 0.3);

    plot.addGraph(gp=new JKQTPGeoPolygon(&plot));
    gp->appendPoint(1,2);
    gp->appendPoint(-5,1);
    gp->appendPoint(-3,-2.5);
    gp->appendPoint(6,-2);
    gp->setLineColor(Qt::red);
    gp->setFillColor(QColor("salmon"), 0.3);
    gp->setLineWidth(1);
    qDebug()<<gp->getPointCount();

    JKQTPGeoEllipse* ell;
    plot.addGraph(ell=new JKQTPGeoEllipse(&plot, QPointF(-2,3), QSizeF(7,7)));
    ell->setFillColor(QColor("blue"), 0.2);
    ell->setLineWidth(1);

    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setMaintainAxisAspectRatio(false);

    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.getXAxis()->setDrawMode1(JKQTPCADMnone);
    plot.getXAxis()->setDrawMode2(JKQTPCADMnone);
    plot.getXAxis()->setDrawMode0(JKQTPCADMcomplete|JKQTPCADMMaxFilledArrow);
    plot.getXAxis()->setLabelPosition(JKQTPLabelMax);
    plot.getXAxis()->setAxisLabel("$X$ Coordinate");
    plot.getYAxis()->setDrawMode1(JKQTPCADMnone);
    plot.getYAxis()->setDrawMode2(JKQTPCADMnone);
    plot.getYAxis()->setDrawMode0(JKQTPCADMcomplete|JKQTPCADMMaxFilledArrow);
    plot.getYAxis()->setLabelPosition(JKQTPLabelMax);
    plot.getYAxis()->setAxisLabel("$Y$ Coordinate");

    // 6. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.getPlotter()->setPlotLabel(QObject::tr("Coordinate Axes Through Origin"));
    plot.show();
    plot.resize(290,300);

    return app.exec();
}
