#include "TestWidgetEmptyPlot.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>




TestWidgetEmptyPlot::TestWidgetEmptyPlot(QWidget *parent) :
    QWidget(parent)
{


    #define NEMPTY 500

    JKQTPLotter* plotEmpty=new JKQTPLotter(true, this);
    JKQTPXYLineGraph* efunc=new JKQTPXYLineGraph(plotEmpty->get_plotter());
    double xef[NEMPTY], efy[NEMPTY];
    for (int i=0; i<NEMPTY; i++) {
        xef[i]=i;
        efy[i]=double(i%5)*1e-308;
    }
    efunc->set_xColumn(plotEmpty->getDatastore()->addCopiedColumn(xef, NEMPTY, "x"));
    efunc->set_yColumn(plotEmpty->getDatastore()->addCopiedColumn(efy, NEMPTY, "y"));
    plotEmpty->addGraph(efunc);
    plotEmpty->get_yAxis()->set_logAxis(true);
    plotEmpty->zoomToFit();
    plotEmpty->setY(0,0);



}
