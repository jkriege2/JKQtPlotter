#include "TestWidgetGeometry.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>





TestWidgetGeometry::TestWidgetGeometry(QWidget *parent) :
    QWidget(parent)
{
    // create third plotter with output of a geometric drawing
    QVBoxLayout* layout1=new QVBoxLayout();
    setLayout(layout1);
    resize(1000, 800);
    JKQTPlotter* plotGeo=new JKQTPlotter(true, this);
    plotGeo->setPlotUpdateEnabled(false);
    plotGeo->setObjectName("plotGeo");
    layout1->addWidget(plotGeo);
    plotGeo->setXY(0,1,0,1);
    plotGeo->getPlotter()->getXAxis()->setAxisLabel("$x$ [mm]");
    plotGeo->getPlotter()->getYAxis()->setAxisLabel("$y$ [mm]");
    /*plotGeo->getPlotter()->getXAxis()->setLinkedAxis(plotGeo->getPlotter()->getYAxis());
    plotGeo->getPlotter()->getXAxis()->setChangeWidthToAspectRatio(true);*/
    plotGeo->getPlotter()->setMaintainAspectRatio(true);
    plotGeo->getPlotter()->setAspectRatio(1);
    plotGeo->getPlotter()->setMaintainAxisAspectRatio(true);
    plotGeo->getPlotter()->setAxisAspectRatio(1);


//    JKQTPGeoLine* line1=new JKQTPGeoLine(plotGeo->getPlotter(), -1, -2, 2, 1.5);
    //plotGeo->getPlotter()->addGraph(line1);
//    JKQTPGeoLine* line2=new JKQTPGeoLine(plotGeo->getPlotter(), 1, -5, 4, 3, QColor("blue"), 4);
    //plotGeo->getPlotter()->addGraph(line2);

    JKQTPGeoRectangle* rect1=new JKQTPGeoRectangle(plotGeo->getPlotter(), 1,1,2,2);
    rect1->setStyle(QColor("blue"),2, Qt::SolidLine, QColor("lightblue"), Qt::SolidPattern);
    rect1->setAngle(30);
    //plotGeo->getPlotter()->addGraph(rect1);
    JKQTPGeoRectangle* rect2=new JKQTPGeoRectangle(plotGeo->getPlotter(), 1,1,2,2);
    rect2->setStyleTransparentFill(QColor("red"),2);
    rect2->setAngle(45);
    //plotGeo->getPlotter()->addGraph(rect2);
    JKQTPGeoRectangle* rect3=new JKQTPGeoRectangle(plotGeo->getPlotter(), 1,1,2,4);
    rect3->setStyleTransparentFill(QColor("green"),2);
    rect3->setAngle(-30);
    //plotGeo->getPlotter()->addGraph(rect3);
    JKQTPGeoEllipse* ell3=new JKQTPGeoEllipse(plotGeo->getPlotter(), 1,1,2,4);
    ell3->setStyle(QColor("green"),2, Qt::SolidLine, QColor("lightgreen"), Qt::CrossPattern);
    ell3->setAngle(-30);
    //plotGeo->getPlotter()->addGraph(ell3);

    QVector<QPointF> p;
    p.append(QPointF(4,1));
    p.append(QPointF(-11,5));
    p.append(QPointF(1,11));
    p.append(QPointF(0,-2));
    p.append(QPointF(JKQTPSTATISTICS_PI,5.5));

    JKQTPGeoEllipse* ell1=new JKQTPGeoEllipse(plotGeo->getPlotter(), 0,0,2,4);
    ell1->setStyleTransparentFill(QColor("black"),1.5, Qt::DotLine);
    plotGeo->getPlotter()->addGraph(ell1);

    JKQTPGeoInfiniteLine* iline=new JKQTPGeoInfiniteLine(plotGeo->getPlotter(), 0.5,1,1,1);
    iline->setStyle(QColor("blue"), 3);
    plotGeo->getPlotter()->addGraph(iline);
    JKQTPGeoInfiniteLine* iline1=new JKQTPGeoInfiniteLine(plotGeo->getPlotter(), 0.5,1,-1,0);
    iline->setStyle(QColor("red"), 3);
    plotGeo->getPlotter()->addGraph(iline1);
    JKQTPGeoInfiniteLine* iline2=new JKQTPGeoInfiniteLine(plotGeo->getPlotter(), 0.5,1,0,1);
    iline->setStyle(QColor("red"), 3);
    plotGeo->getPlotter()->addGraph(iline2);
    JKQTPGeoInfiniteLine* iline3=new JKQTPGeoInfiniteLine(plotGeo->getPlotter(), 0.5,1,-1,0.5);
    iline->setStyle(QColor("green"), 3);
    iline3->setTwoSided(true);
    plotGeo->getPlotter()->addGraph(iline3);

    JKQTPGeoText* text=new JKQTPGeoText(plotGeo->getPlotter(), -1, -1, "Test $f(x)=\\sqrt{\\sin(x^2)}$ \\textbf{bold}", 12, QColor("red"));
    plotGeo->getPlotter()->addGraph(text);

    plotGeo->setPlotUpdateEnabled(true);
    plotGeo->getPlotter()->zoomToFit();


    QCheckBox* chkAspect=new QCheckBox("keep aspect ratio", this);
    chkAspect->setChecked(plotGeo->getPlotter()->doesMaintainAxisAspectRatio());
    connect(chkAspect, SIGNAL(toggled(bool)), plotGeo->getPlotter(), SLOT(setMaintainAspectRatio(bool)));
    connect(chkAspect, SIGNAL(toggled(bool)), plotGeo->getPlotter(), SLOT(setMaintainAxisAspectRatio(bool)));
    layout1->addWidget(chkAspect);

}
