#include "TestWidgetParamScatterPlots.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>






TestWidgetParamScatterPlots::TestWidgetParamScatterPlots(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout* layoutPSP=new QVBoxLayout(this);

    setLayout(layoutPSP);
    resize(1000, 800);
    plotPSP=new JKQtPlotter(true, this);
    plotPSP->set_doDrawing(false);
    plotPSP->get_plotter()->set_plotLabel(tr("\\textbf{parametrized scatter plots}"));
    plotPSP->setObjectName("plotPSP");
    plotPSP->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");

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

    pltePSPCol=new JKQTPxyParametrizedErrorScatterGraph(plotPSP->get_plotter());
    pltePSPCol->set_title(tr("changing color"));
    pltePSPCol->set_xColumn(pspx);
    pltePSPCol->set_yColumn(pspy);
    pltePSPCol->set_colorColumn(pspc);
    pltePSPCol->set_yErrorColumn(pspy1);
    pltePSPCol->set_yErrorColumnLower(pspy2);
    pltePSPCol->set_yErrorSymmetric(true);
    pltePSPCol->set_drawLine(true);
    plotPSP->get_plotter()->addGraph(pltePSPCol);

    pltePSPSize=new JKQTPxyParametrizedScatterGraph(plotPSP->get_plotter());
    pltePSPSize->set_xColumn(pspx);
    pltePSPSize->set_yColumn(pspy1);
    pltePSPSize->set_sizeColumn(psps);
    pltePSPSize->set_title(tr("changing size"));
    plotPSP->get_plotter()->addGraph(pltePSPSize);


    pltePSPSymb=new JKQTPxyParametrizedScatterGraph(plotPSP->get_plotter());
    pltePSPSymb->set_xColumn(pspx);
    pltePSPSymb->set_yColumn(pspy3);
    pltePSPSymb->set_symbolColumn(pspsy);
    pltePSPSymb->set_title(tr("changing symbol"));
    plotPSP->get_plotter()->addGraph(pltePSPSymb);

    plotPSP->set_doDrawing(true);
    plotPSP->zoomToFit();


    QCheckBox* chkPSPErrorSymmetric=new QCheckBox(tr("symmetric errors"), this);
    chkPSPErrorSymmetric->setChecked(true);
    connect(chkPSPErrorSymmetric, SIGNAL(toggled(bool)), this, SLOT(setPSPSymmErrors(bool)));
    layoutPSP->addWidget(chkPSPErrorSymmetric);







}

void TestWidgetParamScatterPlots::setPSPSymmErrors(bool checked)
{
     pltePSPCol->set_yErrorSymmetric(checked);
     plotPSP->update_plot();
}
