/** \example geometric.cpp
 * Shows how to plot several geometric forms with JKQTPlotter
 *
 * \ref JKQTPlotterGeometricGraphs
 */
#include <QCheckBox>
#include <QComboBox>
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a Widget with a plotter and some control-elements
    QWidget widMain;
    QGridLayout* layMain=new QGridLayout;
    widMain.setLayout(layMain);
    JKQTPlotter* plot=new JKQTPlotter(&widMain);
    layMain->addWidget(plot, 1,0,1,5);
    layMain->setRowStretch(1,1);
    QCheckBox* chk;
    // when checkbox is toggled: aspect-ration is maintained, or not
    layMain->addWidget(chk=new QCheckBox(QObject::tr("maintain aspect ratio"), &widMain), 0, 0);
    chk->setChecked(true);
    QObject::connect(chk, &QCheckBox::toggled, [plot](bool checked) {
        plot->getPlotter()->setMaintainAspectRatio(checked);
        plot->getPlotter()->setMaintainAxisAspectRatio(checked);
    });
    // toggling the checkbox switches between linear and log x-axis
    layMain->addWidget(chk=new QCheckBox(QObject::tr("x: log-scale"), &widMain), 0, 1);
    chk->setChecked(false);
    QObject::connect(chk, &QCheckBox::toggled,  plot->getPlotter()->getXAxis(), &JKQTPHorizontalAxis::setLogAxis);
    // toggling the checkbox switches between linear and log y-axis
    layMain->addWidget(chk=new QCheckBox(QObject::tr("y: log-scale"), &widMain), 0, 2);
    chk->setChecked(false);
    QObject::connect(chk, &QCheckBox::toggled,  plot->getPlotter()->getYAxis(), &JKQTPVerticalAxis::setLogAxis);
    QComboBox* cmb;
    // a combobox to select the DrawMode of all graph elements
    layMain->addWidget(new QLabel(QObject::tr("DrawMode: "), &widMain), 0, 3);
    layMain->addWidget(cmb=new QComboBox(&widMain), 0, 4);
    cmb->addItem(QObject::tr("DrawAsGraphicElement"));
    cmb->addItem(QObject::tr("DrawAsMathematicalCurve"));
    cmb->setCurrentIndex(0);
    QObject::connect(cmb, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [plot](int index) {
        for (size_t i=0; i<plot->getPlotter()->getGraphCount(); i++) {
            JKQTPGeometricPlotElement* obj=dynamic_cast<JKQTPGeometricPlotElement*>(plot->getPlotter()->getGraph(i));
            if (obj) {
                obj->setDrawMode((index==0)?JKQTPGeometricPlotElement::DrawAsGraphicElement:JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
            }
        }
        plot->redrawPlot();
    });



    // 2. format graph:
    // 2.1 set the graph scales manually
    plot->setXY(0,3.05,0,3.05);
    // 2.2 set the asxpect ratio to 1
    plot->getPlotter()->setMaintainAspectRatio(true);
    plot->getPlotter()->setAspectRatio(1);
    plot->getPlotter()->setMaintainAxisAspectRatio(true);
    plot->getPlotter()->setAxisAspectRatio(1);
    // 2.3 set the asxpect ratio to 1
    plot->getXAxis()->setDrawGrid(false);
    plot->getYAxis()->setDrawGrid(false);
    // 2.4 show 9 minor ticks between two major ticks
    plot->getXAxis()->setMinorTicks(9);
    plot->getYAxis()->setMinorTicks(9);


    // 3. add some geometric plots
    // 3.1 some text elements, also with formatting by LaTeX expressions
    plot->addGraph(new JKQTPGeoText(plot, 0.1,0.95, "\\textbf{Text:}", 14, QColor("red")));
    plot->addGraph(new JKQTPGeoText(plot, 0.1,0.1, "A text object in 10pt", 10, QColor("red")));
    plot->addGraph(new JKQTPGeoSymbol(plot, 0.1,0.1, JKQTPCircle, 5, QColor("grey")));
    plot->addGraph(new JKQTPGeoText(plot, 0.1,0.3, "A text object in 12pt", 12, QColor("red")));
    plot->addGraph(new JKQTPGeoSymbol(plot, 0.1,0.3, JKQTPCircle, 5, QColor("grey")));
    plot->addGraph(new JKQTPGeoText(plot, 0.1,0.6, "$x_{1/2}=\\frac{\\sqrt{b^2-4ac}}{2a}$", 10, QColor("red")));
    plot->addGraph(new JKQTPGeoSymbol(plot, 0.1,0.6, JKQTPCircle, 5, QColor("grey")));


    // 3.2 some lines elements (single lines)
    plot->addGraph(new JKQTPGeoText(plot, 1.1,0.95, "\\textbf{Lines:}", 14, QColor("red")));
    JKQTPGeoLine* l;
    plot->addGraph(l=new JKQTPGeoLine(plot, 1,  0.05, 1.9, 0.9)); l->setStyle(QColor("red"), 2);
    plot->addGraph(l=new JKQTPGeoLine(plot, 1,  0.1, 1.9, 0.8)); l->setStyle(QColor("blue"), 1, Qt::PenStyle::DashLine);
    JKQTPGeoInfiniteLine* infLine=new JKQTPGeoInfiniteLine(plot, 1.7,  0.2, 0.2, 0.25);
    infLine->setStyle(QColor("green"), 1.5, Qt::PenStyle::DashLine);
    infLine->setTwoSided(false);
    infLine->setAlpha(0.5);
    plot->addGraph(infLine);
    infLine=new JKQTPGeoInfiniteLine(plot, 1.7,  0.2, 0.2, -0.15);
    infLine->setStyle(QColor("blue"), 1.5, Qt::PenStyle::SolidLine);
    infLine->setTwoSided(true);
    infLine->setAlpha(0.5);
    plot->addGraph(infLine);

    // 3.3 some arrows
    plot->addGraph(new JKQTPGeoText(plot, 0.1,2.95, "\\textbf{Arrows:}", 14, QColor("red")));
    JKQTPGeoArrow* a;
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.3,  2.1, 0.1, 2.9, JKQTPTriangleDecoratorAndBar, JKQTPDiamondDecoratorAndBar)); a->setStyle(QColor("green"), 1);
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.2,  2.1, 0.2, 2.9, JKQTPNoDecorator, JKQTPFilledTriangleDecoratorAndBar)); a->setStyle(QColor("blue"), 2);
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.1,  2.1, 0.3, 2.9, JKQTPDoubleArrowAndBar, JKQTPNoDecorator)); a->setStyle(QColor("orange"), 3);

    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.6,  2.1, 0.4, 2.9, JKQTPTriangleDecorator, JKQTPTriangleDecorator)); a->setStyle(QColor("green"), 0.5);
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.5,  2.1, 0.5, 2.9, JKQTPNoDecorator, JKQTPFilledArrow)); a->setStyle(QColor("blue"), 2);
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.4,  2.1, 0.6, 2.9, JKQTPFilledArrow, JKQTPNoDecorator)); a->setStyle(QColor("orange"), 1);

    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.7,  2.1, 0.7, 2.5, JKQTPArrowAndBar, JKQTPFilledArrow)); a->setStyle(QColor("green"), 0.5);
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.8,  2.1, 0.8, 2.5, JKQTPArrowAndBar, JKQTPFilledArrow)); a->setStyle(QColor("orange"), 1);
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.9,  2.1, 0.9, 2.5, JKQTPArrowAndBar, JKQTPFilledArrow)); a->setStyle(QColor("orange"), 2);

    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.7,  2.7, 0.7, 2.9, JKQTPTriangleDecoratorAndBar, JKQTPFilledCircleDecorator)); a->setStyle(QColor("green"), 0.5);
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.8,  2.7, 0.8, 2.9, JKQTPTriangleDecoratorAndBar, JKQTPFilledCircleDecorator)); a->setStyle(QColor("orange"), 1);
    plot->addGraph(a=new JKQTPGeoArrow(plot, 0.9,  2.7, 0.9, 2.9, JKQTPTriangleDecoratorAndBar, JKQTPFilledCircleDecorator)); a->setStyle(QColor("orange"), 2);



    // 3.4 some rectangles (you give the center and width/height of the rectangle in the contructor)
    plot->addGraph(new JKQTPGeoText(plot, 0.1,1.95, "\\textbf{Rectangles:}", 14, QColor("red")));
    JKQTPGeoBaseFilled* geo;
    JKQTPGeoRectangle* rec;
    plot->addGraph(geo=new JKQTPGeoRectangle(plot, 0.5,1.5,0.8,0.8));
    geo->setStyle(QColor("blue"), 1, Qt::SolidLine, QColor("lightblue"), Qt::SolidPattern);
    QColor rfill("lightblue"); rfill.setAlphaF(0.5);
    plot->addGraph(geo=new JKQTPGeoRectangle(plot, QPointF(0.4,1.3), QPointF(0.9,1.6)));
    geo->setStyle(QColor("blue"), 1, Qt::SolidLine, rfill, Qt::SolidPattern);
    plot->addGraph(rec=new JKQTPGeoRectangle(plot, 0.5,1.5,0.8,0.5));
    rec->setStyle(QColor("darkblue"), 2, Qt::DashLine);
    rec->setAngle(35);

    plot->addGraph(rec=new JKQTPGeoRectangle(plot, 0.5,1.5,0.6,0.6));
    rec->setStyle(QColor("orange"), 2, Qt::DotLine, QColor("salmon"), Qt::FDiagPattern);
    rec->setAngle(-7);
    plot->addGraph(new JKQTPGeoSymbol(plot, 0.5,1.5, JKQTPCircle, 5, QColor("grey")));
    plot->addGraph(new JKQTPGeoSymbol(plot, 0.4,1.3, JKQTPCircle, 5, QColor("grey")));
    plot->addGraph(new JKQTPGeoSymbol(plot, 0.9,1.6, JKQTPCircle, 5, QColor("grey")));


    // 3.5 some circles and elllipses
    plot->addGraph(new JKQTPGeoText(plot, 1.1,1.95, "\\textbf{Circles/Ellipses:}", 14, QColor("red")));
    QColor col=QColor("blue"); col.setAlphaF(0.2);
    plot->addGraph(geo=new JKQTPGeoRectangle(plot, 1.5,1.5,0.8,0.8));
    geo->setStyleTransparentFill(col, 0.5, Qt::SolidLine);
    col.setAlphaF(1);
    plot->addGraph(geo=new JKQTPGeoEllipse(plot, 1.5,1.5,0.8,0.8));
    geo->setStyle(col, 1, Qt::SolidLine, QColor("lightblue"), Qt::SolidPattern);
    col=QColor("blue"); col.setAlphaF(0.2);
    plot->addGraph(geo=new JKQTPGeoRectangle(plot, QPointF(1.4,1.3), QPointF(1.9,1.6)));
    geo->setStyleTransparentFill(col, 0.5, Qt::SolidLine);
    col.setAlphaF(1);
    plot->addGraph(geo=new JKQTPGeoEllipse(plot, QPointF(1.4,1.3), QPointF(1.9,1.6)));
    geo->setStyle(col, 1, Qt::SolidLine, rfill, Qt::SolidPattern);
    col=QColor("darkblue"); col.setAlphaF(0.2);
    plot->addGraph(rec=new JKQTPGeoRectangle(plot, 1.5,1.5,0.8,0.5));
    rec->setStyleTransparentFill(col, 0.5, Qt::DashLine);
    rec->setAngle(35);
    col.setAlphaF(1);
    plot->addGraph(rec=new JKQTPGeoEllipse(plot, 1.5,1.5,0.8,0.5));
    rec->setStyleTransparentFill(col, 2, Qt::DashLine);
    rec->setAngle(35);
    col=QColor("orange"); col.setAlphaF(0.2);
    plot->addGraph(rec=new JKQTPGeoRectangle(plot, 1.5,1.5,0.6,0.6));
    rec->setStyleTransparentFill(col, 0.5, Qt::DotLine);
    rec->setAngle(-7);
    col.setAlphaF(1);
    plot->addGraph(rec=new JKQTPGeoEllipse(plot, 1.5,1.5,0.6,0.6));
    rec->setStyle(col, 2, Qt::DotLine, QColor("salmon"), Qt::FDiagPattern);
    rec->setAngle(-7);
    plot->addGraph(new JKQTPGeoSymbol(plot, 1.5,1.5, JKQTPCircle, 5, QColor("grey")));
    plot->addGraph(new JKQTPGeoSymbol(plot, 1.4,1.3, JKQTPCircle, 5, QColor("grey")));
    plot->addGraph(new JKQTPGeoSymbol(plot, 1.9,1.6, JKQTPCircle, 5, QColor("grey")));


    // 3.6 some polygon elements
    plot->addGraph(new JKQTPGeoText(plot, 2.1,0.95, "\\textbf{PolyLines/Polygons:}", 14, QColor("red")));
    QVector<QPointF> polygon;
    polygon<<QPointF(2, 0.1)<<QPointF(2.3, 0.9)<<QPointF(2.6, 0.3)<<QPointF(2, 0.8);
    JKQTPGeoPolygon* polygraph=new JKQTPGeoPolygon(plot, polygon);
    polygraph->setStyle(QColor("blue"), 2, Qt::PenStyle::DashLine, QColor("lightblue"), Qt::SolidPattern);
    polygraph->setAlpha(0.75);
    plot->addGraph(polygraph);
    polygraph=new JKQTPGeoPolygon(plot);
    polygraph->setStyle(QColor("red"), 2, Qt::PenStyle::DashLine, QColor("salmon"), Qt::SolidPattern);
    polygraph->appendPoint(2.1, 0.5);
    polygraph->appendPoint(2.9, 0.9);
    polygraph->appendPoint(2.2, 0.8);
    polygraph->appendPoint(2.8, 0.25);
    polygraph->appendPoint(2.6, 0.6);
    polygraph->setAlpha(0.75);
    plot->addGraph(polygraph);


    // 3.7 a poly-line element
    QVector<QPointF> p;
    p<<QPointF(2.1, 0.0)<<QPointF(2.2, 0.92)<<QPointF(2.6, 0.3)<<QPointF(2.9, 0.9);
    JKQTPGeoPolyLines* poly;
    plot->addGraph(poly=new JKQTPGeoPolyLines(plot, p));
    poly->setStyle(QColor("darkgreen"), 4, Qt::PenStyle::DashDotLine);


    // 3.8 some arcs
    plot->addGraph(new JKQTPGeoText(plot, 2.1,1.95, "\\textbf{Arcs:}", 14, QColor("red")));
    plot->addGraph(rec=new JKQTPGeoRectangle(plot, 2.5,1.5,0.9,0.9));
    rec->setStyleTransparentFill(QColor("lightgrey"), 0.5, Qt::SolidLine);
    JKQTPGeoArc* arc;
    plot->addGraph(arc=new JKQTPGeoArc(plot,2.5,1.5,0.9,0.9, 0, 90)); arc->setStyle(QColor("blue"), 4, Qt::PenStyle::SolidLine);
    plot->addGraph(arc=new JKQTPGeoArc(plot,2.5,1.5,0.8,0.8, 90,180)); arc->setStyle(QColor("red"), 4, Qt::PenStyle::SolidLine);
    plot->addGraph(arc=new JKQTPGeoArc(plot,2.5,1.5,0.7,0.7, 180,270)); arc->setStyle( QColor("darkblue"), 4, Qt::PenStyle::SolidLine);
    plot->addGraph(arc=new JKQTPGeoArc(plot,2.5,1.5,0.6,0.6, 270,360)); arc->setStyle(QColor("maroon"), 4, Qt::PenStyle::DashDotLine);
    plot->addGraph(arc=new JKQTPGeoArc(plot,2.5,1.5,0.5,0.5, -10, 117)); arc->setStyle(QColor("orange"), 4, Qt::PenStyle::DashLine);
    plot->addGraph(arc=new JKQTPGeoArc(plot,2.5,1.5,0.4,0.4, 85, 347)); arc->setStyle(QColor("darkgreen"), 4, Qt::PenStyle::SolidLine);
    plot->addGraph(new JKQTPGeoSymbol(plot, 2.5,1.5, JKQTPCircle, 5, QColor("grey")));


    // 3.9 some pies
    plot->addGraph(new JKQTPGeoText(plot, 2.1,2.95, "\\textbf{Pies:}", 14, QColor("red")));
    plot->addGraph(rec=new JKQTPGeoRectangle(plot, 2.5,2.5,0.9,0.9));
    rec->setStyleTransparentFill(QColor("lightgrey"), 0.5, Qt::SolidLine);
    JKQTPGeoPie* pie;
    plot->addGraph(pie=new JKQTPGeoPie(plot,2.5,2.5,0.9,0.9, 0, 90));
    pie->setStyle(QColor("blue"), 4, Qt::PenStyle::SolidLine,QColor("lightblue"), Qt::SolidPattern);
    plot->addGraph(pie=new JKQTPGeoPie(plot,2.5,2.5,0.7,0.7, 180,270 ));
    pie->setStyle(QColor("darkblue"), 4, Qt::PenStyle::SolidLine,QColor("blue"), Qt::SolidPattern);
    pie->setAlpha(0.5);
    plot->addGraph(pie=new JKQTPGeoPie(plot,2.5,2.5,0.5,0.3, -25, 117));
    pie->setStyle(QColor("orange"), 4, Qt::PenStyle::DashLine,QColor("red"), Qt::SolidPattern);
    pie->setAlpha(0.5);
    plot->addGraph(pie=new JKQTPGeoPie(plot,2.5,2.5,0.25,0.8, 85, 347));
    pie->setStyle(QColor("darkgreen"), 4, Qt::PenStyle::SolidLine,QColor("green"), Qt::SolidPattern);
    pie->setAlpha(0.5);
    plot->addGraph(new JKQTPGeoSymbol(plot, 2.5,2.5, JKQTPCircle, 5, QColor("grey")));


    // 3.10 some chords
    plot->addGraph(new JKQTPGeoText(plot, 1.1,2.95, "\\textbf{Chords:}", 14, QColor("red")));
    plot->addGraph(rec=new JKQTPGeoRectangle(plot, 1.5,2.5,0.9,0.9));
    rec->setStyleTransparentFill(QColor("lightgrey"), 0.5, Qt::SolidLine);
    JKQTPGeoChord* chord;
    plot->addGraph(chord=new JKQTPGeoChord(plot,1.5,2.5,0.9,0.9, 0, 90));
    chord->setStyle(QColor("blue"), 4, Qt::PenStyle::SolidLine,QColor("lightblue"), Qt::SolidPattern);
    plot->addGraph(chord=new JKQTPGeoChord(plot,1.5,2.5,0.7,0.7, 180,270));
    chord->setStyle(QColor("darkblue"), 4, Qt::PenStyle::SolidLine,QColor("blue"), Qt::SolidPattern);
    chord->setAlpha(0.5);
    plot->addGraph(chord=new JKQTPGeoChord(plot,1.5,2.5,0.5,0.3, -25, 117));
    chord->setStyle(QColor("orange"), 4, Qt::PenStyle::DashLine,QColor("red"), Qt::SolidPattern);
    chord->setAlpha(0.5);
    plot->addGraph(chord=new JKQTPGeoChord(plot,1.5,2.5,0.25,0.8, 85, 347 ));
    chord->setStyle(QColor("darkgreen"), 4, Qt::PenStyle::SolidLine,QColor("green"), Qt::SolidPattern);
    chord->setAlpha(0.5);
    plot->addGraph(new JKQTPGeoSymbol(plot, 1.5,2.5, JKQTPCircle, 5, QColor("grey")));


    // 4. show plotter and make it a decent size
    widMain.show();
    widMain.resize(1100,800);

    return app.exec();
}
