/** \example geo_arrows.cpp
 * Shows how to plot arrows as geometric elements with JKQTPlotter
 *
 * \ref JKQTPlotterGeometricArrows
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window
    JKQTPlotter plot;

    // 2. format graph:
    // 2.1 set the graph scales manually
    plot.setXY(0,4.05,0,3.15);
    // 2.2 set the asxpect ratio to 1
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setAspectRatio(4.05/3.05);
    plot.getPlotter()->setMaintainAxisAspectRatio(true);
    plot.getPlotter()->setAxisAspectRatio(4.05/3.05);
    // 2.3 set the asxpect ratio to 1
    plot.getXAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawGrid(false);


    // 3.1 demonastrate all arrow heads
    JKQTPGeoArrow* a;
    double arr_y=0.1;
    double arr_deltay=2.9/static_cast<double>(JKQTPLineDecoratorCount);
    plot.addGraph(new JKQTPGeoText(&plot, 0.1, 3.0, "w_{line}=0.2", 12, QColor("darkred")));
    plot.addGraph(new JKQTPGeoText(&plot, 0.4, 3.0, "w_{line}=0.5", 12, QColor("darkred")));
    plot.addGraph(new JKQTPGeoText(&plot, 0.7, 3.0, "w_{line}=1", 12, QColor("darkred")));
    plot.addGraph(new JKQTPGeoText(&plot, 1.0, 3.0, "w_{line}=2", 12, QColor("darkred")));
    plot.addGraph(new JKQTPGeoText(&plot, 1.4, 3.0, "w_{line}=3", 12, QColor("darkred")));
    for (size_t i=0; i<static_cast<size_t>(JKQTPLineDecoratorCount); i++) {
        auto const decor=static_cast<JKQTPLineDecoratorStyle>(i);
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 0.1,  arr_y, 0.3, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 0.2));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 0.4,  arr_y, 0.6, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 0.5));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 0.7,  arr_y, 0.9, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 1));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 1.0,  arr_y, 1.3, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 2));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 1.4,  arr_y, 1.7, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 3));
        plot.addGraph(new JKQTPGeoText(&plot, a->getX2()+0.05, a->getY2(), "\\verb{"+JKQTPLineDecoratorStyle2String(decor)+"}", 12, a->getLineColor()));
        arr_y+=arr_deltay;
    }

    // 3.2 note that all decorators are drawn in a way that lets them end at the intended end-point of the line, even circles!
    for (size_t i=0; i<6; i++) {
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 3+static_cast<double>(i)*0.15,  3, 3+static_cast<double>(i)*0.15, 2.75, QColor("blue"), static_cast<JKQTPLineDecoratorStyle>(i*4+1), static_cast<JKQTPLineDecoratorStyle>(i*4+2), static_cast<double>(i+1)*0.5));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 3+static_cast<double>(i)*0.15,  2.5, 3+static_cast<double>(i)*0.15, 2.75, QColor("red"), static_cast<JKQTPLineDecoratorStyle>(i*4+3), static_cast<JKQTPLineDecoratorStyle>(i*4+4), static_cast<double>(i+1)*0.5));
    }
    plot.addGraph(new JKQTPGeoLine(&plot, 2.9,  2.5, 4.0, 2.5, QColor("silver"), 0.25, Qt::DashLine));
    plot.addGraph(new JKQTPGeoLine(&plot, 2.9,  2.75, 4.0, 2.75, QColor("silver"), 0.25, Qt::DashLine));
    plot.addGraph(new JKQTPGeoLine(&plot, 2.9,  3, 4.6, 3, QColor("silver"), 0.25, Qt::DashLine));

    // 3.3 a JKQTPGeoLine with and without errow heads:
    JKQTPGeoLine* line=new JKQTPGeoLine(&plot, 2.9,  2.3, 3.6, 2.4, QColor("red"), 2.0);
    JKQTPGeoLine* line_heads=new JKQTPGeoLine(&plot, 2.9,  2.1, 3.6, 2.2, QColor("red"), 2.0);
    line_heads->setHeadDecoratorStyle(JKQTPFilledDoubleArrow);
    line_heads->setTailDecoratorStyle(JKQTPCircleDecorator);
    plot.addGraph(line);
    plot.addGraph(line_heads);
    plot.addGraph(new JKQTPGeoSymbol(&plot, line->getX1(), line->getY1(), JKQTPCirclePlus, 8, QColor("silver")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, line->getX2(), line->getY2(), JKQTPCirclePlus, 8, QColor("silver")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, line_heads->getX1(), line_heads->getY1(), JKQTPCirclePlus, 8, QColor("silver")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, line_heads->getX2(), line_heads->getY2(), JKQTPCirclePlus, 8, QColor("silver")));


    // 3.4 also the class JKQTPGeoPolyLines can decorate its start/end with arrows
    QVector<QPointF> points; points<<QPointF(3,  0.8)<<QPointF(4,  0.7)<<QPointF(3,  1.2)<<QPointF(4,  1.0);
    JKQTPGeoPolyLines* polyLine=new JKQTPGeoPolyLines(&plot, points, QColor("blue"));
    polyLine->setHeadDecoratorStyle(JKQTPFilledDoubleArrow);
    polyLine->setTailDecoratorStyle(JKQTPCircleDecorator);
    plot.addGraph(polyLine);


    // 3.5 also the class JKQTPGeoInfiniteLine can decorate its start with arrows (only if two_sided==false!)
    JKQTPGeoInfiniteLine* infLine=new JKQTPGeoInfiniteLine(&plot, 3.5, 0.2, 1, 0.25, QColor("blue"), 2);
    infLine->setHeadDecoratorStyle(JKQTPFilledDoubleArrow);
    plot.addGraph(infLine);
    plot.addGraph(new JKQTPGeoSymbol(&plot, infLine->getX(), infLine->getY(), JKQTPCirclePlus, 8, QColor("silver")));
    plot.addGraph(new JKQTPGeoArrow(&plot, infLine->getX(), infLine->getY(), infLine->getX()+infLine->getDx(), infLine->getY()+infLine->getDy(), QColor("silver"), JKQTPArrow, JKQTPNoDecorator, 1));
/*
    infLine=new JKQTPGeoInfiniteLine(&plot, 3.5, 0.1, -1, 0.25, QColor("darkblue"), 2);
    infLine->setHeadDecoratorStyle(JKQTPFilledDoubleArrow);
    infLine->setTwoSided(true);
    plot.addGraph(infLine);
    plot.addGraph(new JKQTPGeoSymbol(&plot, infLine->getX(), infLine->getY(), JKQTPCirclePlus, 8, QColor("silver")));
    plot.addGraph(new JKQTPGeoArrow(&plot, infLine->getX(), infLine->getY(), infLine->getX()+infLine->getDx(), infLine->getY()+infLine->getDy(), QColor("silver"), JKQTPArrow, JKQTPNoDecorator, 1));
*/


    // 4. show plotter and make it a decent size
    plot.show();
    plot.resize(1200,800);

    return app.exec();
}
