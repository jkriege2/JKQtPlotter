/** \example jkqtplotter_simpletest_geometric.cpp
 * Shows how to plot several geometric forms with JKQTPlotter
 *
 * \ref JKQTPlotterGeometricGraphs
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsgeometric.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window
    JKQTPlotter plot;

    // 2. format graph:
    // 2.1 set the graph scales manually
    plot.setXY(0,3.05,0,3.05);
    // 2.2 set the asxpect ratio to 1
    plot.getPlotter()->setMaintainAspectRatio(true);
    plot.getPlotter()->setAspectRatio(1);
    plot.getPlotter()->setMaintainAxisAspectRatio(true);
    plot.getPlotter()->setAxisAspectRatio(1);
    // 2.3 set the asxpect ratio to 1
    plot.getXAxis()->set_drawGrid(false);
    plot.getYAxis()->set_drawGrid(false);


    // 3. add some geometric plots
    // 3.1 some text elements, also with formatting by LaTeX expressions
    plot.addGraph(new JKQTPGeoText(&plot, 0.1,0.95, "\\textbf{Text:}", 14, QColor("red")));
    plot.addGraph(new JKQTPGeoText(&plot, 0.1,0.1, "A text object in 10pt", 10, QColor("red")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 0.1,0.1, JKQTPCircle, 5, QColor("grey")));
    plot.addGraph(new JKQTPGeoText(&plot, 0.1,0.3, "A text object in 12pt", 12, QColor("red")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 0.1,0.3, JKQTPCircle, 5, QColor("grey")));
    plot.addGraph(new JKQTPGeoText(&plot, 0.1,0.6, "$x_{1/2}=\\frac{\\sqrt{b^2-4ac}}{2a}$", 10, QColor("red")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 0.1,0.6, JKQTPCircle, 5, QColor("grey")));


    // 3.2 some lines elements (single lines and polylines)
    plot.addGraph(new JKQTPGeoText(&plot, 1.1,0.95, "\\textbf{Lines:}", 14, QColor("red")));
    plot.addGraph(new JKQTPGeoLine(&plot, 1,  0.05, 1.9, 0.9, QColor("red"), 2));
    plot.addGraph(new JKQTPGeoLine(&plot, 1,  0.1, 1.9, 0.8, QColor("blue"), 1, Qt::PenStyle::DashLine));
    QVector<QPointF> p;
    p<<QPointF(1, 0.1)<<QPointF(1.3, 0.9)<<QPointF(1.6, 0.3)<<QPointF(1.9, 0.8);
    plot.addGraph(new JKQTPGeoPolyLines(&plot, p, QColor("darkgreen"), 4, Qt::PenStyle::DashDotLine));
    JKQTPGeoInfiniteLine* infLine=new JKQTPGeoInfiniteLine(&plot, 1.7,  0.2, 0.2, 0.25, QColor("green"), 1.5, Qt::PenStyle::DashLine);
    infLine->set_two_sided(false);
    infLine->setAlpha(0.5);
    plot.addGraph(infLine);
    infLine=new JKQTPGeoInfiniteLine(&plot, 1.7,  0.2, 0.2, -0.15, QColor("blue"), 1.5, Qt::PenStyle::SolidLine);
    infLine->set_two_sided(true);
    infLine->setAlpha(0.5);
    plot.addGraph(infLine);


    // 3.3 some rectangles (you give the center and width/height of the rectangle in the contructor)
    plot.addGraph(new JKQTPGeoText(&plot, 0.1,1.95, "\\textbf{Rectangles:}", 14, QColor("red")));
    plot.addGraph(new JKQTPGeoRectangle(&plot, 0.5,1.5,0.8,0.8, QColor("blue"), 1, Qt::SolidLine, QColor("lightblue")));
    QColor rfill("lightblue"); rfill.setAlphaF(0.5);
    plot.addGraph(new JKQTPGeoRectangle(&plot, QPointF(0.4,1.3), QPointF(0.9,1.6), QColor("blue"), 1, Qt::SolidLine, rfill));
    plot.addGraph(new JKQTPGeoRectangle(&plot, 0.5,1.5,0.8,0.5, 35, QColor("darkblue"), 2, Qt::DashLine));
    plot.addGraph(new JKQTPGeoRectangle(&plot, 0.5,1.5,0.6,0.6, -7, QColor("orange"), 2, Qt::DotLine, QColor("salmon"), Qt::FDiagPattern));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 0.5,1.5, JKQTPCircle, 5, QColor("grey")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 0.4,1.3, JKQTPCircle, 5, QColor("grey")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 0.9,1.6, JKQTPCircle, 5, QColor("grey")));


    // 3.4 some circles and elllipses
    plot.addGraph(new JKQTPGeoText(&plot, 1.1,1.95, "\\textbf{Circles/Ellipses:}", 14, QColor("red")));
    QColor col=QColor("blue"); col.setAlphaF(0.2);
    plot.addGraph(new JKQTPGeoRectangle(&plot, 1.5,1.5,0.8,0.8, col, 0.5, Qt::SolidLine));
    col.setAlphaF(1);
    plot.addGraph(new JKQTPGeoEllipse(&plot, 1.5,1.5,0.8,0.8, col, 1, Qt::SolidLine, QColor("lightblue")));
    col=QColor("blue"); col.setAlphaF(0.2);
    plot.addGraph(new JKQTPGeoRectangle(&plot, QPointF(1.4,1.3), QPointF(1.9,1.6),col, 0.5, Qt::SolidLine));
    col.setAlphaF(1);
    plot.addGraph(new JKQTPGeoEllipse(&plot, QPointF(1.4,1.3), QPointF(1.9,1.6), col, 1, Qt::SolidLine, rfill));
    col=QColor("darkblue"); col.setAlphaF(0.2);
    plot.addGraph(new JKQTPGeoRectangle(&plot, 1.5,1.5,0.8,0.5, 35, col, 0.5, Qt::DashLine));
    col.setAlphaF(1);
    plot.addGraph(new JKQTPGeoEllipse(&plot, 1.5,1.5,0.8,0.5, 35, col, 2, Qt::DashLine));
    col=QColor("orange"); col.setAlphaF(0.2);
    plot.addGraph(new JKQTPGeoRectangle(&plot, 1.5,1.5,0.6,0.6, -7, col, 0.5, Qt::DotLine));
    col.setAlphaF(1);
    plot.addGraph(new JKQTPGeoEllipse(&plot, 1.5,1.5,0.6,0.6, -7, col, 2, Qt::DotLine, QColor("salmon"), Qt::FDiagPattern));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 1.5,1.5, JKQTPCircle, 5, QColor("grey")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 1.4,1.3, JKQTPCircle, 5, QColor("grey")));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 1.9,1.6, JKQTPCircle, 5, QColor("grey")));


    // 3.5 some polygon elements
    plot.addGraph(new JKQTPGeoText(&plot, 2.1,0.95, "\\textbf{PolyLines/Polygons:}", 14, QColor("red")));
    QVector<QPointF> polygon;
    polygon<<QPointF(2, 0.1)<<QPointF(2.3, 0.9)<<QPointF(2.6, 0.3)<<QPointF(2, 0.8);
    JKQTPGeoPolygon* polygraph=new JKQTPGeoPolygon(&plot, polygon, QColor("blue"), 2, Qt::PenStyle::DashLine, QColor("lightblue"));
    polygraph->setAlpha(0.75);
    plot.addGraph(polygraph);
    polygraph=new JKQTPGeoPolygon(&plot, QColor("red"), 2, Qt::PenStyle::DashLine, QColor("salmon"));
    polygraph->appendPoint(2.1, 0.5);
    polygraph->appendPoint(2.9, 0.9);
    polygraph->appendPoint(2.2, 0.8);
    polygraph->appendPoint(2.8, 0.25);
    polygraph->appendPoint(2.6, 0.6);
    polygraph->setAlpha(0.75);
    plot.addGraph(polygraph);


    // 3.6 some arcs
    plot.addGraph(new JKQTPGeoText(&plot, 2.1,1.95, "\\textbf{Arcs:}", 14, QColor("red")));
    plot.addGraph(new JKQTPGeoRectangle(&plot, 2.5,1.5,0.9,0.9, QColor("lightgrey"), 0.5, Qt::SolidLine));
    plot.addGraph(new JKQTPGeoArc(&plot,2.5,1.5,0.9,0.9, 0, 90 , QColor("blue"), 4, Qt::PenStyle::SolidLine));
    plot.addGraph(new JKQTPGeoArc(&plot,2.5,1.5,0.8,0.8, 90,180 , QColor("red"), 4, Qt::PenStyle::SolidLine));
    plot.addGraph(new JKQTPGeoArc(&plot,2.5,1.5,0.7,0.7, 180,270 , QColor("darkblue"), 4, Qt::PenStyle::SolidLine));
    plot.addGraph(new JKQTPGeoArc(&plot,2.5,1.5,0.6,0.6, 270,360 , QColor("maroon"), 4, Qt::PenStyle::DashDotLine));
    plot.addGraph(new JKQTPGeoArc(&plot,2.5,1.5,0.5,0.5, -10, 117 , QColor("orange"), 4, Qt::PenStyle::DashLine));
    plot.addGraph(new JKQTPGeoArc(&plot,2.5,1.5,0.4,0.4, 85, 347 , QColor("darkgreen"), 4, Qt::PenStyle::SolidLine));
    plot.addGraph(new JKQTPGeoSymbol(&plot, 2.5,1.5, JKQTPCircle, 5, QColor("grey")));


    // 3.7 some pies
    plot.addGraph(new JKQTPGeoText(&plot, 2.1,2.95, "\\textbf{Pies:}", 14, QColor("red")));
    plot.addGraph(new JKQTPGeoRectangle(&plot, 2.5,2.5,0.9,0.9, QColor("lightgrey"), 0.5, Qt::SolidLine));
    JKQTPGeoPie* pie;
    plot.addGraph(pie=new JKQTPGeoPie(&plot,2.5,2.5,0.9,0.9, 0, 90 , QColor("blue"), 4, Qt::PenStyle::SolidLine,QColor("lightblue")));
    plot.addGraph(pie=new JKQTPGeoPie(&plot,2.5,2.5,0.7,0.7, 180,270 , QColor("darkblue"), 4, Qt::PenStyle::SolidLine,QColor("blue")));
    pie->setAlpha(0.5);
    plot.addGraph(pie=new JKQTPGeoPie(&plot,2.5,2.5,0.5,0.3, -25, 117 , QColor("orange"), 4, Qt::PenStyle::DashLine,QColor("red")));
    pie->setAlpha(0.5);
    plot.addGraph(pie=new JKQTPGeoPie(&plot,2.5,2.5,0.25,0.8, 85, 347 , QColor("darkgreen"), 4, Qt::PenStyle::SolidLine,QColor("green")));
    pie->setAlpha(0.5);
    plot.addGraph(new JKQTPGeoSymbol(&plot, 2.5,2.5, JKQTPCircle, 5, QColor("grey")));


    // 3.8 some chords
    plot.addGraph(new JKQTPGeoText(&plot, 1.1,2.95, "\\textbf{Chords:}", 14, QColor("red")));
    plot.addGraph(new JKQTPGeoRectangle(&plot, 1.5,2.5,0.9,0.9, QColor("lightgrey"), 0.5, Qt::SolidLine));
    JKQTPGeoChord* chord;
    plot.addGraph(chord=new JKQTPGeoChord(&plot,1.5,2.5,0.9,0.9, 0, 90 , QColor("blue"), 4, Qt::PenStyle::SolidLine,QColor("lightblue")));
    plot.addGraph(chord=new JKQTPGeoChord(&plot,1.5,2.5,0.7,0.7, 180,270 , QColor("darkblue"), 4, Qt::PenStyle::SolidLine,QColor("blue")));
    chord->setAlpha(0.5);
    plot.addGraph(chord=new JKQTPGeoChord(&plot,1.5,2.5,0.5,0.3, -25, 117 , QColor("orange"), 4, Qt::PenStyle::DashLine,QColor("red")));
    chord->setAlpha(0.5);
    plot.addGraph(chord=new JKQTPGeoChord(&plot,1.5,2.5,0.25,0.8, 85, 347 , QColor("darkgreen"), 4, Qt::PenStyle::SolidLine,QColor("green")));
    chord->setAlpha(0.5);
    plot.addGraph(new JKQTPGeoSymbol(&plot, 1.5,2.5, JKQTPCircle, 5, QColor("grey")));


    // 4. show plotter and make it a decent size
    plot.show();
    plot.resize(800,800);

    return app.exec();
}
