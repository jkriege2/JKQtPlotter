/** \example geo_simple.cpp
 * This project contains code that is used to generate the example graphs for geometric objects from teh documentation.
 *
 * \ref JKQTPlotterGeometricGraphsSimple
 */
#include <QCheckBox>
#include <QComboBox>
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"


template <class TELLIPSEPART>
void drawExample(JKQTPlotter* plot, double x0, double y0, double wid, double heig, double angleStart, double angleStop) {
    JKQTPGeoRectangle* rect;
    plot->addGraph(rect=new JKQTPGeoRectangle(plot,x0, y0, wid, heig));
    rect->setLineColor(QColor("orange"));
    rect->setLineStyle(Qt::PenStyle::DashLine);
    rect->setFillColor(Qt::transparent);
    rect->setLineWidth(1);
    JKQTPGeoEllipse* ell;
    plot->addGraph(ell=new JKQTPGeoEllipse(plot,x0, y0, wid, heig));
    ell->setLineColor(QColor("orange"));
    ell->setLineStyle(Qt::PenStyle::DashLine);
    ell->setFillColor(Qt::transparent);
    ell->setLineWidth(1);
    TELLIPSEPART* obj;
    plot->addGraph(obj=new TELLIPSEPART(plot,x0, y0, wid, heig, 10, angleStop));
    obj->setLineColor(QColor("blue"));
    obj->setLineStyle(Qt::PenStyle::SolidLine);
    obj->setLineWidth(4);
    JKQTPGeoBaseFilled* fillobj=dynamic_cast<JKQTPGeoBaseFilled*>(obj);
    if (fillobj) {
        fillobj->setFillColor(QColor("lightblue"));
        fillobj->setAlpha(0.8);
    }
    JKQTPGeoLine* l;
    plot->addGraph(new JKQTPGeoSymbol(plot, x0, y0,  JKQTPCircle, 5, QColor("red")));
    plot->addGraph(l=new JKQTPGeoLine(plot, x0, y0, x0+wid/2.0*cos(angleStart/180.0*JKQTPSTATISTICS_PI),y0+heig/2.0*sin(angleStart/180.0*JKQTPSTATISTICS_PI))); l->setColor(QColor("orange"));
    plot->addGraph(new JKQTPGeoText(plot, x0+wid*1.1/2.0*cos(angleStart/180.0*JKQTPSTATISTICS_PI),y0+heig*1.1/2.0*sin(angleStart/180.0*JKQTPSTATISTICS_PI), "\\alpha_\\text{start}="+QString::number(angleStart, 'f', 0)+"\\degree", 10, QColor("orange")));
    plot->addGraph(new JKQTPGeoLine(plot, x0, y0, x0+wid/2.0*cos(angleStop/180.0*JKQTPSTATISTICS_PI),y0+heig/2.0*sin(angleStop/180.0*JKQTPSTATISTICS_PI))); l->setColor(QColor("red"));
    plot->addGraph(new JKQTPGeoText(plot, x0+wid*1.1/2.0*cos(angleStop/180.0*JKQTPSTATISTICS_PI),y0+heig*1.1/2.0*sin(angleStop/180.0*JKQTPSTATISTICS_PI), "\\alpha_\\text{stop}="+QString::number(angleStop, 'f', 0)+"\\degree", 10, QColor("red")));
    plot->addGraph(new JKQTPGeoLine(plot, x0, y0, x0+wid/2.0*cos(0/180.0*JKQTPSTATISTICS_PI),y0+heig/2.0*sin(0/180.0*JKQTPSTATISTICS_PI)));l->setColor(QColor("orange"));
    plot->addGraph(new JKQTPGeoText(plot, x0+wid*1.1/2.0*cos(0/180.0*JKQTPSTATISTICS_PI),y0+heig*1.1/2.0*sin(0/180.0*JKQTPSTATISTICS_PI), "0\\degree", 10, QColor("orange")));
    plot->addGraph(new JKQTPGeoText(plot, x0,y0-0.05*heig, "(x,y)", 10, QColor("red")));
    plot->addGraph(new JKQTPGeoText(plot, x0,y0-heig*1.1/2.0, "width="+QString::number(wid, 'f', 1), 10, QColor("red")));
    plot->addGraph(new JKQTPGeoText(plot, x0-wid*1.05/2.0,y0, "height="+QString::number(heig, 'f', 1), 10, QColor("red")));

}



void drawEllExample(JKQTPlotter* plot, double x0, double y0, double wid, double heig, double angle=0) {
    JKQTPGeoRectangle* rec;
    plot->addGraph(rec=new JKQTPGeoRectangle(plot,x0, y0, wid, heig));
    rec->setAngle(angle);
    rec->setLineColor(QColor("orange"));
    rec->setLineStyle(Qt::PenStyle::DashLine);
    rec->setFillColor(Qt::transparent);
    rec->setLineWidth(1);
    JKQTPGeoEllipse* fillobj;
    plot->addGraph(fillobj=new JKQTPGeoEllipse(plot,x0, y0, wid, heig));
    fillobj->setLineColor(QColor("blue"));
    fillobj->setLineStyle(Qt::PenStyle::DashLine);
    fillobj->setFillColor(Qt::transparent);
    fillobj->setLineWidth(1);
    fillobj->setFillColor(QColor("lightblue"));
    fillobj->setAlpha(0.8);
    fillobj->setAngle(angle);
    plot->addGraph(new JKQTPGeoSymbol(plot, x0, y0,  JKQTPCircle, 5, QColor("red")));
    plot->addGraph(new JKQTPGeoText(plot, x0,y0-0.05*heig, "(x,y)", 10, QColor("red")));
    if (fabs(angle)<0.01) {
        plot->addGraph(new JKQTPGeoText(plot, x0,y0-heig*1.1/2.0, "width="+QString::number(wid, 'f', 1), 10, QColor("red")));
        plot->addGraph(new JKQTPGeoText(plot, x0-wid*1.05/2.0,y0, "height="+QString::number(heig, 'f', 1), 10, QColor("red")));
    } else {
        JKQTPGeoLine* l;
        plot->addGraph(l=new JKQTPGeoLine(plot, x0, y0, x0+wid/2.0*cos(angle/180.0*JKQTPSTATISTICS_PI),y0+heig/2.0*sin(angle/180.0*JKQTPSTATISTICS_PI))); l->setColor( QColor("red"));
        plot->addGraph(new JKQTPGeoText(plot, x0+wid*1.1/2.0*cos(angle/180.0*JKQTPSTATISTICS_PI),y0+heig*1.1/2.0*sin(angle/180.0*JKQTPSTATISTICS_PI), "\\alpha="+QString::number(angle, 'f', 0)+"\\degree", 10, QColor("red")));
        plot->addGraph(l=new JKQTPGeoLine(plot, x0, y0, x0+wid/2.0*cos(0/180.0*JKQTPSTATISTICS_PI),y0+heig/2.0*sin(0/180.0*JKQTPSTATISTICS_PI))); l->setColor( QColor("orange"));
        plot->addGraph(new JKQTPGeoText(plot, x0+wid*1.1/2.0*cos(0/180.0*JKQTPSTATISTICS_PI),y0+heig*1.1/2.0*sin(0/180.0*JKQTPSTATISTICS_PI), "0\\degree", 10, QColor("orange")));
    }

}

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    // 1. create a Widget with a plotter and some control-elements
    QWidget widMain;
    QGridLayout* layMain=new QGridLayout;
    widMain.setLayout(layMain);
    JKQTPlotter* plot=new JKQTPlotter(&widMain);
    JKQTPlotter* plotlog=new JKQTPlotter(&widMain);
    layMain->addWidget(plot, 1,0,1,3);
    layMain->addWidget(plotlog, 1,3,1,3);
    layMain->setRowStretch(1,1);
    QCheckBox* chk;
    QComboBox* cmb;
    // combobox to select what to draw
    layMain->addWidget(new QLabel(QObject::tr("Object: "), &widMain), 0, 1);
    layMain->addWidget(cmb=new QComboBox(&widMain), 0, 2);
    cmb->addItem(QObject::tr("JKQTPGeoArc"));
    cmb->addItem(QObject::tr("JKQTPGeoPie"));
    cmb->addItem(QObject::tr("JKQTPGeoChord"));
    cmb->addItem(QObject::tr("JKQTPGeoEllipse"));
    cmb->addItem(QObject::tr("JKQTPGeoEllipse, angle=-35deg"));
    cmb->setCurrentIndex(0);
    auto drawFunctor=[&](int index) {
        plot->clearGraphs();
        plotlog->clearGraphs();
        if (index==0) {
            drawExample<JKQTPGeoArc>(plot, 1,1,1,1.5,10,110);
            drawExample<JKQTPGeoArc>(plotlog, 1,1,1,1.5,10,110);
        } else if (index==1) {
            drawExample<JKQTPGeoPie>(plot, 1,1,1,1.5,10,110);
            drawExample<JKQTPGeoPie>(plotlog, 1,1,1,1.5,10,110);
        } else if (index==2) {
            drawExample<JKQTPGeoChord>(plot, 1,1,1,1.5,10,110);
            drawExample<JKQTPGeoChord>(plotlog, 1,1,1,1.5,10,110);
        } else if (index==3) {
            drawEllExample(plot, 1,1,1,1.5);
            drawEllExample(plotlog, 1,1,1,1.5);
        } else if (index==4) {
            drawEllExample(plot, 1,1,1,1.5,-35);
            drawEllExample(plotlog, 1,1,1,1.5,-35);
        }
        plot->zoomToFit();
        plotlog->zoomToFit();
    };
    QObject::connect(cmb, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), drawFunctor);

    // when checkbox is toggled: aspect-ration is maintained, or not
    layMain->addWidget(chk=new QCheckBox(QObject::tr("maintain aspect ratio"), &widMain), 0, 3);
    chk->setChecked(true);
    QObject::connect(chk, &QCheckBox::toggled, [&](bool checked) {
        plot->getPlotter()->setMaintainAspectRatio(checked);
        plot->getPlotter()->setMaintainAxisAspectRatio(checked);
        plotlog->getPlotter()->setMaintainAspectRatio(checked);
        plotlog->getPlotter()->setMaintainAxisAspectRatio(checked);
    });
    // when checkbox is toggled: aspect-ration is maintained, or not
    layMain->addWidget(new QLabel(QObject::tr("DrawMode: "), &widMain), 0, 4);
    layMain->addWidget(cmb=new QComboBox(&widMain), 0, 5);
    cmb->addItem(QObject::tr("DrawAsGraphicElement"));
    cmb->addItem(QObject::tr("DrawAsMathematicalCurve"));
    cmb->setCurrentIndex(0);
    QObject::connect(cmb, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int index) {
        for (size_t i=0; i<plot->getPlotter()->getGraphCount(); i++) {
            JKQTPGeometricPlotElement* obj=dynamic_cast<JKQTPGeometricPlotElement*>(plot->getPlotter()->getGraph(i));
            if (obj) {
                obj->setDrawMode((index==0)?JKQTPGeometricPlotElement::DrawAsGraphicElement:JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
            }
        }
        plot->redrawPlot();
        for (size_t i=0; i<plotlog->getPlotter()->getGraphCount(); i++) {
            JKQTPGeometricPlotElement* obj=dynamic_cast<JKQTPGeometricPlotElement*>(plotlog->getPlotter()->getGraph(i));
            if (obj) {
                obj->setDrawMode((index==0)?JKQTPGeometricPlotElement::DrawAsGraphicElement:JKQTPGeometricPlotElement::DrawAsMathematicalCurve);
            }
        }
        plotlog->redrawPlot();
    });



    // 2. format graph:
    // 2.1 set the asxpect ratio to 1
    plot->getPlotter()->setMaintainAspectRatio(true);
    plot->getPlotter()->setAspectRatio(1);
    plot->getPlotter()->setMaintainAxisAspectRatio(true);
    plot->getPlotter()->setAxisAspectRatio(1);
    // 2.2 set the asxpect ratio to 1
    plot->getXAxis()->setDrawGrid(true);
    plot->getYAxis()->setDrawGrid(true);
    plot->getXAxis()->setDrawMinorGrid(true);
    plot->getYAxis()->setDrawMinorGrid(true);
    // 2.3 show 9 minor ticks between two major ticks
    plot->getXAxis()->setMinorTicks(9);
    plot->getYAxis()->setMinorTicks(9);
    // 2.4 set log-axes
    plotlog->getXAxis()->setLogAxis(true);
    plotlog->getYAxis()->setLogAxis(true);
    // 2.5 set the asxpect ratio to 1
    plotlog->getPlotter()->setMaintainAspectRatio(true);
    plotlog->getPlotter()->setAspectRatio(1);
    plotlog->getPlotter()->setMaintainAxisAspectRatio(true);
    plotlog->getPlotter()->setAxisAspectRatio(1);
    // 2.6 set the asxpect ratio to 1
    plotlog->getXAxis()->setDrawGrid(true);
    plotlog->getYAxis()->setDrawGrid(true);
    plotlog->getXAxis()->setDrawMinorGrid(true);
    plotlog->getYAxis()->setDrawMinorGrid(true);
    // 2.7 show 9 minor ticks between two major ticks
    plotlog->getXAxis()->setMinorTicks(9);
    plotlog->getYAxis()->setMinorTicks(9);


    cmb->setCurrentIndex(0);
    drawFunctor(0);



    // 4. show plotter and make it a decent size
    widMain.show();
    widMain.resize(800,500);

    return app.exec();
}
