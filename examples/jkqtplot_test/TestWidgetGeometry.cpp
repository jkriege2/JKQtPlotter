#include "TestWidgetGeometry.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>





TestWidgetGeometry::TestWidgetGeometry(QWidget *parent) :
    QWidget(parent)
{
    // create third plotter with output of a geometric drawing
    QVBoxLayout* layout1=new QVBoxLayout(this);
    setLayout(layout1);
    resize(1000, 800);
    JKQTPlotter* plotGeo=new JKQTPlotter(true, this);
    plotGeo->set_doDrawing(false);
    plotGeo->setObjectName("plotGeo");
    layout1->addWidget(plotGeo);
    plotGeo->setXY(0,1,0,1);
    plotGeo->get_plotter()->get_xAxis()->set_axisLabel("$x$ [mm]");
    plotGeo->get_plotter()->get_yAxis()->set_axisLabel("$y$ [mm]");
    /*plotGeo->get_plotter()->get_xAxis()->set_linkedAxis(plotGeo->get_plotter()->get_yAxis());
    plotGeo->get_plotter()->get_xAxis()->set_changeWidthToAspectRatio(true);*/
    plotGeo->get_plotter()->set_maintainAspectRatio(true);
    plotGeo->get_plotter()->set_aspectRatio(1);
    plotGeo->get_plotter()->set_maintainAxisAspectRatio(true);
    plotGeo->get_plotter()->set_axisAspectRatio(1);


//    JKQTPGeoLine* line1=new JKQTPGeoLine(plotGeo->get_plotter(), -1, -2, 2, 1.5);
    //plotGeo->get_plotter()->addGraph(line1);
//    JKQTPGeoLine* line2=new JKQTPGeoLine(plotGeo->get_plotter(), 1, -5, 4, 3, QColor("blue"), 4);
    //plotGeo->get_plotter()->addGraph(line2);

    JKQTPGeoRectangle* rect1=new JKQTPGeoRectangle(plotGeo->get_plotter(), 1,1,2,2, QColor("blue"),2, Qt::SolidLine, QColor("lightblue"));
    rect1->set_angle(30);
    //plotGeo->get_plotter()->addGraph(rect1);
    JKQTPGeoRectangle* rect2=new JKQTPGeoRectangle(plotGeo->get_plotter(), 1,1,2,2, QColor("red"),2);
    rect2->set_angle(45);
    //plotGeo->get_plotter()->addGraph(rect2);
    JKQTPGeoRectangle* rect3=new JKQTPGeoRectangle(plotGeo->get_plotter(), 1,1,2,4, QColor("green"),2);
    rect3->set_angle(-30);
    //plotGeo->get_plotter()->addGraph(rect3);
    JKQTPGeoEllipse* ell3=new JKQTPGeoEllipse(plotGeo->get_plotter(), 1,1,2,4, QColor("green"),2, Qt::SolidLine, QColor("lightgreen"), Qt::CrossPattern);
    ell3->set_angle(-30);
    //plotGeo->get_plotter()->addGraph(ell3);

    QVector<QPointF> p;
    p.append(QPointF(4,1));
    p.append(QPointF(-11,5));
    p.append(QPointF(1,11));
    p.append(QPointF(0,-2));
    p.append(QPointF(M_PI,5.5));

//    JKQTPGeoPolygon* poly=new JKQTPGeoPolygon(plotGeo->get_plotter(), p, QColor("black"), 1.5, Qt::SolidLine, QColor(128,128,0,128));
    //plotGeo->get_plotter()->addGraph(poly);
//    JKQTPGeoPolyLines* lines=new JKQTPGeoPolyLines(plotGeo->get_plotter(), p, QColor(255,255,0), 3);
    //plotGeo->get_plotter()->addGraph(lines);

    JKQTPGeoEllipse* ell1=new JKQTPGeoEllipse(plotGeo->get_plotter(), 0,0,2,4, QColor("black"),1.5, Qt::DotLine);
    plotGeo->get_plotter()->addGraph(ell1);
//    JKQTPGeoArc* arc=new JKQTPGeoArc(plotGeo->get_plotter(), 0,0,2,4,0, 135, QColor("blue"),3, Qt::SolidLine);
    //plotGeo->get_plotter()->addGraph(arc);
//    JKQTPGeoPie* pie=new JKQTPGeoPie(plotGeo->get_plotter(), 0,0,2,4,35, 98, QColor("blue"),3, Qt::SolidLine, QColor(0,0,255,127));
    //plotGeo->get_plotter()->addGraph(pie);
//    JKQTPGeoChord* chord=new JKQTPGeoChord(plotGeo->get_plotter(), 0,0,2,4,35, 98, QColor("blue"),3, Qt::SolidLine, QColor(0,0,255,127));
    //plotGeo->get_plotter()->addGraph(chord);

    JKQTPGeoInfiniteLine* iline=new JKQTPGeoInfiniteLine(plotGeo->get_plotter(), 0.5,1,1,1, QColor("blue"), 3);
    plotGeo->get_plotter()->addGraph(iline);
    JKQTPGeoInfiniteLine* iline1=new JKQTPGeoInfiniteLine(plotGeo->get_plotter(), 0.5,1,-1,0, QColor("red"), 3);
    plotGeo->get_plotter()->addGraph(iline1);
    JKQTPGeoInfiniteLine* iline2=new JKQTPGeoInfiniteLine(plotGeo->get_plotter(), 0.5,1,0,1, QColor("red"), 3);
    plotGeo->get_plotter()->addGraph(iline2);
    JKQTPGeoInfiniteLine* iline3=new JKQTPGeoInfiniteLine(plotGeo->get_plotter(), 0.5,1,-1,0.5, QColor("green"), 3);
    iline3->set_two_sided(true);
    plotGeo->get_plotter()->addGraph(iline3);

    JKQTPGeoText* text=new JKQTPGeoText(plotGeo->get_plotter(), -1, -1, "Test $f(x)=\\sqrt{\\sin(x^2)}$ \\textbf{bold}", 12, QColor("red"));
    plotGeo->get_plotter()->addGraph(text);

    plotGeo->set_doDrawing(true);
    plotGeo->get_plotter()->zoomToFit();
    plotGeo->get_plotter()->set_keyAutosize(true);


    QCheckBox* chkAspect=new QCheckBox("keep aspect ratio", this);
    chkAspect->setChecked(plotGeo->get_plotter()->get_maintainAxisAspectRatio());
    connect(chkAspect, SIGNAL(toggled(bool)), plotGeo->get_plotter(), SLOT(set_maintainAspectRatio(bool)));
    connect(chkAspect, SIGNAL(toggled(bool)), plotGeo->get_plotter(), SLOT(set_maintainAxisAspectRatio(bool)));
    layout1->addWidget(chkAspect);

}
