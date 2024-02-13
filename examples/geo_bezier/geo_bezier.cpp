/** \example geo_bezier.cpp
 * Shows how to plot bezier curves as geometric elements with JKQTPlotter
 *
 * \ref JKQTPlotterGeometricBezier
 */

#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpgeolines.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"


int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    // 1. create a plotter window
    JKQTPlotter plot;

    // 2. format graph:
    // 2.1 set the graph scales manually
    plot.setXY(0.05,10.05,0.05,3.15);
    // 2.2 set the asxpect ratio to 1
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setAspectRatio(10.05/3.05);
    plot.getPlotter()->setMaintainAxisAspectRatio(true);
    plot.getPlotter()->setAxisAspectRatio(10.05/3.05);
    // 2.3 set the asxpect ratio to 1
    plot.getXAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawGrid(false);

    auto fx1=[&](double i) { return 0.25+(3.0-i)*9.0/4.0;};
    auto fx2=[&](double i) { return (4.0-i)*9.0/4.0;};

    // 3.1 demonastrate linear bezier curve
    JKQTPGeoBezierCurve* bezLine=new JKQTPGeoBezierCurve(&plot);
    bezLine->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
    bezLine->setLine(QPointF(fx1(0),0.25), QPointF(fx2(0),2.75));
    bezLine->setLineColor(QColor("maroon"));
    plot.addGraph(bezLine);
    JKQTPXYScatterGraph* scatLine=new JKQTPXYScatterGraph(&plot);
    scatLine->setXYColumns(plot.getDatastore()->addCopiedPoints(bezLine->getPoints()));
    scatLine->setSymbolColor(QColor("blue"));
    scatLine->setSymbolType(JKQTPCircle);
    plot.addGraph(scatLine);

    // 3.2 demonastrate quad bezier curve
    JKQTPGeoPolyLines* l1;
    JKQTPGeoBezierCurve* bezQuad=new JKQTPGeoBezierCurve(&plot);
    bezQuad->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
    bezQuad->setQuad(QPointF(fx1(1),0.25), QPointF(fx1(1)+0.25,2.5), QPointF(fx2(1),2.75));
    bezQuad->setLineColor(QColor("maroon"));
    plot.addGraph(bezQuad);
    plot.addGraph(l1=new JKQTPGeoPolyLines(&plot, bezQuad->getPoints()));
    l1->setLineColor(QColor("darkgrey"));
    l1->setLineWidth(1);
    JKQTPXYScatterGraph* scatQuad=new JKQTPXYScatterGraph(&plot);
    scatQuad->setXYColumns(plot.getDatastore()->addCopiedPoints(bezQuad->getPoints()));
    scatQuad->setSymbolColor(QColor("blue"));
    scatQuad->setSymbolType(JKQTPCircle);
    plot.addGraph(scatQuad);

    // 3.3 demonastrate cubic bezier curve
    JKQTPGeoPolyLines* l2;
    JKQTPGeoBezierCurve* bezCubic=new JKQTPGeoBezierCurve(&plot);
    bezCubic->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
    bezCubic->setCubic(QPointF(fx1(2),0.25), QPointF(fx1(2)+0.4,2.5), QPointF(fx2(2)-0.25,0.2), QPointF(fx2(2),2.75));
    bezCubic->setLineColor(QColor("maroon"));
    bezCubic->setHeadDecoratorStyle(JKQTPArrow);
    bezCubic->setTailDecoratorStyle(JKQTPArrow);
    plot.addGraph(bezCubic);
    plot.addGraph(l2=new JKQTPGeoPolyLines(&plot, bezCubic->getPoints()));
    l2->setLineColor(QColor("darkgrey"));
    l2->setLineWidth(1);
    JKQTPXYScatterGraph* scatCubic=new JKQTPXYScatterGraph(&plot);
    scatCubic->setXYColumns(plot.getDatastore()->addCopiedPoints(bezCubic->getPoints()));
    scatCubic->setSymbolColor(QColor("blue"));
    scatCubic->setSymbolType(JKQTPCircle);
    plot.addGraph(scatCubic);


    // 3.4 demonastrate quartic bezier curve
    JKQTPGeoPolyLines* l3;
    JKQTPGeoBezierCurve* bezQuart=new JKQTPGeoBezierCurve(&plot);
    bezQuart->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
    bezQuart->setQuartic(QPointF(fx1(3),2.25), QPointF((fx2(3)+fx1(3))/2.0,2), QPointF(fx1(3),0.75), QPointF((fx2(3)+fx1(3))/2.0,1.5), QPointF(fx2(3),0.2));
    bezQuart->setLineColor(QColor("maroon"));
    bezQuart->setHeadDecoratorStyle(JKQTPArrow);
    bezQuart->setTailDecoratorStyle(JKQTPArrow);
    plot.addGraph(bezQuart);
    plot.addGraph(l3=new JKQTPGeoPolyLines(&plot, bezQuart->getPoints()));
    l3->setLineColor(QColor("darkgrey"));
    l3->setLineWidth(1);
    JKQTPXYScatterGraph* scatQuartic=new JKQTPXYScatterGraph(&plot);
    scatQuartic->setXYColumns(plot.getDatastore()->addCopiedPoints(bezQuart->getPoints()));
    scatQuartic->setSymbolColor(QColor("blue"));
    scatQuartic->setSymbolType(JKQTPCircle);
    plot.addGraph(scatQuartic);



    // 4. show plotter and make it a decent size
    plot.show();
    plot.resize(600/plot.devicePixelRatioF(),250/plot.devicePixelRatioF());


    app.addExportStepFunctor([&](){
        bezLine->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        bezQuad->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        bezCubic->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        bezQuart->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        plot.redrawPlot();
    });

    app.addExportStepFunctor([&](){
        plot.getPlotter()->setMaintainAspectRatio(false);
        plot.getPlotter()->setMaintainAxisAspectRatio(false);
        plot.setXY(0.2,8.8,0.15,3.9);
        bezLine->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        bezQuad->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        bezCubic->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        bezQuart->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        l1->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        l2->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        l3->setDrawMode(JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
        plot.getXAxis()->setLogAxis(true);
        plot.getYAxis()->setLogAxis(true);
        plot.redrawPlot();
    });
    app.addExportStepFunctor([&](){
        plot.getPlotter()->setMaintainAspectRatio(false);
        plot.getPlotter()->setMaintainAxisAspectRatio(false);
        plot.getXAxis()->setLogAxis(true);
        plot.getYAxis()->setLogAxis(true);
        bezLine->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
        bezQuad->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
        bezCubic->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
        bezQuart->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
        l1->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
        l2->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
        l3->setDrawMode(JKQTPGeometricPlotElement::DrawAsGraphicElement);
        plot.redrawPlot();
    });
    return app.exec();
}
