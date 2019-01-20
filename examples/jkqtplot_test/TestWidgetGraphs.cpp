#include "TestWidgetGraphs.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsboxplot.h"
#include "jkqtplotter/jkqtpgraphsfilledcurve.h"
#include "jkqtplottergui/jkqtpcomboboxes.h"




TestWidgetGraphs::TestWidgetGraphs(QWidget *parent) :
    QWidget(parent)
{

    ovlCnt=0;
    ovlTime.start();


    plot=new JKQTPlotter(true, this);
    plot->set_doDrawing(false);
    plotBot=new JKQTPlotter(false, this, plot->getDatastore());
    plotBot2=new JKQTPlotter(false, this, plot->getDatastore());
    ovl1=new JKQTPOverlayVerticalLine(0.0, tr("test"), plot->get_plotter());
    plot->get_plotter()->addOverlayElement(ovl1);
    plot->setObjectName("plot");
    plotBot->setObjectName("plotBot");
    plotBot2->setObjectName("plotBot2");
    plot->get_plotter()->set_plotLabel("Plot Label $1.\\alpha$");
    plot->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");
    plotBot->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");
    plotBot2->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");

    QListView* listPlots=new QListView(this);
    listPlots->setModel(plot->get_plotter()->getPlotsModel());
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

    plotBot->synchronizeToMaster(plot, true, false);
    connect(plot, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTPlotter*)), plotBot, SLOT(synchronizeXAxis(double,double,double,double,JKQTPlotter*)));
    plot->get_plotter()->set_gridPrinting(true);
    plot->get_plotter()->addGridPrintingPlotter(0,1,plotBot->get_plotter());
    plot->get_plotter()->addGridPrintingPlotter(0,2,plotBot2->get_plotter());

    plot->get_plotter()->get_xAxis()->set_axisLabel("$x$ axis with greek letters $\\sqrt{\\alpha\\cdot\\beta}$");
    plot->get_plotter()->get_yAxis()->set_axisLabel("$y$-axis label $f(x)=\\sin\\left(x^2\\right)$ [unit]");

    plotBot->get_plotter()->get_xAxis()->set_axisLabel(plot->get_plotter()->get_xAxis()->get_axisLabel());
    plotBot->get_plotter()->get_yAxis()->set_axisLabel("$y$-axis 2 [unit]");
    plotBot2->get_plotter()->get_xAxis()->set_axisLabel(plot->get_plotter()->get_xAxis()->get_axisLabel());
    plotBot2->get_plotter()->get_yAxis()->set_axisLabel("$y$-axis 3 [unit]");
    plotBot2->get_plotter()->get_xAxis()->addAxisTickLabel(2.5, "tick 1");
    plotBot2->get_plotter()->get_xAxis()->addAxisTickLabel(7.5, "tick 2");
    plotBot2->get_plotter()->get_xAxis()->addAxisTickLabel(12.5, "tick 3");
    plotBot2->get_plotter()->get_xAxis()->addAxisTickLabel(17.5, "tick 4");


    for (unsigned int i=0; i<N1; i++) {
        x1[i]=(double)(i+1)/(double)N1*8*M_PI;
        y11[i]=sin(x1[i]);
        y12[i]=cos(x1[i]);
        y13[i]=y12[i]/4.0;
        x2r[i]=double(rand()%250)/10.0;
        y21r[i]=double(rand()%100)/10.0-5.0;
        y21re[i]=y21r[i]*double(rand()%10)/40.0;
    }
    JKQTPDatastore* ds=plot->get_plotter()->getDatastore();

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


    size_t id=plot->get_plotter()->addGraph(cx2, cy21, "$5\\cdot\\sin(x)$", JKQTPFilledCurveX);
    JKQTPFilledCurveXGraph* fcxgr=qobject_cast<JKQTPFilledCurveXGraph*>(plot->get_plotter()->getGraph(id));
    if (fcxgr) {
        fcxgr->set_baseline(2);
    }

    size_t yeb=plot->get_plotter()->addGraphWithXYError(cx2, cy21, cy22, cy22, "sine with errors", JKQTPFilledCurveX);
    plteErrors=plot->get_plotter()->getGraph(yeb);
    setErrorStyle(0);
    yeb=plot->get_plotter()->addGraph(cx2, cy21, "$5\\cdot\\sin(x)$", JKQTPLinesPoints);
    plteSymbols=plot->get_plotter()->getGraph(yeb);

    pltePlot2=new JKQTPXYLineErrorGraph(plotBot->get_plotter());
    pltePlot2->set_xColumn(cx2r);
    pltePlot2->set_yColumn(cy21r);
    pltePlot2->set_title(tr("random numbers"));
    pltePlot2->set_drawLine(true);
    pltePlot2->set_yErrorColumn(cy21re);
    pltePlot2->set_yErrorStyle(JKQTPErrorBarsPolygons);
    pltePlot2->set_symbol(JKQTPFilledStar);
    plotBot->get_plotter()->addGraph(pltePlot2);
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

        //plot->get_xAxis()->addAxisTickLabel(i+1, QString("$\\pi_{%1}\\cdot 10^{%2}$").arg(i+1).arg(i+1-N3));
        //plot->get_yAxis()->addAxisTickLabel(i+1, QString("$\\pi_{%1}\\cdot 10^{%2}$").arg(i+1).arg(i+1-N3));
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
    plot->get_plotter()->addVerticalBargraph(cb, cbp, ts);

    JKQTPVerticalRange* r1=new JKQTPVerticalRange(plot->get_plotter());
    r1->set_rangeMin(5);
    r1->set_rangeMax(10);
    r1->set_rangeCenter(7.5);
    r1->set_title("vert. range");
    plot->get_plotter()->addGraph(r1);

    JKQTPHorizontalRange* r2=new JKQTPHorizontalRange(plot->get_plotter());
    r2->set_rangeMin(5);
    r2->set_rangeMax(10);
    r2->set_rangeCenter(7.5);
    r2->set_title("hor. range");
    plot->get_plotter()->addGraph(r2);

    JKQTPBoxplotVerticalGraph* box=new JKQTPBoxplotVerticalGraph(plotBot2->get_plotter());
    box->set_posColumn(cbxbox);
    box->set_medianColumn(cbmed);
    box->set_meanColumn(cbmean);
    box->set_minColumn(cbmin);
    box->set_maxColumn(cbmax);
    box->set_percentile25Column(cbp25);
    box->set_percentile75Column(cbp75);
    box->set_title("Boxplot");
    plotBot2->addGraph(box);
    plotBot2->zoomToFit();

    /*JKQTPYFunctionLineGraph* g=new JKQTPYFunctionLineGraph(&plot);
    g->set_title("function 1");
    g->set_plotFunction(f1);
    g->set_errorFunction(f1e);
    g->set_drawErrorLines(true);
    g->set_drawErrorPolygons(false);
    g->set_color(QColor("red"));
    plot->addGraph(g);*/

    /*JKQTPXFunctionLineGraph* g=new JKQTPXFunctionLineGraph(&plot);
    g->set_title("function 2");
    g->set_plotFunction(f2);
    plot->addGraph(g);*/

    /*JKQTPStepVerticalGraph* g=new JKQTPStepVerticalGraph(&plot);
    g->set_xColumn(cy11);
    g->set_yColumn(cx1);
    g->set_fillCurve(true);
    g->set_valuesCentered(true);
    plot->addGraph(g);
    plot->addGraph(cy11, cx1, "$\\sin(x)$", JKQTPPoints);*/

    plot->set_doDrawing(true);
    plot->get_plotter()->zoomToFit(true, false);//, true);
    plot->get_plotter()->set_keyAutosize(true);

    QCheckBox* chklogX=new QCheckBox("logarithmic X-axis", this);
    QCheckBox* chklogY=new QCheckBox("logarithmic Y-axis", this);
    connect(chklogX, SIGNAL(toggled(bool)), plot->get_plotter()->get_xAxis(), SLOT(set_logAxis(bool)));
    connect(chklogY, SIGNAL(toggled(bool)), plot->get_plotter()->get_yAxis(), SLOT(set_logAxis(bool)));
    connect(chklogX, SIGNAL(toggled(bool)), plotBot2->get_plotter()->get_xAxis(), SLOT(set_logAxis(bool)));
    connect(chklogY, SIGNAL(toggled(bool)), plotBot2->get_plotter()->get_yAxis(), SLOT(set_logAxis(bool)));
    layout->addWidget(chklogX);
    layout->addWidget(chklogY);
    QCheckBox* chkInvertX=new QCheckBox("invert X-axis", this);
    QCheckBox* chkInvertY=new QCheckBox("invert Y-axis", this);
    connect(chkInvertX, SIGNAL(toggled(bool)), plot->get_plotter()->get_xAxis(), SLOT(set_inverted(bool)));
    connect(chkInvertY, SIGNAL(toggled(bool)), plot->get_plotter()->get_yAxis(), SLOT(set_inverted(bool)));
    layout->addWidget(chkInvertX);
    layout->addWidget(chkInvertY);
    layout->addWidget(chkInvertX);
    layout->addWidget(chkInvertY);
    JKQTPSymbolComboBox* cmbJKQTPPlotSymbol=new JKQTPSymbolComboBox(this);
    cmbJKQTPPlotSymbol->setSymbol(JKQTPPlus);
    connect(cmbJKQTPPlotSymbol, SIGNAL(currentIndexChanged(int)), this, SLOT(setESSymbol(int)));
    layout->addWidget(cmbJKQTPPlotSymbol);
    JKQTPErrorPlotstyleComboBox* cmbErrors=new JKQTPErrorPlotstyleComboBox(this);
    cmbErrors->setCurrentErrorStyle(JKQTPErrorBars);
    connect(cmbErrors, SIGNAL(currentIndexChanged(int)), this, SLOT(setErrorStyle(int)));
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
    ovl1->set_position(ovl1->get_position()+0.1);
    if (ovl1->get_position()>30) ovl1->set_position(0);
    ovlCnt++;
    if (ovlCnt>20) {
        double fps=double(ovlCnt)/(double(ovlTime.elapsed())/1000.0);
        ovl1->set_text(QString("%1 fps").arg(fps));
        ovlCnt=0;
        ovlTime.start();
    }
    plot->update_overlays();
    QTimer::singleShot(10, this, SLOT(moveovl()));
}


void TestWidgetGraphs::setErrorStyle(int /*index*/)
{
    QComboBox* cmb=qobject_cast<QComboBox*>(sender());
    if (cmb) {
        JKQTPXYGraphErrors* exy=dynamic_cast<JKQTPXYGraphErrors*>(plteErrors);
//        JKQTPXGraphErrors* ex=dynamic_cast<JKQTPXGraphErrors*>(plteErrors);
        JKQTPYGraphErrors* ey=dynamic_cast<JKQTPYGraphErrors*>(plteErrors);
        if (exy) {
            exy->set_yErrorStyle(String2JKQTPErrorPlotstyle(cmb->itemData(cmb->currentIndex()).toString()));
        }
        if (ey) {
            ey->set_yErrorStyle(String2JKQTPErrorPlotstyle(cmb->itemData(cmb->currentIndex()).toString()));
        }
        plot->update_plot();
    }
}

void TestWidgetGraphs::setESSymbol(int /*index*/)
{
    QComboBox* cmb=qobject_cast<QComboBox*>(sender());
    if (cmb) {
        JKQTPXYLineGraph* exy=dynamic_cast<JKQTPXYLineGraph*>(plteSymbols);
        if (exy) {
            exy->set_symbol(String2JKQTPGraphSymbols(cmb->itemData(cmb->currentIndex()).toString()));
        }

        plot->update_plot();
    }
}


void TestWidgetGraphs::setSortOrder2(int index)
{
    if (index==0) pltePlot2->set_sortData(JKQTPXYGraph::Unsorted);
    if (index==1) pltePlot2->set_sortData(JKQTPXYLineGraph::SortedX);
    if (index==2) pltePlot2->set_sortData(JKQTPXYLineGraph::SortedY);
    plotBot->update_plot();
}

void TestWidgetGraphs::setKeyLayout2(JKQTPKeyLayout layout)
{
    plot->get_plotter()->set_keyLayout(layout);
    plotBot->get_plotter()->set_keyLayout(layout);
    plotBot2->get_plotter()->set_keyLayout(layout);
    plot->update_plot();
    plotBot->update_plot();
    plotBot2->update_plot();
}

void TestWidgetGraphs::setKeyPos2(JKQTPKeyPosition layout)
{
    plot->get_plotter()->set_keyPosition(layout);
    plotBot->get_plotter()->set_keyPosition(layout);
    plotBot2->get_plotter()->set_keyPosition(layout);
    plot->update_plot();
    plotBot->update_plot();
    plotBot2->update_plot();
}
