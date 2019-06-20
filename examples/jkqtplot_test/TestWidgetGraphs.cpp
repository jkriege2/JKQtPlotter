#include "TestWidgetGraphs.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>
#include "jkqtplotter/jkqtpgraphsscatter.h"
#include "jkqtplotter/jkqtpgraphsboxplot.h"
#include "jkqtplotter/jkqtpgraphsfilledcurve.h"
#include "jkqtplotter/gui/jkqtpcomboboxes.h"




TestWidgetGraphs::TestWidgetGraphs(QWidget *parent) :
    QWidget(parent)
{

    ovlCnt=0;
    ovlTime.start();


    plot=new JKQTPlotter(true, this);
    plot->setPlotUpdateEnabled(false);
    plotBot=new JKQTPlotter(false, this, plot->getDatastore());
    plotBot2=new JKQTPlotter(false, this, plot->getDatastore());
    ovl1=new JKQTPOverlayVerticalLine(0.0, tr("test"), plot->getPlotter());
    plot->getPlotter()->addOverlayElement(ovl1);
    plot->setObjectName("plot");
    plotBot->setObjectName("plotBot");
    plotBot2->setObjectName("plotBot2");
    plot->getPlotter()->setPlotLabel("Plot Label $1.\\alpha$");
    plot->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");
    plotBot->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");
    plotBot2->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");

    QListView* listPlots=new QListView(this);
    listPlots->setModel(plot->getPlotter()->getPlotsModel());
    listPlots->setMaximumWidth(256);
    QVBoxLayout* layout=new QVBoxLayout(this);
    QGridLayout* layout_grid=new QGridLayout(this);
    layout_grid->addWidget(plot,0,0,1,1);
    layout_grid->addWidget(plotBot,1,0,1,1);
    layout_grid->addWidget(listPlots,0,1,1,1);
    layout_grid->setColumnStretch(0,1);
    layout->addLayout(layout_grid,2);
    layout->addWidget(plotBot2,1);
    setLayout(layout);
    resize(1000, 800);

    plotBot->synchronizeToMaster(plot, JKQTBasePlotter::sdXAxis, true, true, true);
    plot->getPlotter()->setGridPrinting(true);
    plot->getPlotter()->addGridPrintingPlotter(0,1,plotBot->getPlotter());
    plot->getPlotter()->addGridPrintingPlotter(0,2,plotBot2->getPlotter());

    plot->getPlotter()->getXAxis()->setAxisLabel("$x$ axis with greek letters $\\sqrt{\\alpha\\cdot\\beta}$");
    plot->getPlotter()->getYAxis()->setAxisLabel("$y$-axis label $f(x)=\\sin\\left(x^2\\right)$ [unit]");

    plotBot->getPlotter()->getXAxis()->setAxisLabel(plot->getPlotter()->getXAxis()->getAxisLabel());
    plotBot->getPlotter()->getYAxis()->setAxisLabel("$y$-axis 2 [unit]");
    plotBot2->getPlotter()->getXAxis()->setAxisLabel(plot->getPlotter()->getXAxis()->getAxisLabel());
    plotBot2->getPlotter()->getYAxis()->setAxisLabel("$y$-axis 3 [unit]");
    plotBot2->getPlotter()->getXAxis()->addAxisTickLabel(2.5, "tick 1");
    plotBot2->getPlotter()->getXAxis()->addAxisTickLabel(7.5, "tick 2");
    plotBot2->getPlotter()->getXAxis()->addAxisTickLabel(12.5, "tick 3");
    plotBot2->getPlotter()->getXAxis()->addAxisTickLabel(17.5, "tick 4");


    for (unsigned int i=0; i<N1; i++) {
        x1[i]=(double)(i+1)/(double)N1*8*M_PI;
        y11[i]=sin(x1[i]);
        y12[i]=cos(x1[i]);
        y13[i]=y12[i]/4.0;
        x2r[i]=double(rand()%250)/10.0;
        y21r[i]=double(rand()%100)/10.0-5.0;
        y21re[i]=y21r[i]*double(rand()%10)/40.0;
    }
    JKQTPDatastore* ds=plot->getPlotter()->getDatastore();

    //size_t cx1=ds->addColumn(x1, N1, "x1");
    //size_t cy11=ds->addColumn(y11, N1, "y11");
    //size_t cy12=ds->addColumn(y12, N1, "y12");
    //size_t cy13=ds->addColumn(y13, N1, "y13");

    //plot->addGraph(cx1, cy11, "$\\sin(x)$", JKQTPPoints);
    //plot->addGraph(cy12, cx1, "$\\cos(x)$", JKQTPFilledCurveY);
    //plot->addGraphWithXError(cy12, cx1, cy13, "$\\cos(x)$ with errors", JKQTPFilledCurveY, JKQTPErrorPolygons);

    for (unsigned int i=0; i<N2; i++) {
        x2[i]=(double)(i+1)/(double)N2*8*M_PI;
        y21[i]=5*sin(x2[i]);
        y22[i]=1;//1+sin(x2[i]*4);
        y23[i]=pow(5,x2[i]/5.0);
        y24[i]=x2[i]/10;
    }

    size_t cx2=ds->addColumn(x2, N2, "x2");
    size_t cy21=ds->addColumn(y21, N2, "y21");
    size_t cy22=ds->addColumn(y22, N2, "y22");
//    size_t cy23=ds->addColumn(y23, N2, "y23");
    //size_t cy24=ds->addColumn(y24, N2, "y24");
    size_t cx2r=ds->addColumn(x2r, N2, "x2r");
    size_t cy21r=ds->addColumn(y21r, N2, "y21r");
    size_t cy21re=ds->addColumn(y21re, N2, "y21re");


    size_t id=plot->getPlotter()->addGraph(cx2, cy21, "$5\\cdot\\sin(x)$", JKQTPFilledCurveX);
    JKQTPFilledCurveXGraph* fcxgr=qobject_cast<JKQTPFilledCurveXGraph*>(plot->getPlotter()->getGraph(id));
    if (fcxgr) {
        fcxgr->setBaseline(2);
    }

    size_t yeb=plot->getPlotter()->addGraphWithXYError(cx2, cy21, cy22, cy22, "sine with errors", JKQTPFilledCurveX);
    plteErrors=plot->getPlotter()->getGraph(yeb);
    setErrorLineStyle(0);
    yeb=plot->getPlotter()->addGraph(cx2, cy21, "$5\\cdot\\sin(x)$", JKQTPLinesPoints);
    plteSymbols=plot->getPlotter()->getGraph(yeb);

    pltePlot2=new JKQTPXYLineErrorGraph(plotBot->getPlotter());
    pltePlot2->setXColumn(cx2r);
    pltePlot2->setYColumn(cy21r);
    pltePlot2->setTitle(tr("random numbers"));
    pltePlot2->setDrawLine(true);
    pltePlot2->setYErrorColumn(cy21re);
    pltePlot2->setYErrorStyle(JKQTPErrorBarsPolygons);
    pltePlot2->setSymbolType(JKQTPFilledStar);
    plotBot->getPlotter()->addGraph(pltePlot2);
    plotBot->zoomToFit();


    for (unsigned int i=0; i<N3; i++) {
        if (i<N3/2) xbox[i]=pow(10, double(i)/N3*2.0);
        else xbox[i]=10+i*2.0;
        ymed[i]=i+3;
        ymean[i]=i+3.5;
        ymin[i]=ymed[i]-(i*2.0+1.5);
        ymax[i]=ymed[i]+(i*2.0+1.5);
        yp25[i]=ymed[i]-(i+0.5);
        yp75[i]=ymed[i]+(i+1);

        b1[i]=1.5+cos(i);
        b2[i]=1.5+cos(i+M_PI/8.0);
        b3[i]=1.5+sin(i);

        //plot->getXAxis()->addAxisTickLabel(i+1, QString("$\\pi_{%1}\\cdot 10^{%2}$").arg(i+1).arg(i+1-N3));
        //plot->getYAxis()->addAxisTickLabel(i+1, QString("$\\pi_{%1}\\cdot 10^{%2}$").arg(i+1).arg(i+1-N3));
    }

    size_t cbp=ds->addLinearColumn(N3, 2, N3+2, "boxplot_x");
    size_t cbxbox=ds->addColumn(xbox, N3, "xbox_log");
    size_t cbmean=ds->addColumn(ymean, N3, "ymean");
    size_t cbmed=ds->addColumn(ymed, N3, "ymed");
    size_t cbmin=ds->addColumn(ymin, N3, "ymin");
    size_t cbmax=ds->addColumn(ymax, N3, "ymax");
    size_t cbp25=ds->addColumn(yp25, N3, "yp25");
    size_t cbp75=ds->addColumn(yp75, N3, "yp75");


    QVector<size_t> cb;
    cb.push_back(ds->addColumn(b1, N3, "b1"));
    cb.push_back(ds->addColumn(b2, N3, "b2"));
    cb.push_back(ds->addColumn(b3, N3, "b3"));
    QStringList ts;
    ts<<"bars 1"<<"bars 2"<<"bars 3";
    plot->getPlotter()->addVerticalBargraph(cb, cbp, ts);

    JKQTPVerticalRange* r1=new JKQTPVerticalRange(plot->getPlotter());
    r1->setRangeMin(5);
    r1->setRangeMax(10);
    r1->setRangeCenter(7.5);
    r1->setTitle("vert. range");
    plot->getPlotter()->addGraph(r1);

    JKQTPHorizontalRange* r2=new JKQTPHorizontalRange(plot->getPlotter());
    r2->setRangeMin(5);
    r2->setRangeMax(10);
    r2->setRangeCenter(7.5);
    r2->setTitle("hor. range");
    plot->getPlotter()->addGraph(r2);

    JKQTPBoxplotVerticalGraph* box=new JKQTPBoxplotVerticalGraph(plotBot2->getPlotter());
    box->setPositionColumn(cbxbox);
    box->setMedianColumn(cbmed);
    box->setMeanColumn(cbmean);
    box->setMinColumn(cbmin);
    box->setMaxColumn(cbmax);
    box->setPercentile25Column(cbp25);
    box->setPercentile75Column(cbp75);
    box->setTitle("Boxplot");
    plotBot2->addGraph(box);
    plotBot2->zoomToFit();

    /*JKQTPYFunctionLineGraph* g=new JKQTPYFunctionLineGraph(&plot);
    g->setTitle("function 1");
    g->setPlotFunctionFunctor(f1);
    g->setErrorFunction(f1e);
    g->setDrawErrorLines(true);
    g->setDrawErrorPolygons(false);
    g->setColor(QColor("red"));
    plot->addGraph(g);*/

    /*JKQTPXFunctionLineGraph* g=new JKQTPXFunctionLineGraph(&plot);
    g->setTitle("function 2");
    g->setPlotFunctionFunctor(f2);
    plot->addGraph(g);*/

    /*JKQTPSpecialLineVerticalGraph* g=new JKQTPSpecialLineVerticalGraph(&plot);
    g->setXColumn(cy11);
    g->setYColumn(cx1);
    g->setFillCurve(true);
    g->setValuesCentered(true);
    plot->addGraph(g);
    plot->addGraph(cy11, cx1, "$\\sin(x)$", JKQTPPoints);*/

    plot->setPlotUpdateEnabled(true);
    plot->getPlotter()->zoomToFit(true, false);//, true);
    plot->getPlotter()->setKeyAutosize(true);

    QCheckBox* chklogX=new QCheckBox("logarithmic X-axis", this);
    QCheckBox* chklogY=new QCheckBox("logarithmic Y-axis", this);
    connect(chklogX, SIGNAL(toggled(bool)), plot->getPlotter()->getXAxis(), SLOT(setLogAxis(bool)));
    connect(chklogY, SIGNAL(toggled(bool)), plot->getPlotter()->getYAxis(), SLOT(setLogAxis(bool)));
    connect(chklogX, SIGNAL(toggled(bool)), plotBot2->getPlotter()->getXAxis(), SLOT(setLogAxis(bool)));
    connect(chklogY, SIGNAL(toggled(bool)), plotBot2->getPlotter()->getYAxis(), SLOT(setLogAxis(bool)));
    layout->addWidget(chklogX);
    layout->addWidget(chklogY);
    QCheckBox* chkInvertX=new QCheckBox("invert X-axis", this);
    QCheckBox* chkInvertY=new QCheckBox("invert Y-axis", this);
    connect(chkInvertX, SIGNAL(toggled(bool)), plot->getPlotter()->getXAxis(), SLOT(setInverted(bool)));
    connect(chkInvertY, SIGNAL(toggled(bool)), plot->getPlotter()->getYAxis(), SLOT(setInverted(bool)));
    layout->addWidget(chkInvertX);
    layout->addWidget(chkInvertY);
    layout->addWidget(chkInvertX);
    layout->addWidget(chkInvertY);
    JKQTPSymbolComboBox* cmbJKQTPPlotSymbol=new JKQTPSymbolComboBox(this);
    cmbJKQTPPlotSymbol->setSymbolType(JKQTPPlus);
    connect(cmbJKQTPPlotSymbol, SIGNAL(currentIndexChanged(int)), this, SLOT(setESSymbol(int)));
    layout->addWidget(cmbJKQTPPlotSymbol);
    JKQTPErrorPlotstyleComboBox* cmbErrors=new JKQTPErrorPlotstyleComboBox(this);
    cmbErrors->setCurrentErrorStyle(JKQTPErrorBars);
    connect(cmbErrors, SIGNAL(currentIndexChanged(int)), this, SLOT(setErrorLineStyle(int)));
    layout->addWidget(cmbErrors);
    QDoubleSpinBox* spinScale=new QDoubleSpinBox(this);
    spinScale->setRange(0.1,5);
    spinScale->setSingleStep(0.1);
    spinScale->setValue(1);
    spinScale->setPrefix("magnification:  ");
    layout->addWidget(spinScale);
    connect(spinScale, SIGNAL(valueChanged(double)), plot, SLOT(setMagnification(double)));
    connect(spinScale, SIGNAL(valueChanged(double)), plotBot, SLOT(setMagnification(double)));
    connect(spinScale, SIGNAL(valueChanged(double)), plotBot2, SLOT(setMagnification(double)));

    QComboBox* cmbSortOrder2=new QComboBox(this);
    cmbSortOrder2->addItem(tr("plot 2: unsorted"));
    cmbSortOrder2->addItem(tr("plot 2: sort X"));
    cmbSortOrder2->addItem(tr("plot 2: sort Y"));
    connect(cmbSortOrder2, SIGNAL(currentIndexChanged(int)), this, SLOT(setSortOrder2(int)));
    layout->addWidget(cmbSortOrder2);


    JKQTPKeyLayoutComboBox* cmbKeyLayout=new JKQTPKeyLayoutComboBox(this);
    connect(cmbKeyLayout, SIGNAL(currentLayoutChanged(JKQTPKeyLayout)), this, SLOT(setKeyLayout2(JKQTPKeyLayout)));
    layout->addWidget(cmbKeyLayout);


    JKQTPKeyPositionComboBox* cmbKeyPos=new JKQTPKeyPositionComboBox(this);
    connect(cmbKeyPos, SIGNAL(currentPositionChanged(JKQTPKeyPosition)), this, SLOT(setKeyPos2(JKQTPKeyPosition)));
    layout->addWidget(cmbKeyPos);














    QTimer::singleShot(50, this, SLOT(moveovl()));

}

void TestWidgetGraphs::moveovl() {
    ovl1->setPosition(ovl1->getPosition()+0.1);
    if (ovl1->getPosition()>30) ovl1->setPosition(0);
    ovlCnt++;
    if (ovlCnt>20) {
        double fps=double(ovlCnt)/(double(ovlTime.elapsed())/1000.0);
        ovl1->setText(QString("%1 fps").arg(fps));
        ovlCnt=0;
        ovlTime.start();
    }
    plot->redrawOverlays();
    QTimer::singleShot(10, this, SLOT(moveovl()));
}


void TestWidgetGraphs::setErrorLineStyle(int /*index*/)
{
    QComboBox* cmb=qobject_cast<QComboBox*>(sender());
    if (cmb) {
        JKQTPXYGraphErrors* exy=dynamic_cast<JKQTPXYGraphErrors*>(plteErrors);
//        JKQTPXGraphErrors* ex=dynamic_cast<JKQTPXGraphErrors*>(plteErrors);
        JKQTPYGraphErrors* ey=dynamic_cast<JKQTPYGraphErrors*>(plteErrors);
        if (exy) {
            exy->setYErrorStyle(String2JKQTPErrorPlotstyle(cmb->itemData(cmb->currentIndex()).toString()));
        }
        if (ey) {
            ey->setYErrorStyle(String2JKQTPErrorPlotstyle(cmb->itemData(cmb->currentIndex()).toString()));
        }
        plot->redrawPlot();
    }
}

void TestWidgetGraphs::setESSymbol(int /*index*/)
{
    QComboBox* cmb=qobject_cast<QComboBox*>(sender());
    if (cmb) {
        JKQTPXYLineGraph* exy=dynamic_cast<JKQTPXYLineGraph*>(plteSymbols);
        if (exy) {
            exy->setSymbolType(String2JKQTPGraphSymbols(cmb->itemData(cmb->currentIndex()).toString()));
        }

        plot->redrawPlot();
    }
}


void TestWidgetGraphs::setSortOrder2(int index)
{
    if (index==0) pltePlot2->setDataSortOrder(JKQTPXYGraph::Unsorted);
    if (index==1) pltePlot2->setDataSortOrder(JKQTPXYLineGraph::SortedX);
    if (index==2) pltePlot2->setDataSortOrder(JKQTPXYLineGraph::SortedY);
    plotBot->redrawPlot();
}

void TestWidgetGraphs::setKeyLayout2(JKQTPKeyLayout layout)
{
    plot->getPlotter()->setKeyLayout(layout);
    plotBot->getPlotter()->setKeyLayout(layout);
    plotBot2->getPlotter()->setKeyLayout(layout);
    plot->redrawPlot();
    plotBot->redrawPlot();
    plotBot2->redrawPlot();
}

void TestWidgetGraphs::setKeyPos2(JKQTPKeyPosition layout)
{
    plot->getPlotter()->setKeyPosition(layout);
    plotBot->getPlotter()->setKeyPosition(layout);
    plotBot2->getPlotter()->setKeyPosition(layout);
    plot->redrawPlot();
    plotBot->redrawPlot();
    plotBot2->redrawPlot();
}
