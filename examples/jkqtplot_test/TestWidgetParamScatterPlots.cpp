#include "TestWidgetParamScatterPlots.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>






TestWidgetParamScatterPlots::TestWidgetParamScatterPlots(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout* layoutPSP=new QVBoxLayout();

    setLayout(layoutPSP);
    resize(1000, 800);
    plotPSP=new JKQTPlotter(true, this);
    plotPSP->setPlotUpdateEnabled(false);
    plotPSP->getPlotter()->setPlotLabel(tr("\\textbf{parametrized scatter plots}"));
    plotPSP->setObjectName("plotPSP");
    plotPSP->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");

    layoutPSP->addWidget(plotPSP);

    QVector<double> dataX, dataY, dataY1, dataY2, dataY3, dataC, dataS, dataI, dataSy ;
    for (int i=0; i<100; i++) {
        dataI<<i;
        double x=double(i);
        dataX<<x;
        dataY<<sin(x*M_PI/20.0);
        dataY1<<3.0+sin(x*M_PI/20.0);
        dataY2<<dataY1.last()/5.0;
        dataY3<<dataY1.last()+4.0;
        dataC<<x;
        dataS<<(sin(x*M_PI/30.0)+1.0)*50.0;
        dataSy<<(2+i%15);
    }



    int pspx=plotPSP->getDatastore()->addCopiedColumn(dataX, "x");
    int pspy=plotPSP->getDatastore()->addCopiedColumn(dataY, "y");
    int pspy1=plotPSP->getDatastore()->addCopiedColumn(dataY1, "y1");
    int pspy2=plotPSP->getDatastore()->addCopiedColumn(dataY2, "y2");
    int pspy3=plotPSP->getDatastore()->addCopiedColumn(dataY2, "y3");
    int pspc=plotPSP->getDatastore()->addCopiedColumn(dataC, "color");
    int psps=plotPSP->getDatastore()->addCopiedColumn(dataS, "size");
    int pspsy=plotPSP->getDatastore()->addCopiedColumn(dataSy, "symbol");

    pltePSPCol=new JKQTPXYParametrizedErrorScatterGraph(plotPSP->getPlotter());
    pltePSPCol->setTitle(tr("changing color"));
    pltePSPCol->setXColumn(pspx);
    pltePSPCol->setYColumn(pspy);
    pltePSPCol->setColorColumn(pspc);
    pltePSPCol->setYErrorColumn(pspy1);
    pltePSPCol->setYErrorColumnLower(pspy2);
    pltePSPCol->setYErrorSymmetric(true);
    pltePSPCol->setDrawLine(true);
    plotPSP->getPlotter()->addGraph(pltePSPCol);

    pltePSPSize=new JKQTPXYParametrizedScatterGraph(plotPSP->getPlotter());
    pltePSPSize->setXColumn(pspx);
    pltePSPSize->setYColumn(pspy1);
    pltePSPSize->setSizeColumn(psps);
    pltePSPSize->setTitle(tr("changing size"));
    plotPSP->getPlotter()->addGraph(pltePSPSize);


    pltePSPSymb=new JKQTPXYParametrizedScatterGraph(plotPSP->getPlotter());
    pltePSPSymb->setXColumn(pspx);
    pltePSPSymb->setYColumn(pspy3);
    pltePSPSymb->setSymbolColumn(pspsy);
    pltePSPSymb->setTitle(tr("changing symbol"));
    plotPSP->getPlotter()->addGraph(pltePSPSymb);

    plotPSP->setPlotUpdateEnabled(true);
    plotPSP->zoomToFit();


    QCheckBox* chkPSPErrorSymmetric=new QCheckBox(tr("symmetric errors"), this);
    chkPSPErrorSymmetric->setChecked(true);
    connect(chkPSPErrorSymmetric, SIGNAL(toggled(bool)), this, SLOT(setPSPSymmErrors(bool)));
    layoutPSP->addWidget(chkPSPErrorSymmetric);







}

void TestWidgetParamScatterPlots::setPSPSymmErrors(bool checked)
{
     pltePSPCol->setYErrorSymmetric(checked);
     plotPSP->redrawPlot();
}
