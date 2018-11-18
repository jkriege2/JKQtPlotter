#include "testmain.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>
#ifdef Q_WS_WIN
#include "jkqtpemfengineadapter.h"
#endif


double f1(double x, void* /*param*/) {
    return x/30;
}

double f1e(double x, void* /*param*/) {
    return 2;
}

double f2(double x, void* /*param*/) {
    return 10*sin(exp(x/100));
}




TestMain::TestMain(QWidget *parent) :
    QTabWidget(parent)
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    #ifdef Q_WS_WIN
        JKQtBasePlotter::registerPaintDeviceAdapter(new JKQTPEmfEngineAdapter());
    #endif
#endif
    QWidget* win=new QWidget(this);

    ovlCnt=0;
    ovlTime.start();


    plot=new JKQtPlotter(true, win);
    plot->set_doDrawing(false);
    plotBot=new JKQtPlotter(false, win, plot->getDatastore());
    plotBot2=new JKQtPlotter(false, win, plot->getDatastore());
    ovl1=new JKQTPoverlayVerticalLine(0.0, tr("test"), plot->get_plotter());
    plot->get_plotter()->addOverlayElement(ovl1);
    plot->setObjectName("plot");
    plotBot->setObjectName("plotBot");
    plotBot2->setObjectName("plotBot2");
    plot->get_plotter()->set_plotLabel("Plot Label $1.\\alpha$");
    plot->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");
    plotBot->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");
    plotBot2->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot/");

    QListView* listPlots=new QListView(win);
    listPlots->setModel(plot->get_plotter()->getPlotsModel());
    listPlots->setMaximumWidth(256);
    QVBoxLayout* layout=new QVBoxLayout(win);
    QGridLayout* layout_grid=new QGridLayout(win);
    layout_grid->addWidget(plot,0,0,1,1);
    layout_grid->addWidget(plotBot,1,0,1,1);
    layout_grid->addWidget(listPlots,0,1,1,1);
    layout_grid->setColumnStretch(0,1);
    layout->addLayout(layout_grid,2);
    layout->addWidget(plotBot2,1);
    win->setLayout(layout);
    win->resize(1000, 800);

    plotBot->synchronizeToMaster(plot, true, false);
    connect(plot, SIGNAL(zoomChangedLocally(double,double,double,double,JKQtPlotter*)), plotBot, SLOT(synchronizeXAxis(double,double,double,double,JKQtPlotter*)));
    plot->get_plotter()->set_gridPrinting(true);
    plot->get_plotter()->addGridPrintingPlotter(0,1,plotBot->get_plotter());
    plot->get_plotter()->addGridPrintingPlotter(0,2,plotBot2->get_plotter());

    plot->get_plotter()->getXAxis()->set_axisLabel("$x$ axis with greek letters $\\sqrt{\\alpha\\cdot\\beta}$");
    plot->get_plotter()->getYAxis()->set_axisLabel("$y$-axis label $f(x)=\\sin\\left(x^2\\right)$ [unit]");

    plotBot->get_plotter()->getXAxis()->set_axisLabel(plot->get_plotter()->getXAxis()->get_axisLabel());
    plotBot->get_plotter()->getYAxis()->set_axisLabel("$y$-axis 2 [unit]");
    plotBot2->get_plotter()->getXAxis()->set_axisLabel(plot->get_plotter()->getXAxis()->get_axisLabel());
    plotBot2->get_plotter()->getYAxis()->set_axisLabel("$y$-axis 3 [unit]");
    plotBot2->get_plotter()->getXAxis()->addAxisTickLabel(2.5, "tick 1");
    plotBot2->get_plotter()->getXAxis()->addAxisTickLabel(7.5, "tick 2");
    plotBot2->get_plotter()->getXAxis()->addAxisTickLabel(12.5, "tick 3");
    plotBot2->get_plotter()->getXAxis()->addAxisTickLabel(17.5, "tick 4");


    for (unsigned int i=0; i<N1; i++) {
        x1[i]=(double)(i+1)/(double)N1*8*M_PI;
        y11[i]=sin(x1[i]);
        y12[i]=cos(x1[i]);
        y13[i]=y12[i]/4.0;
        x2r[i]=double(rand()%250)/10.0;
        y21r[i]=double(rand()%100)/10.0-5.0;
        y21re[i]=y21r[i]*double(rand()%10)/40.0;
    }
    JKQTPdatastore* ds=plot->get_plotter()->getDatastore();

    //size_t cx1=ds->addColumn(x1, N1, "x1");
    //size_t cy11=ds->addColumn(y11, N1, "y11");
    //size_t cy12=ds->addColumn(y12, N1, "y12");
    //size_t cy13=ds->addColumn(y13, N1, "y13");

    //plot->addGraph(cx1, cy11, "$\\sin(x)$", JKQTPpoints);
    //plot->addGraph(cy12, cx1, "$\\cos(x)$", JKQTPfilledCurveY);
    //plot->addGraphWithXError(cy12, cx1, cy13, "$\\cos(x)$ with errors", JKQTPfilledCurveY, JKQTPerrorPolygons);

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
    size_t cy23=ds->addColumn(y23, N2, "y23");
    //size_t cy24=ds->addColumn(y24, N2, "y24");
    size_t cx2r=ds->addColumn(x2r, N2, "x2r");
    size_t cy21r=ds->addColumn(y21r, N2, "y21r");
    size_t cy21re=ds->addColumn(y21re, N2, "y21re");


    size_t id=plot->get_plotter()->addGraph(cx2, cy21, "$5\\cdot\\sin(x)$", JKQTPfilledCurveX);
    JKQTPfilledCurveXGraph* fcxgr=qobject_cast<JKQTPfilledCurveXGraph*>(plot->get_plotter()->getGraph(id));
    if (fcxgr) {
        fcxgr->set_baseline(2);
    }

    size_t yeb=plot->get_plotter()->addGraphWithXYError(cx2, cy21, cy22, cy22, "sine with errors", JKQTPfilledCurveX);
    plteErrors=plot->get_plotter()->getGraph(yeb);
    setErrorStyle(0);
    yeb=plot->get_plotter()->addGraph(cx2, cy21, "$5\\cdot\\sin(x)$", JKQTPlinesPoints);
    plteSymbols=plot->get_plotter()->getGraph(yeb);

    pltePlot2=new JKQTPxyLineErrorGraph(plotBot->get_plotter());
    pltePlot2->set_xColumn(cx2r);
    pltePlot2->set_yColumn(cy21r);
    pltePlot2->set_title(tr("random numbers"));
    pltePlot2->set_drawLine(true);
    pltePlot2->set_yErrorColumn(cy21re);
    pltePlot2->set_yErrorStyle(JKQTPerrorBarsPolygons);
    pltePlot2->set_symbol(JKQTPfilledStar);
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
    plot->get_plotter()->addVerticalBargraph(cb, cbp, ts);

    JKQTPverticalRange* r1=new JKQTPverticalRange(plot->get_plotter());
    r1->set_rangeMin(5);
    r1->set_rangeMax(10);
    r1->set_rangeCenter(7.5);
    r1->set_title("vert. range");
    plot->get_plotter()->addGraph(r1);

    JKQTPhorizontalRange* r2=new JKQTPhorizontalRange(plot->get_plotter());
    r2->set_rangeMin(5);
    r2->set_rangeMax(10);
    r2->set_rangeCenter(7.5);
    r2->set_title("hor. range");
    plot->get_plotter()->addGraph(r2);

    JKQTPboxplotVerticalGraph* box=new JKQTPboxplotVerticalGraph(plotBot2->get_plotter());
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

    /*JKQTPyFunctionLineGraph* g=new JKQTPyFunctionLineGraph(&plot);
    g->set_title("function 1");
    g->set_plotFunction(f1);
    g->set_errorFunction(f1e);
    g->set_drawErrorLines(true);
    g->set_drawErrorPolygons(false);
    g->set_color(QColor("red"));
    plot->addGraph(g);*/

    /*JKQTPxFunctionLineGraph* g=new JKQTPxFunctionLineGraph(&plot);
    g->set_title("function 2");
    g->set_plotFunction(f2);
    plot->addGraph(g);*/

    /*JKQTPstepVerticalGraph* g=new JKQTPstepVerticalGraph(&plot);
    g->set_xColumn(cy11);
    g->set_yColumn(cx1);
    g->set_fillCurve(true);
    g->set_valuesCentered(true);
    plot->addGraph(g);
    plot->addGraph(cy11, cx1, "$\\sin(x)$", JKQTPpoints);*/

    plot->set_doDrawing(true);
    plot->get_plotter()->zoomToFit(true, false);//, true);
    plot->get_plotter()->set_keyAutosize(true);

    QCheckBox* chklogX=new QCheckBox("logarithmic X-axis", win);
    QCheckBox* chklogY=new QCheckBox("logarithmic Y-axis", win);
    connect(chklogX, SIGNAL(toggled(bool)), plot->get_plotter()->getXAxis(), SLOT(set_logAxis(bool)));
    connect(chklogY, SIGNAL(toggled(bool)), plot->get_plotter()->getYAxis(), SLOT(set_logAxis(bool)));
    connect(chklogX, SIGNAL(toggled(bool)), plotBot2->get_plotter()->getXAxis(), SLOT(set_logAxis(bool)));
    connect(chklogY, SIGNAL(toggled(bool)), plotBot2->get_plotter()->getYAxis(), SLOT(set_logAxis(bool)));
    layout->addWidget(chklogX);
    layout->addWidget(chklogY);
    QCheckBox* chkInvertX=new QCheckBox("invert X-axis", win);
    QCheckBox* chkInvertY=new QCheckBox("invert Y-axis", win);
    connect(chkInvertX, SIGNAL(toggled(bool)), plot->get_plotter()->getXAxis(), SLOT(set_inverted(bool)));
    connect(chkInvertY, SIGNAL(toggled(bool)), plot->get_plotter()->getYAxis(), SLOT(set_inverted(bool)));
    layout->addWidget(chkInvertX);
    layout->addWidget(chkInvertY);
    layout->addWidget(chkInvertX);
    layout->addWidget(chkInvertY);
    JKQTPSymbolComboBox* cmbPlotSymbol=new JKQTPSymbolComboBox(win);
    cmbPlotSymbol->setSymbol(JKQTPplus);
    connect(cmbPlotSymbol, SIGNAL(currentIndexChanged(int)), this, SLOT(setESSymbol(int)));
    layout->addWidget(cmbPlotSymbol);
    JKQTPerrorPlotstyleComboBox* cmbErrors=new JKQTPerrorPlotstyleComboBox(win);
    cmbErrors->setCurrentErrorStyle(JKQTPerrorBars);
    connect(cmbErrors, SIGNAL(currentIndexChanged(int)), this, SLOT(setErrorStyle(int)));
    layout->addWidget(cmbErrors);
    QDoubleSpinBox* spinScale=new QDoubleSpinBox(win);
    spinScale->setRange(0.1,5);
    spinScale->setSingleStep(0.1);
    spinScale->setValue(1);
    spinScale->setPrefix("magnification:  ");
    layout->addWidget(spinScale);
    connect(spinScale, SIGNAL(valueChanged(double)), plot, SLOT(setMagnification(double)));
    connect(spinScale, SIGNAL(valueChanged(double)), plotBot, SLOT(setMagnification(double)));
    connect(spinScale, SIGNAL(valueChanged(double)), plotBot2, SLOT(setMagnification(double)));

    QComboBox* cmbSortOrder2=new QComboBox(win);
    cmbSortOrder2->addItem(tr("plot 2: unsorted"));
    cmbSortOrder2->addItem(tr("plot 2: sort X"));
    cmbSortOrder2->addItem(tr("plot 2: sort Y"));
    connect(cmbSortOrder2, SIGNAL(currentIndexChanged(int)), this, SLOT(setSortOrder2(int)));
    layout->addWidget(cmbSortOrder2);


    JKQTPkeyLayoutComboBox* cmbKeyLayout=new JKQTPkeyLayoutComboBox(win);
    connect(cmbKeyLayout, SIGNAL(currentLayoutChanged(JKQTPkeyLayout)), this, SLOT(setKeyLayout2(JKQTPkeyLayout)));
    layout->addWidget(cmbKeyLayout);


    JKQTPkeyPositionComboBox* cmbKeyPos=new JKQTPkeyPositionComboBox(win);
    connect(cmbKeyPos, SIGNAL(currentPositionChanged(JKQTPkeyPosition)), this, SLOT(setKeyPos2(JKQTPkeyPosition)));
    layout->addWidget(cmbKeyPos);















    QWidget* winLOG=new QWidget();
    plotLOG=new JKQtPlotter(false, winLOG, ds);
    plotLOG->set_doDrawing(false);
    plotLOG->setObjectName("plotLOG");
    plotLOG->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plotLOG/");

    layout=new QVBoxLayout(winLOG);
    layout->addWidget(plotLOG);
    winLOG->setLayout(layout);
    winLOG->resize(1000, 800);


    plotLOG->get_plotter()->addGraph(cx2, cy23, "data, $f(x)=5^{x/5}$", JKQTPlines);


    plotLOG->set_doDrawing(true);
    plotLOG->get_plotter()->zoomToFit(true, true);//, true);
    plotLOG->get_plotter()->set_keyAutosize(true);

    chklogX=new QCheckBox("logarithmic X-axis", winLOG);
    chklogY=new QCheckBox("logarithmic Y-axis", winLOG);
    connect(chklogX, SIGNAL(toggled(bool)), plotLOG->get_plotter()->getXAxis(), SLOT(set_logAxis(bool)));
    connect(chklogY, SIGNAL(toggled(bool)), plotLOG->get_plotter()->getYAxis(), SLOT(set_logAxis(bool)));
    layout->addWidget(chklogX);
    layout->addWidget(chklogY);


    QCheckBox* chkXMinorTicks=new QCheckBox("minor X-tick labels", winLOG);
    QCheckBox* chkYMinorTicks=new QCheckBox("minor Y-tick labels", winLOG);
    connect(chkXMinorTicks, SIGNAL(toggled(bool)), plotLOG->get_plotter()->getXAxis(), SLOT(set_minorTickLabelsEnabled(bool)));
    connect(chkYMinorTicks, SIGNAL(toggled(bool)), plotLOG->get_plotter()->getYAxis(), SLOT(set_minorTickLabelsEnabled(bool)));
    layout->addWidget(chkXMinorTicks);
    layout->addWidget(chkYMinorTicks);

    QCheckBox* chkFullNumberMinor=new QCheckBox("full minor numbers", winLOG);
    connect(chkFullNumberMinor, SIGNAL(toggled(bool)), plotLOG->get_plotter()->getXAxis(), SLOT(set_minorTickLabelFullNumber(bool)));
    connect(chkFullNumberMinor, SIGNAL(toggled(bool)), plotLOG->get_plotter()->getYAxis(), SLOT(set_minorTickLabelFullNumber(bool)));
    layout->addWidget(chkFullNumberMinor);

    QComboBox* cmbTickMode=new QComboBox(winLOG);
    cmbTickMode->addItem("JKQTPLTMLinOrLog");
    cmbTickMode->addItem("JKQTPLTMLin");
    cmbTickMode->addItem("JKQTPLTMPower");
    connect(cmbTickMode, SIGNAL(currentIndexChanged(int)), plotLOG->get_plotter()->getXAxis(), SLOT(set_tickMode(int)));
    layout->addWidget(cmbTickMode);

    cmbTickMode=new QComboBox(winLOG);
    cmbTickMode->addItem("JKQTPLTMLinOrLog");
    cmbTickMode->addItem("JKQTPLTMLin");
    cmbTickMode->addItem("JKQTPLTMPower");
    connect(cmbTickMode, SIGNAL(currentIndexChanged(int)), plotLOG->get_plotter()->getYAxis(), SLOT(set_tickMode(int)));
    layout->addWidget(cmbTickMode);
    cmbTickMode->setCurrentIndex(0);
    plotLOG->get_plotter()->getYAxis()->set_tickMode(JKQTPLTMLinOrPower);
    plotLOG->get_plotter()->getYAxis()->set_minorTicks(1);

    plotLOG->get_plotter()->getYAxis()->set_drawMode2(JKQTPCADMcomplete);
    plotLOG->get_plotter()->getXAxis()->set_drawMode2(JKQTPCADMcomplete);



    QSpinBox* spinMinTicks=new QSpinBox(winLOG);
    spinMinTicks->setRange(0,999);
    connect(spinMinTicks, SIGNAL(valueChanged(int)), plotLOG->get_plotter()->getXAxis(), SLOT(set_minorTicks(int)));
    connect(spinMinTicks, SIGNAL(valueChanged(int)), plotLOG->get_plotter()->getYAxis(), SLOT(set_minorTicks(int)));
    layout->addWidget(spinMinTicks);
    spinMinTicks->setValue(1);

    QDoubleSpinBox* spinLOGAngle=new QDoubleSpinBox(winLOG);
    spinLOGAngle->setRange(-90,90);
    spinLOGAngle->setPrefix(tr("X-axis label angle = "));
    spinLOGAngle->setSuffix(QLatin1String("°"));
    connect(spinLOGAngle, SIGNAL(valueChanged(double)), plotLOG->get_plotter()->getXAxis(), SLOT(set_tickLabelAngle(double)));
    layout->addWidget(spinLOGAngle);
    spinLOGAngle->setValue(0);

    spinLOGAngle=new QDoubleSpinBox(winLOG);
    spinLOGAngle->setRange(-90,90);
    spinLOGAngle->setPrefix(tr("Y-axis label angle = "));
    spinLOGAngle->setSuffix(QLatin1String("°"));
    connect(spinLOGAngle, SIGNAL(valueChanged(double)), plotLOG->get_plotter()->getYAxis(), SLOT(set_tickLabelAngle(double)));
    layout->addWidget(spinLOGAngle);
    spinLOGAngle->setValue(0);















    // plotter for images
    QWidget* wini=new QWidget(this);
    QVBoxLayout* layouti=new QVBoxLayout(this);
    wini->setLayout(layouti);
    wini->resize(1000, 800);
    plotImg=new JKQtPlotter(true, wini);
    plotImg->set_doDrawing(false);
    plotImg->get_plotter()->set_plotLabel(tr("\\textbf{image plot}"));
    plotImg->setObjectName("plotImg");
    plotImg->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_img/");

    plotImg->get_plotter()->set_maintainAspectRatio(true);
    plotImg->get_plotter()->set_aspectRatio(3.0/2.0);
    plotImg->get_plotter()->set_maintainAxisAspectRatio(true);
    plotImg->get_plotter()->set_axisAspectRatio(4.0);
    /*plotImg->get_plotter()->getXAxis()->set_linkedAxis(plotImg->get_plotter()->getYAxis());
    plotImg->get_plotter()->getXAxis()->set_changeWidthToAspectRatio(true);
    plotImg->get_plotter()->getXAxis()->set_aspectRatio(1.0);*/


    layouti->addWidget(plotImg);
    plotImg->setXY(0,30,0,20);
    plotImg->setAbsoluteXY(0,30,0,20);
    img1=QImage(":/lena.png");
    JKQTPImage* pimg1=new JKQTPImage(0,0,10,20, &img1, plotImg->get_plotter());
    pimg1->set_title("LENA");
    plotImg->get_plotter()->addGraph(pimg1);
    double* img2=(double*)calloc(100*100,sizeof(double));
    int32_t* img2M=(int32_t*)calloc(100*100,sizeof(int32_t));
    bool* bimg2=(bool*)calloc(100*100,sizeof(bool));
    for (int x=0; x<100; x++) {
        for (int y=0; y<100; y++) {
            img2[y*100+x]=sin(2*M_PI*sqrt(1.0+x*x+y*y)/20);
            img2M[y*100+x]=abs(y-x);
            bimg2[y*100+x]=static_cast<double>(rand())/static_cast<double>(RAND_MAX)>0.9;
        }
    }
    pimg2=new JKQTPMathImage(10,0,10,10, JKQTPMathImageBase::DoubleArray, img2, 100, 100, JKQTPMathImageGRAY, plotImg->get_plotter());
    pimg2->set_imageMin(-1);
    pimg2->set_imageMax(0.5);
    pimg2->set_imageName("grey image");
    pimg2->set_title("grey image");
    pimg2->set_autoImageRange(false);
    pimg2->set_colorBarTopVisible(true);
    pimg2->set_colorBarRightVisible(false);
    plotImg->get_plotter()->addGraph(pimg2);
    pimg3=new JKQTPMathImage(20,0,10,10, JKQTPMathImageBase::DoubleArray, img2, 100, 100, JKQTPMathImageMATLAB, plotImg->get_plotter());
    pimg3->set_colorBarTopVisible(true);
    pimg3->set_colorBarRightVisible(true);
    pimg3->set_imageMax(0.5);
    pimg3->set_rangeMaxFailAction(JKQTPMathImageGivenColor);
    pimg3->set_rangeMaxFailColor(QColor("salmon"));
    pimg3->set_imageMin(-0.3);
    pimg3->set_rangeMinFailAction(JKQTPMathImageTransparent);
    pimg3->set_autoImageRange(false);
    pimg3->set_imageName("I(x,y)=\\sin\\left(\\frac{2\\pi\\cdot\\sqrt{1+x^2+y^2}}{20}\\right)");
    pimg3->set_dataModifier(img2M, JKQTPMathImageBase::Int32Array);
    pimg3->set_modifierMode(JKQTPMathImageBase::ModifyNone);
    pimg3->set_title("colored image");
    plotImg->get_plotter()->addGraph(pimg3);

    QColor ty=QColor("blue");
    ty.setAlphaF(0.1);
    JKQTPOverlayImage* oimg1=new JKQTPOverlayImage(10,0,10,10, bimg2, 100, 100, QColor("blue"), plotImg->get_plotter());
    oimg1->set_title("overlay");
    oimg1->set_falseColor(ty);
    plotImg->get_plotter()->addGraph(oimg1);

    JKQTPOverlayImageEnhanced* oimg2=new JKQTPOverlayImageEnhanced(10,0,10,10, bimg2, 100, 100, QColor("red"), plotImg->get_plotter());
    oimg2->set_title("enhanced overlay");
    oimg2->set_drawAsRectangles(false);
    plotImg->get_plotter()->addGraph(oimg2);

    plotImg->set_doDrawing(true);
    plotImg->zoomToFit();

    JKQTPMathImageColorPaletteComboBox* cmbPal=new JKQTPMathImageColorPaletteComboBox(this);
    layouti->addWidget(cmbPal);
    connect(cmbPal, SIGNAL(currentIndexChanged(int)), this, SLOT(setImgPalette1(int)));
    cmbPal=new JKQTPMathImageColorPaletteComboBox(this);
    layouti->addWidget(cmbPal);
    connect(cmbPal, SIGNAL(currentIndexChanged(int)), this, SLOT(setImgPalette(int)));
    QComboBox* cmb=new QComboBox(this);
    layouti->addWidget(cmb);
    cmb->addItem("none");
    cmb->addItem("value");
    cmb->addItem("lightness");
    cmb->addItem("alpha");
    connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setImgModifier(int)));
    cmb->setCurrentIndex(1);






    // create third plotter with output of a geometric drawing
    QWidget* win1=new QWidget(this);
    QVBoxLayout* layout1=new QVBoxLayout(this);
    win1->setLayout(layout1);
    win1->resize(1000, 800);
    JKQtPlotter* plotGeo=new JKQtPlotter(true, win1);
    plotGeo->set_doDrawing(false);
    plotGeo->setObjectName("plotGeo");
    layout1->addWidget(plotGeo);
    plotGeo->setXY(0,1,0,1);
    plotGeo->get_plotter()->getXAxis()->set_axisLabel("$x$ [mm]");
    plotGeo->get_plotter()->getYAxis()->set_axisLabel("$y$ [mm]");
    /*plotGeo->get_plotter()->getXAxis()->set_linkedAxis(plotGeo->get_plotter()->getYAxis());
    plotGeo->get_plotter()->getXAxis()->set_changeWidthToAspectRatio(true);*/
    plotGeo->get_plotter()->set_maintainAspectRatio(true);
    plotGeo->get_plotter()->set_aspectRatio(1);
    plotGeo->get_plotter()->set_maintainAxisAspectRatio(true);
    plotGeo->get_plotter()->set_axisAspectRatio(1);


    JKQTPgeoLine* line1=new JKQTPgeoLine(plotGeo->get_plotter(), -1, -2, 2, 1.5);
    //plotGeo->get_plotter()->addGraph(line1);
    JKQTPgeoLine* line2=new JKQTPgeoLine(plotGeo->get_plotter(), 1, -5, 4, 3, QColor("blue"), 4);
    //plotGeo->get_plotter()->addGraph(line2);

    JKQTPgeoRectangle* rect1=new JKQTPgeoRectangle(plotGeo->get_plotter(), 1,1,2,2, QColor("blue"),2, Qt::SolidLine, QColor("lightblue"));
    rect1->set_alpha(30);
    //plotGeo->get_plotter()->addGraph(rect1);
    JKQTPgeoRectangle* rect2=new JKQTPgeoRectangle(plotGeo->get_plotter(), 1,1,2,2, QColor("red"),2);
    rect2->set_alpha(45);
    //plotGeo->get_plotter()->addGraph(rect2);
    JKQTPgeoRectangle* rect3=new JKQTPgeoRectangle(plotGeo->get_plotter(), 1,1,2,4, QColor("green"),2);
    rect3->set_alpha(-30);
    //plotGeo->get_plotter()->addGraph(rect3);
    JKQTPgeoEllipse* ell3=new JKQTPgeoEllipse(plotGeo->get_plotter(), 1,1,2,4, QColor("green"),2, Qt::SolidLine, QColor("lightgreen"), Qt::CrossPattern);
    ell3->set_alpha(-30);
    //plotGeo->get_plotter()->addGraph(ell3);

    QVector<QPointF> p;
    p.append(QPointF(4,1));
    p.append(QPointF(-11,5));
    p.append(QPointF(1,11));
    p.append(QPointF(0,-2));
    p.append(QPointF(M_PI,5.5));

    JKQTPgeoPolygon* poly=new JKQTPgeoPolygon(plotGeo->get_plotter(), p, QColor("black"), 1.5, Qt::SolidLine, QColor(128,128,0,128));
    //plotGeo->get_plotter()->addGraph(poly);
    JKQTPgeoLines* lines=new JKQTPgeoLines(plotGeo->get_plotter(), p, QColor(255,255,0), 3);
    //plotGeo->get_plotter()->addGraph(lines);

    JKQTPgeoEllipse* ell1=new JKQTPgeoEllipse(plotGeo->get_plotter(), 0,0,2,4, QColor("black"),1.5, Qt::DotLine);
    plotGeo->get_plotter()->addGraph(ell1);
    JKQTPgeoArc* arc=new JKQTPgeoArc(plotGeo->get_plotter(), 0,0,2,4,0, 135, QColor("blue"),3, Qt::SolidLine);
    //plotGeo->get_plotter()->addGraph(arc);
    JKQTPgeoPie* pie=new JKQTPgeoPie(plotGeo->get_plotter(), 0,0,2,4,35, 98, QColor("blue"),3, Qt::SolidLine, QColor(0,0,255,127));
    //plotGeo->get_plotter()->addGraph(pie);
    JKQTPgeoChord* chord=new JKQTPgeoChord(plotGeo->get_plotter(), 0,0,2,4,35, 98, QColor("blue"),3, Qt::SolidLine, QColor(0,0,255,127));
    //plotGeo->get_plotter()->addGraph(chord);

    JKQTPgeoInfiniteLine* iline=new JKQTPgeoInfiniteLine(plotGeo->get_plotter(), 0.5,1,1,1, QColor("blue"), 3);
    plotGeo->get_plotter()->addGraph(iline);
    JKQTPgeoInfiniteLine* iline1=new JKQTPgeoInfiniteLine(plotGeo->get_plotter(), 0.5,1,-1,0, QColor("red"), 3);
    plotGeo->get_plotter()->addGraph(iline1);
    JKQTPgeoInfiniteLine* iline2=new JKQTPgeoInfiniteLine(plotGeo->get_plotter(), 0.5,1,0,1, QColor("red"), 3);
    plotGeo->get_plotter()->addGraph(iline2);
    JKQTPgeoInfiniteLine* iline3=new JKQTPgeoInfiniteLine(plotGeo->get_plotter(), 0.5,1,-1,0.5, QColor("green"), 3);
    iline3->set_two_sided(true);
    plotGeo->get_plotter()->addGraph(iline3);

    JKQTPgeoText* text=new JKQTPgeoText(plotGeo->get_plotter(), -1, -1, "Test $f(x)=\\sqrt{\\sin(x^2)}$ \\textbf{bold}", 12, QColor("red"));
    plotGeo->get_plotter()->addGraph(text);

    plotGeo->set_doDrawing(true);
    plotGeo->get_plotter()->zoomToFit();
    plotGeo->get_plotter()->set_keyAutosize(true);


    QWidget* plotFunc=new QWidget(this);
    QVBoxLayout* plotFuncLayout=new QVBoxLayout(this);
    plotFunc->setLayout(plotFuncLayout);
    plotFuncPlt=new JKQtPlotter(true, win);
    plotFuncLayout->addWidget(plotFuncPlt);
    pfunc=new JKQTPxParsedFunctionLineGraph(plotFuncPlt->get_plotter());
    pfunc->set_function("x^2/10+sin(x*pi*10)");
    pfunc->set_errorFunction("x^2/20");
    pfunc->set_drawErrorPolygons(true);
    pfunc->set_style(Qt::DashLine);
    plotFuncPlt->addGraph(pfunc);
    plotFuncPlt->update_plot();

    pfuncErrorStyle=new JKQTPerrorPlotstyleComboBox(plotFunc);
    pfuncErrorStyle->setCurrentIndex(3);
    plotFuncLayout->addWidget(pfuncErrorStyle);
    connect(pfuncErrorStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(setPFuncStyle()));
    chkPFuncDrawSamples=new QCheckBox(tr("display samples"), plotFunc);
    plotFuncLayout->addWidget(chkPFuncDrawSamples);
    connect(chkPFuncDrawSamples, SIGNAL(toggled(bool)), this, SLOT(setPFuncStyle()));
    chkPFuncDrawLine=new QCheckBox(tr("draw line"), plotFunc);
    chkPFuncDrawLine->setChecked(true);
    plotFuncLayout->addWidget(chkPFuncDrawLine);
    connect(chkPFuncDrawLine, SIGNAL(toggled(bool)), this, SLOT(setPFuncStyle()));
    QSpinBox* spinPFunc=new QSpinBox(plotFunc);
    spinPFunc->setValue(pfunc->get_minSamples());
    plotFuncLayout->addWidget(new QLabel(tr("min samples:"), plotFunc));
    plotFuncLayout->addWidget(spinPFunc);
    connect(spinPFunc, SIGNAL(valueChanged(int)), this, SLOT(setPFuncMinPoint(int)));
    spinPFunc=new QSpinBox(plotFunc);
    spinPFunc->setValue(pfunc->get_maxRefinementDegree());
    plotFuncLayout->addWidget(new QLabel(tr("max refinement degree:"), plotFunc));
    plotFuncLayout->addWidget(spinPFunc);
    connect(spinPFunc, SIGNAL(valueChanged(int)), this, SLOT(setPFuncMaxRefine(int)));
    QComboBox* edtPFunc=new QComboBox(plotFunc);
    edtPFunc->addItem(pfunc->get_function());
    edtPFunc->addItem("x");
    edtPFunc->addItem("x^2");
    edtPFunc->addItem("x^2-x^3");
    edtPFunc->addItem("sin(x)");
    edtPFunc->setCurrentIndex(0);
    plotFuncLayout->addWidget(new QLabel(tr("function:"), plotFunc));
    plotFuncLayout->addWidget(edtPFunc);
    connect(edtPFunc, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPFunc(QString)));
    edtPFunc=new QComboBox(plotFunc);
    edtPFunc->addItem(pfunc->get_errorFunction());
    edtPFunc->addItem("");
    edtPFunc->addItem("x");
    edtPFunc->addItem("cos(x)");
    edtPFunc->addItem("1");
    edtPFunc->setCurrentIndex(0);
    plotFuncLayout->addWidget(new QLabel(tr("error function:"), plotFunc));
    plotFuncLayout->addWidget(edtPFunc);
    connect(edtPFunc, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPFuncE(QString)));
    QCheckBox* chkFLogX=new QCheckBox(tr("x-axis: log-scale"));
    chkFLogX->setChecked(false);
    connect(chkFLogX, SIGNAL(toggled(bool)), plotFuncPlt->getXAxis(), SLOT(set_logAxis(bool)));
    plotFuncLayout->addWidget(chkFLogX);
    QCheckBox* chkFLogY=new QCheckBox(tr("y-axis: log-scale"));
    chkFLogY->setChecked(false);
    connect(chkFLogY, SIGNAL(toggled(bool)), plotFuncPlt->getYAxis(), SLOT(set_logAxis(bool)));
    plotFuncLayout->addWidget(chkFLogY);


    #define NEMPTY 500

    JKQtPlotter* plotEmpty=new JKQtPlotter(true, win);
    JKQTPxyLineGraph* efunc=new JKQTPxyLineGraph(plotEmpty->get_plotter());
    double xef[NEMPTY], efy[NEMPTY];
    for (int i=0; i<NEMPTY; i++) {
        xef[i]=i;
        efy[i]=double(i%5)*1e-308;
    }
    efunc->set_xColumn(plotEmpty->getDatastore()->addCopiedColumn(xef, NEMPTY, "x"));
    efunc->set_yColumn(plotEmpty->getDatastore()->addCopiedColumn(efy, NEMPTY, "y"));
    plotEmpty->addGraph(efunc);
    plotEmpty->getYAxis()->set_logAxis(true);
    plotEmpty->zoomToFit();
    plotEmpty->setY(0,0);












    // plotter for images
    QWidget* winrgb=new QWidget(this);
    QVBoxLayout* layoutrgb=new QVBoxLayout(this);
    QFormLayout* frm=new QFormLayout(nullptr);
    winrgb->setLayout(layoutrgb);
    winrgb->resize(1000, 800);
    plotImgRGB=new JKQtPlotter(true, winrgb);
    plotImgRGB->set_doDrawing(false);
    plotImgRGB->get_plotter()->set_plotLabel(tr("\\textbf{RGB image plot}"));
    plotImgRGB->setObjectName("plotImgRGB");
    plotImgRGB->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_img_rgb/");

    /*plotImgRGB->get_plotter()->set_maintainAspectRatio(true);
    plotImgRGB->get_plotter()->set_aspectRatio(1.0);
    plotImgRGB->get_plotter()->set_maintainAxisAspectRatio(true);
    plotImgRGB->get_plotter()->set_axisAspectRatio(1.0);*/

    QCheckBox* chkColorbars=new QCheckBox(tr("color bars share space"), winrgb);
    chkColorbars->setChecked(true);
    connect(chkColorbars, SIGNAL(toggled(bool)), this, SLOT(setRGBColorBars(bool)));
    layoutrgb->addWidget(chkColorbars);
    layoutrgb->addLayout(frm);
    layoutrgb->addWidget(plotImgRGB);
    plotImgRGB->setXY(0,100,0,100);
    plotImgRGB->setAbsoluteXY(0,100,0,100);

    rgb_rimg2=(double*)calloc(100*100,sizeof(double));
    rgb_bimg2=(double*)calloc(100*100,sizeof(double));
    rgb_gimg2=(double*)calloc(100*100,sizeof(double));
    rgb_aimg2=(double*)calloc(100*100,sizeof(double));
    for (int x=0; x<100; x++) {
        for (int y=0; y<100; y++) {
            rgb_rimg2[y*100+x]=sin(2*M_PI*sqrt(1.0+x*x+y*y)/20);
            rgb_bimg2[y*100+x]=sin(2*M_PI*sqrt(1.0+x*x+y*y)/30);
            rgb_gimg2[y*100+x]=cos(2*M_PI*sqrt(1.0+x*x+y*y)/30);
            rgb_aimg2[y*100+x]=double(x)/100.0;
        }
    }
    rgbimg=new JKQTPRGBMathImage(0,0,10,10, JKQTPMathImageBase::DoubleArray, rgb_rimg2, 100, 100, plotImgRGB->get_plotter());
    rgbimg->set_imageMin(-1);
    rgbimg->set_imageMax(0.5);
    rgbimg->set_imageMinB(-1);
    rgbimg->set_imageMaxB(0.5);
    rgbimg->set_imageName("R image");
    rgbimg->set_imageNameG("G image");
    rgbimg->set_imageNameB("B image");
    rgbimg->set_data(rgb_rimg2, rgb_gimg2, rgb_bimg2, 100, 100, JKQTPMathImageBase::DoubleArray);
    rgbimg->set_dataModifier(rgb_aimg2, JKQTPMathImageBase::DoubleArray);
    rgbimg->set_modifierMode(JKQTPMathImageBase::ModifyNone);
    plotImgRGB->get_plotter()->addGraph(rgbimg);
    rgbimg->set_autoImageRange(false);
    rgbimg->set_colorBarTopVisible(true);
    rgbimg->set_colorBarRightVisible(true);
    rgbimg->set_colorbarsSideBySide(chkColorbars->isChecked());
    rgbimg2=new JKQTPColumnRGBMathImage(10,0,10,10,100,100, plotImgRGB->get_plotter());
    rgbimg2->set_imageMin(-1);
    rgbimg2->set_imageMax(0.5);
    rgbimg2->set_imageMinB(-1);
    rgbimg2->set_imageMaxB(0.5);
    rgbimg2->set_imageName("R image");
    rgbimg2->set_imageNameG("G image");
    rgbimg2->set_imageNameB("B image");
    rgbimg2->set_Nx(100);
    rgbimg2->set_Ny(100);
    rgbimg2->set_imageRColumn(rgb_colr=plotImgRGB->getDatastore()->addCopiedColumn(rgb_bimg2, 100*100, "R"));
    rgbimg2->set_imageGColumn(rgb_colg=plotImgRGB->getDatastore()->addCopiedColumn(rgb_gimg2, 100*100, "G"));
    rgbimg2->set_imageBColumn(rgb_colb=plotImgRGB->getDatastore()->addCopiedColumn(rgb_rimg2, 100*100, "B"));
    rgbimg2->set_modifierColumn(rgb_cola=plotImgRGB->getDatastore()->addCopiedColumn(rgb_aimg2, 100*100, "alpha"));
    rgbimg2->set_modifierMode(JKQTPMathImageBase::ModifyNone);
    //rgbimg2->set_data(rgb_rimg2, rgb_gimg2, rgb_bimg2, 100, 100, JKQTPMathImageBase::DoubleArray);
    plotImgRGB->get_plotter()->addGraph(rgbimg2);
    rgbimg2->set_autoImageRange(true);
    rgbimg2->set_colorBarTopVisible(true);
    rgbimg2->set_colorBarRightVisible(true);
    rgbimg2->set_colorbarsSideBySide(chkColorbars->isChecked());
    plotImgRGB->setAbsoluteXY(0,20,0,10);
    plotImgRGB->set_doDrawing(true);
    //plotImgRGB->zoomToFit();
    plotImgRGB->setXY(0,20,0,10);

    cmb=new QComboBox(winrgb);
    frm->addRow(tr("mode 1:"), cmb);
    cmb->addItem("RGB");
    cmb->addItem("HSV");
    cmb->addItem("HSL");
    cmb->addItem("CMY");
    cmb->setCurrentIndex(0);
    connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setRGBMode1(int)));

    cmb=new QComboBox(winrgb);
    frm->addRow(tr("mode 2:"), cmb);
    cmb->addItem("RGB");
    cmb->addItem("HSV");
    cmb->addItem("HSL");
    cmb->addItem("CMY");
    connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setRGBMode2(int)));
    cmb->setCurrentIndex(1);

    cmb=new QComboBox(winrgb);
    frm->addRow(tr("modifier channel mode:"), cmb);
    cmb->addItem("none");
    cmb->addItem("value");
    cmb->addItem("lightness");
    cmb->addItem("alpha");
    connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setRGBModifier(int)));
    cmb->setCurrentIndex(0);

    QHBoxLayout* hblrgb=new QHBoxLayout();
    frm->addRow(tr("use channels:"), hblrgb);

    chkRGBUseR=new QCheckBox(tr("red"), winrgb);
    hblrgb->addWidget(chkRGBUseR);
    chkRGBUseR->setChecked(true);
    connect(chkRGBUseR, SIGNAL(toggled(bool)), this, SLOT(updateRGBChannels()));

    chkRGBUseG=new QCheckBox(tr("green"), winrgb);
    hblrgb->addWidget(chkRGBUseG);
    chkRGBUseG->setChecked(true);
    connect(chkRGBUseG, SIGNAL(toggled(bool)), this, SLOT(updateRGBChannels()));

    chkRGBUseB=new QCheckBox(tr("blue"), winrgb);
    hblrgb->addWidget(chkRGBUseB);
    chkRGBUseB->setChecked(true);
    connect(chkRGBUseB, SIGNAL(toggled(bool)), this, SLOT(updateRGBChannels()));

    chkRGBUseA=new QCheckBox(tr("alpha"), winrgb);
    hblrgb->addWidget(chkRGBUseA);
    chkRGBUseB->setChecked(false);
    connect(chkRGBUseB, SIGNAL(toggled(bool)), this, SLOT(updateRGBChannels()));














    QWidget* winpeaks=new QWidget(this);
    QVBoxLayout* layoutpeaks=new QVBoxLayout(this);
    winpeaks->setLayout(layoutpeaks);
    winpeaks->resize(1000, 800);
    plotPeaks=new JKQtPlotter(true, winpeaks);
    plotPeaks->set_doDrawing(false);
    plotPeaks->get_plotter()->set_plotLabel(tr("\\textbf{peaks stream plot}"));
    plotPeaks->setObjectName("plotPeaks");
    plotPeaks->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");

    QCheckBox* chkDrawBaseline=new QCheckBox(tr("draw baseline"), this);
    chkDrawBaseline->setChecked(true);
    connect(chkDrawBaseline, SIGNAL(toggled(bool)), this, SLOT(setDrawBaseline(bool)));
    layoutpeaks->addWidget(chkDrawBaseline);
    QCheckBox* chkYPeaks=new QCheckBox(tr("y peaks"), this);
    chkYPeaks->setChecked(true);
    connect(chkYPeaks, SIGNAL(toggled(bool)), this, SLOT(setYPeaks(bool)));
    layoutpeaks->addWidget(chkYPeaks);
    layoutpeaks->addWidget(plotPeaks);

    QVector<double> photons1, photons2;
    for (int i=0; i<1000; i++) {
        double x=double(i)/1000.0;
        const double p=0.33*fabs(sin(x*4.0*M_PI))*fabs(sin(x*4.0*M_PI));
        if (static_cast<double>(rand())/static_cast<double>(RAND_MAX)<=p) photons1<<x;
        if (static_cast<double>(rand())/static_cast<double>(RAND_MAX)<=p) photons2<<x;
    }
    int phot1=plotPeaks->getDatastore()->addCopiedColumn(photons1.data(), photons1.size(), "photons 1");
    int phot2=plotPeaks->getDatastore()->addCopiedColumn(photons2.data(), photons2.size(), "photons 2");

    graphPeakStream1=new JKQTPPeakStreamGraph(phot1, 0.05, 1, QColor("darkgreen"), plotPeaks->get_plotter());
    graphPeakStream1->set_title("channel 1");
    plotPeaks->get_plotter()->addGraph(graphPeakStream1);
    graphPeakStream2=new JKQTPPeakStreamGraph(phot2, -0.05, -1, QColor("darkred"), plotPeaks->get_plotter());
    graphPeakStream2->set_title("channel 2");
    plotPeaks->get_plotter()->addGraph(graphPeakStream2);

    plotPeaks->set_doDrawing(true);
    plotPeaks->zoomToFit();





    // contour plot
    QWidget* contourWid=new QWidget(this);
    QHBoxLayout* layoutContour=new QHBoxLayout(this);
    contourWid->setLayout(layoutContour);
    JKQtPlotter* plotContour=new JKQtPlotter(true, contourWid);
    plotContour->set_doDrawing(false);
    plotContour->get_plotter()->set_plotLabel(tr("\\textbf{hist contour plot}"));
    plotContour->setObjectName("contour");
    plotContour->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "contour/");
    layoutContour->addWidget(plotContour);

    JKQtPlotter* plotDensity=new JKQtPlotter(true, contourWid);
    plotContour->set_doDrawing(false);
    plotContour->get_plotter()->set_plotLabel(tr("\\textbf{2D histogram plot with contours}"));
    plotDensity->get_plotter()->set_plotLabel(tr("\\textbf{2D histogram}"));
    plotContour->setObjectName("density");
    plotContour->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "density/");
    layoutContour->addWidget(plotDensity);

    QVector<double> histAlex;
    uint histAlexNx,histAlexNy;

    // choose the test data
    histAlex.clear();
    histAlex.resize(5);
    histAlex.append(15.0);
    histAlex.append(5);
    histAlex.append(0);
    histAlex.append(0);
    histAlex.append(5);
    histAlex.append(15);
    histAlex.append(6);
    histAlex.append(6);
    histAlex.resize(16);

    histAlexNx=(uint)sqrt(histAlex.size());
    histAlexNy=histAlexNx;

    ds=plotDensity->get_plotter()->getDatastore();
    plotContour->get_plotter()->useExternalDatastore(ds);
    // empty datastore and grid
    plotDensity->get_plotter()->clearGraphs(true);
    ds->clear();
    plotDensity->setGrid(false);
    plotDensity->get_plotter()->getXAxis()->set_axisLabel("X");
    plotDensity->get_plotter()->getYAxis()->set_axisLabel("Y");

    size_t colHist2D=ds->addCopiedImageAsColumn(histAlex.data(), histAlexNx, histAlexNy, tr("2Dhist"));
//    size_t colContour=plotContour->getDatastore()->addCopiedImageAsColumnTranspose(histAlex.data(), histAlexNx, histAlexNy, tr("2Dhist"));

    JKQTPColumnMathImage* densityplot=new JKQTPColumnMathImage(plotDensity->get_plotter());
    densityplot->set_x(-0.1);
    densityplot->set_y(-0.1);
    densityplot->set_width(1.1);
    densityplot->set_height(1.1);
    densityplot->set_Nx(histAlexNx);
    densityplot->set_Ny(histAlexNy);
    densityplot->set_palette(JKQTPMathImageINVERTED_OCEAN);
    densityplot->set_imageColumn(colHist2D);
    densityplot->set_infColor(Qt::transparent);
    densityplot->set_nanColor(Qt::transparent);
    densityplot->set_title("density plot");
    plotDensity->addGraph(densityplot);

    densityplot=new JKQTPColumnMathImage(plotContour->get_plotter());
    densityplot->set_x(-0.1);
    densityplot->set_y(-0.1);
    densityplot->set_width(1.1);
    densityplot->set_height(1.1);
    densityplot->set_Nx(histAlexNx);
    densityplot->set_Ny(histAlexNy);
    densityplot->set_palette(JKQTPMathImageINVERTED_OCEAN);
    densityplot->set_imageColumn(colHist2D);
    densityplot->set_title("density plot");
    plotContour->addGraph(densityplot);
    plotContour->setGrid(false);
    plotDensity->zoom(-0.1,1.1,-0.1,1.1);

    QList<double> levels;
//    levels<<5<<10<<25; //    levels<<5.1<<10.1;
    levels<<4<<5<<9<<14;
    JKQTPContour* cp=new JKQTPContour(plotContour->get_plotter());
    cp->set_x(-0.1);
    cp->set_y(-0.1);
    cp->set_width(1.1);
    cp->set_height(1.1);
    cp->set_Nx(histAlexNx);
    cp->set_Ny(histAlexNy);
    cp->set_imageColumn(colHist2D);
//    cp->set_palette(JKQTPMathImageINVERTED_OCEAN);
    cp->set_contourLevels(levels);
    cp->set_title("contour plot");
    plotContour->addGraph(cp);
    plotContour->zoom(-0.1,1.1,-0.1,1.1);

    plotContour->set_doDrawing(true);
    plotContour->update_plot();









    QWidget* contourPSP=new QWidget(this);
    QVBoxLayout* layoutPSP=new QVBoxLayout(this);

    contourPSP->setLayout(layoutPSP);
    contourPSP->resize(1000, 800);
    plotPSP=new JKQtPlotter(true, contourPSP);
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



    int pspx=plotPSP->getDatastore()->addCopiedColumn(dataX.data(), dataX.size(), "x");
    int pspy=plotPSP->getDatastore()->addCopiedColumn(dataY.data(), dataY.size(), "y");
    int pspy1=plotPSP->getDatastore()->addCopiedColumn(dataY1.data(), dataY1.size(), "y1");
    int pspy2=plotPSP->getDatastore()->addCopiedColumn(dataY2.data(), dataY2.size(), "y2");
    int pspy3=plotPSP->getDatastore()->addCopiedColumn(dataY2.data(), dataY3.size(), "y3");
    int pspc=plotPSP->getDatastore()->addCopiedColumn(dataC.data(), dataC.size(), "color");
    int psps=plotPSP->getDatastore()->addCopiedColumn(dataS.data(), dataS.size(), "size");
    int pspsy=plotPSP->getDatastore()->addCopiedColumn(dataSy.data(), dataSy.size(), "symbol");

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









    QWidget* barchartW=new QWidget(this);
    QVBoxLayout* barchartLayout=new QVBoxLayout(this);
    barchartW->setLayout(barchartLayout);
    barchartW->resize(1000, 800);
    plotBarchart=new JKQtPlotter(true, barchartW);
    plotBarchart->set_doDrawing(false);
    plotBarchart->get_plotter()->set_plotLabel(tr("\\textbf{bar charts}"));
    plotBarchart->setObjectName("plotBarchart");
    plotBarchart->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");
    plotBarchart->get_plotter()->getXAxis()->set_labelType(JKQTPCALTdate);
    plotBarchart->get_plotter()->getXAxis()->set_tickDateFormat("dd. MMM yyyy");

    barchartLayout->addWidget(plotBarchart);

    QVector<double> dataBCX, dataBCXD, dataBCY1, dataBCY2, dataBCYE1, dataBCYE2;
    for (int i=0; i<8; i++) {
        double x=double(i);
        dataBCX<<x;
        QDateTime dt;
        dt.setDate(QDate(2012,1+i,1));
        //qDebug()<<dt<<dt.toMSecsSinceEpoch();
        dataBCXD<<double(dt.toMSecsSinceEpoch());
        dataBCY1<<1.0+sin(x*M_PI/6.0);
        dataBCY2<<2.0+cos(x*M_PI/3.0);
        dataBCYE1<<dataBCY1.last()*0.1;
        dataBCYE2<<dataBCY2.last()*0.05;
    }
    int bcpx=plotBarchart->getDatastore()->addCopiedColumn(dataBCX.data(), dataBCX.size(), "x");
    int bcpxd=plotBarchart->getDatastore()->addCopiedColumn(dataBCXD.data(), dataBCXD.size(), "x_date");
    int bcpy1=plotBarchart->getDatastore()->addCopiedColumn(dataBCY1.data(), dataBCY1.size(), "y1");
    int bcpye1=plotBarchart->getDatastore()->addCopiedColumn(dataBCYE1.data(), dataBCYE1.size(), "ye1");
    int bcpy2=plotBarchart->getDatastore()->addCopiedColumn(dataBCY2.data(), dataBCY2.size(), "y2");
    int bcpye2=plotBarchart->getDatastore()->addCopiedColumn(dataBCYE2.data(), dataBCYE2.size(), "ye2");

    JKQTPbarHorizontalGraph* plteBar1=new JKQTPbarHorizontalGraph(plotBarchart->get_plotter());
    plteBar1->set_title(tr("bars 1"));
    plteBar1->set_xColumn(bcpxd);
    plteBar1->set_yColumn(bcpy1);
    plteBar1->set_yErrorColumn(bcpye1);
    plteBar1->set_width(0.45);
    plteBar1->set_shift(-0.25);
    plotBarchart->get_plotter()->addGraph(plteBar1);

    JKQTPbarHorizontalGraph* plteBar2=new JKQTPbarHorizontalGraph(plotBarchart->get_plotter());
    plteBar2->set_xColumn(bcpxd);
    plteBar2->set_yColumn(bcpy2);
    plteBar2->set_yErrorColumn(bcpye2);
    plteBar2->set_title(tr("bars 2"));
    plteBar2->set_width(0.45);
    plteBar2->set_shift(0.25);
    plteBar2->set_baseline(0.5);
    plteBar2->set_xErrorSymmetric(false);
    plotBarchart->get_plotter()->addGraph(plteBar2);

    plteBar2->autoscaleBarWidthAndShift();
    plotBarchart->set_doDrawing(true);
    plotBarchart->zoomToFit();




    plotBarchart2=new JKQtPlotter(false, barchartW, plotBarchart->getDatastore());
    plotBarchart2->set_doDrawing(false);
    plotBarchart2->get_plotter()->set_plotLabel(tr("\\textbf{bar charts}"));
    plotBarchart2->setObjectName("plotBarchart2");
    plotBarchart2->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");

    barchartLayout->addWidget(plotBarchart2);

    plteBar1=new JKQTPbarVerticalGraph(plotBarchart2->get_plotter());
    plteBar1->set_title(tr("bars 1"));
    plteBar1->set_yColumn(bcpx);
    plteBar1->set_xColumn(bcpy1);
    plteBar1->set_xErrorColumn(bcpye1);
    plteBar1->set_width(0.45);
    plteBar1->set_shift(-0.25);
    plotBarchart2->get_plotter()->addGraph(plteBar1);

    plteBar2=new JKQTPbarVerticalGraph(plotBarchart2->get_plotter());
    plteBar2->set_yColumn(bcpx);
    plteBar2->set_xColumn(bcpy2);
    plteBar2->set_xErrorColumn(bcpye2);
    plteBar2->set_title(tr("bars 2"));
    plteBar2->set_width(0.45);
    plteBar2->set_shift(0.25);
    plteBar2->set_xErrorSymmetric(false);
    plotBarchart2->get_plotter()->addGraph(plteBar2);

    plteBar2->autoscaleBarWidthAndShift(0.9, 1);
    plotBarchart2->set_doDrawing(true);
    plotBarchart2->zoomToFit();

    chkBarLog=new QCheckBox(tr("log-log plots"), this);
    chkBarLog->setChecked(false);
    barchartLayout->addWidget(chkBarLog);
    connect(chkBarLog, SIGNAL(toggled(bool)), this, SLOT(setBarchartLogLog(bool)));

    spinBarLabelAngel=new QSpinBox(this);
    spinBarLabelAngel->setPrefix(tr("rotation angel:  "));
    spinBarLabelAngel->setSuffix(QLatin1String("°"));
    spinBarLabelAngel->setRange(-180,180);
    spinBarLabelAngel->setValue(0);
    spinBarLabelAngel->setSingleStep(15);
    barchartLayout->addWidget(spinBarLabelAngel);
    connect(spinBarLabelAngel, SIGNAL(valueChanged(int)), this, SLOT(setBarchartAngele(int)));

    spinBarLabelAngel2=new QSpinBox(this);
    spinBarLabelAngel2->setPrefix(tr("rotation angel:  "));
    spinBarLabelAngel2->setSuffix(QLatin1String("°"));
    spinBarLabelAngel2->setRange(-180,180);
    spinBarLabelAngel2->setValue(0);
    spinBarLabelAngel2->setSingleStep(15);
    barchartLayout->addWidget(spinBarLabelAngel2);
    connect(spinBarLabelAngel2, SIGNAL(valueChanged(int)), this, SLOT(setBarchartAngele2(int)));

    // draw to screen
    addTab(win, QString("graphs"));
    addTab(winLOG, QString("log graphs"));
    addTab(wini, QString("images"));
    addTab(winrgb, QString("RGB images"));
    addTab(win1, QString("geometry"));
    addTab(plotFunc, QString("functionplots"));
    addTab(plotEmpty, QString("empty plot"));
    addTab(winpeaks, QString("peaks plot"));
    addTab(contourWid, QString("contour plot"));
    addTab(contourPSP, QString("parametrized scatter plot"));
    addTab(barchartW, QString("barcharts"));
    setCurrentWidget(winLOG);
    QCheckBox* chkAspect=new QCheckBox("keep aspect ratio", win1);
    chkAspect->setChecked(plotGeo->get_plotter()->get_maintainAxisAspectRatio());
    win1->connect(chkAspect, SIGNAL(toggled(bool)), plotGeo->get_plotter(), SLOT(set_maintainAspectRatio(bool)));
    win1->connect(chkAspect, SIGNAL(toggled(bool)), plotGeo->get_plotter(), SLOT(set_maintainAxisAspectRatio(bool)));
    layout1->addWidget(chkAspect);


    showMaximized();
    QTimer::singleShot(50, this, SLOT(moveovl()));

}

void TestMain::moveovl() {
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

void TestMain::setRGBColorBars(bool checked)
{
    rgbimg->set_colorbarsSideBySide(checked);
    rgbimg2->set_colorbarsSideBySide(checked);
    plotImgRGB->update_plot();
}

void TestMain::setDrawBaseline(bool checked)
{
    graphPeakStream1->set_drawBaseline(checked);
    graphPeakStream2->set_drawBaseline(checked);
    plotPeaks->update_plot();
}

void TestMain::setYPeaks(bool checked)
{
    graphPeakStream1->set_yPeaks(checked);
    graphPeakStream2->set_yPeaks(checked);
    plotPeaks->update_plot();
}

void TestMain::setPFuncStyle()
{
    pfunc->set_drawErrorLines(false);
    pfunc->set_drawErrorPolygons(false);
    if (pfuncErrorStyle->getErrorStyle()==JKQTPerrorLines) pfunc->set_drawErrorLines(true);
    if (pfuncErrorStyle->getErrorStyle()==JKQTPerrorPolygons) pfunc->set_drawErrorPolygons(true);
    pfunc->set_drawLine(chkPFuncDrawLine->isChecked());
    pfunc->set_displaySamplePoints(chkPFuncDrawSamples->isChecked());
    plotFuncPlt->update_plot();
}

void TestMain::setPFuncMinPoint(int value)
{
    pfunc->set_minSamples(value);
    plotFuncPlt->update_plot();
}

void TestMain::setPFuncMaxRefine(int value)
{
    pfunc->set_maxRefinementDegree(value);
    plotFuncPlt->update_plot();
}

void TestMain::setPFunc(const QString &expression)
{
    pfunc->set_function(expression);
    plotFuncPlt->update_plot();
}

void TestMain::setPFuncE(const QString &expression)
{
    pfunc->set_errorFunction(expression);
    plotFuncPlt->update_plot();
}

void TestMain::setRGBMode1(int mode)
{
    rgbimg->set_rgbMode((JKQTPRGBMathImageRGBMode)mode);
    plotImgRGB->update_plot();
}

void TestMain::setRGBMode2(int mode)
{
    rgbimg2->set_rgbMode((JKQTPRGBMathImageRGBMode)mode);
    plotImgRGB->update_plot();
}

void TestMain::updateRGBChannels()
{
    if (chkRGBUseR->isChecked()) {
        rgbimg->set_data(rgb_rimg2, 100, 100);
        rgbimg2->set_imageRColumn(rgb_colr);
    } else {
        rgbimg->set_data(nullptr, 100, 100);
        rgbimg2->set_imageRColumn(-1);
    }
    if (chkRGBUseG->isChecked()) {
        rgbimg->set_dataG(rgb_gimg2);
        rgbimg2->set_imageGColumn(rgb_colg);
    } else {
        rgbimg->set_dataG(nullptr);
        rgbimg2->set_imageGColumn(-1);
    }
    if (chkRGBUseB->isChecked()) {
        rgbimg->set_dataB(rgb_bimg2);
        rgbimg2->set_imageBColumn(rgb_colb);
    } else {
        rgbimg->set_dataB(nullptr);
        rgbimg2->set_imageBColumn(-1);
    }
    plotImgRGB->update_plot();
}

void TestMain::setRGBModifier(int item)
{
    rgbimg->set_modifierMode(JKQTPMathImage::ModifierMode(item));
    rgbimg2->set_modifierMode(JKQTPMathImage::ModifierMode(item));
    plotImgRGB->update_plot();
}

void TestMain::setImgPalette(int index)
{
    pimg3->set_palette(index);
    plotImg->update_plot();
}

void TestMain::setImgPalette1(int index)
{
    pimg2->set_palette(index);
    plotImg->update_plot();
}

void TestMain::setImgModifier(int index)
{
    pimg3->set_modifierMode(JKQTPMathImage::ModifierMode(index));
    plotImg->update_plot();
}

void TestMain::setErrorStyle(int index)
{
    QComboBox* cmb=qobject_cast<QComboBox*>(sender());
    if (cmb) {
        JKQTPxyGraphErrors* exy=dynamic_cast<JKQTPxyGraphErrors*>(plteErrors);
        JKQTPxGraphErrors* ex=dynamic_cast<JKQTPxGraphErrors*>(plteErrors);
        JKQTPyGraphErrors* ey=dynamic_cast<JKQTPyGraphErrors*>(plteErrors);
        if (exy) {
            exy->set_yErrorStyle(String2JKQTPerrorPlotstyle(cmb->itemData(cmb->currentIndex()).toString()));
        }
        if (ey) {
            ey->set_yErrorStyle(String2JKQTPerrorPlotstyle(cmb->itemData(cmb->currentIndex()).toString()));
        }
        plot->update_plot();
    }
}

void TestMain::setESSymbol(int index)
{
    QComboBox* cmb=qobject_cast<QComboBox*>(sender());
    if (cmb) {
        JKQTPxyLineGraph* exy=dynamic_cast<JKQTPxyLineGraph*>(plteSymbols);
        if (exy) {
            exy->set_symbol(String2JKQTPgraphSymbols(cmb->itemData(cmb->currentIndex()).toString()));
        }

        plot->update_plot();
    }
}

void TestMain::setBarchartLogLog(bool checked)
{
    plotBarchart->getXAxis()->set_logAxis(checked);
    plotBarchart->getYAxis()->set_logAxis(checked);
    plotBarchart->zoomToFit();
    plotBarchart2->getXAxis()->set_logAxis(checked);
    plotBarchart2->getYAxis()->set_logAxis(checked);
    plotBarchart2->zoomToFit();
}

void TestMain::setBarchartAngele(int angle)
{
    plotBarchart->getXAxis()->set_tickLabelAngle(angle);
    plotBarchart->getXAxis()->set_drawMode2(JKQTPCADMcomplete);
}

void TestMain::setBarchartAngele2(int angle)
{
    plotBarchart->getYAxis()->set_tickLabelAngle(angle);
    plotBarchart->getYAxis()->set_drawMode2(JKQTPCADMcomplete);
}

void TestMain::setPSPSymmErrors(bool checked)
{
     pltePSPCol->set_yErrorSymmetric(checked);
     plotPSP->update_plot();
}

void TestMain::setSortOrder2(int index)
{
    if (index==0) pltePlot2->set_sortData(JKQTPxyLineGraph::Unsorted);
    if (index==1) pltePlot2->set_sortData(JKQTPxyLineGraph::SortedX);
    if (index==2) pltePlot2->set_sortData(JKQTPxyLineGraph::SortedY);
    plotBot->update_plot();
}

void TestMain::setKeyLayout2(JKQTPkeyLayout layout)
{
    plot->get_plotter()->set_keyLayout(layout);
    plotBot->get_plotter()->set_keyLayout(layout);
    plotBot2->get_plotter()->set_keyLayout(layout);
    plot->update_plot();
    plotBot->update_plot();
    plotBot2->update_plot();
}

void TestMain::setKeyPos2(JKQTPkeyPosition layout)
{
    plot->get_plotter()->set_keyPosition(layout);
    plotBot->get_plotter()->set_keyPosition(layout);
    plotBot2->get_plotter()->set_keyPosition(layout);
    plot->update_plot();
    plotBot->update_plot();
    plotBot2->update_plot();
}
