#include "TestWidgetFunctionPlots.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>






TestWidgetFunctionPlots::TestWidgetFunctionPlots(QWidget *parent) :
    QWidget(parent)
{


    QVBoxLayout* plotFuncLayout=new QVBoxLayout(this);
    setLayout(plotFuncLayout);
    plotFuncPlt=new JKQTPlotter(true, this);
    plotFuncLayout->addWidget(plotFuncPlt);
    pfunc=new JKQTPXParsedFunctionLineGraph(plotFuncPlt->getPlotter());
    pfunc->setFunction("x^2/10+sin(x*pi*10)");
    pfunc->setErrorFunction("x^2/20");
    pfunc->setDrawErrorPolygons(true);
    pfunc->setStyle(Qt::DashLine);
    plotFuncPlt->addGraph(pfunc);
    plotFuncPlt->redrawPlot();

    pfuncErrorStyle=new JKQTPErrorPlotstyleComboBox(this);
    pfuncErrorStyle->setCurrentIndex(3);
    plotFuncLayout->addWidget(pfuncErrorStyle);
    connect(pfuncErrorStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(setPFuncStyle()));
    chkPFuncDrawSamples=new QCheckBox(tr("display samples"), this);
    plotFuncLayout->addWidget(chkPFuncDrawSamples);
    connect(chkPFuncDrawSamples, SIGNAL(toggled(bool)), this, SLOT(setPFuncStyle()));
    chkPFuncDrawLine=new QCheckBox(tr("draw line"), this);
    chkPFuncDrawLine->setChecked(true);
    plotFuncLayout->addWidget(chkPFuncDrawLine);
    connect(chkPFuncDrawLine, SIGNAL(toggled(bool)), this, SLOT(setPFuncStyle()));
    QSpinBox* spinPFunc=new QSpinBox(this);
    spinPFunc->setValue(pfunc->getMinSamples());
    plotFuncLayout->addWidget(new QLabel(tr("min samples:"), this));
    plotFuncLayout->addWidget(spinPFunc);
    connect(spinPFunc, SIGNAL(valueChanged(int)), this, SLOT(setPFuncMinPoint(int)));
    spinPFunc=new QSpinBox(this);
    spinPFunc->setValue(pfunc->getMaxRefinementDegree());
    plotFuncLayout->addWidget(new QLabel(tr("max refinement degree:"), this));
    plotFuncLayout->addWidget(spinPFunc);
    connect(spinPFunc, SIGNAL(valueChanged(int)), this, SLOT(setPFuncMaxRefine(int)));
    QComboBox* edtPFunc=new QComboBox(this);
    edtPFunc->addItem(pfunc->getFunction());
    edtPFunc->addItem("x");
    edtPFunc->addItem("x^2");
    edtPFunc->addItem("x^2-x^3");
    edtPFunc->addItem("sin(x)");
    edtPFunc->setCurrentIndex(0);
    plotFuncLayout->addWidget(new QLabel(tr("function:"), this));
    plotFuncLayout->addWidget(edtPFunc);
    connect(edtPFunc, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPFunc(QString)));
    edtPFunc=new QComboBox(this);
    edtPFunc->addItem(pfunc->getErrorFunction());
    edtPFunc->addItem("");
    edtPFunc->addItem("x");
    edtPFunc->addItem("cos(x)");
    edtPFunc->addItem("1");
    edtPFunc->setCurrentIndex(0);
    plotFuncLayout->addWidget(new QLabel(tr("error function:"), this));
    plotFuncLayout->addWidget(edtPFunc);
    connect(edtPFunc, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPFuncE(QString)));
    QCheckBox* chkFLogX=new QCheckBox(tr("x-axis: log-scale"));
    chkFLogX->setChecked(false);
    connect(chkFLogX, SIGNAL(toggled(bool)), plotFuncPlt->getXAxis(), SLOT(setLogAxis(bool)));
    plotFuncLayout->addWidget(chkFLogX);
    QCheckBox* chkFLogY=new QCheckBox(tr("y-axis: log-scale"));
    chkFLogY->setChecked(false);
    connect(chkFLogY, SIGNAL(toggled(bool)), plotFuncPlt->getYAxis(), SLOT(setLogAxis(bool)));
    plotFuncLayout->addWidget(chkFLogY);


}


void TestWidgetFunctionPlots::setPFuncStyle()
{
    pfunc->setDrawErrorLines(false);
    pfunc->setDrawErrorPolygons(false);
    if (pfuncErrorStyle->getErrorStyle()==JKQTPErrorLines) pfunc->setDrawErrorLines(true);
    if (pfuncErrorStyle->getErrorStyle()==JKQTPErrorPolygons) pfunc->setDrawErrorPolygons(true);
    pfunc->setDrawLine(chkPFuncDrawLine->isChecked());
    pfunc->setDisplaySamplePoints(chkPFuncDrawSamples->isChecked());
    plotFuncPlt->redrawPlot();
}

void TestWidgetFunctionPlots::setPFuncMinPoint(int value)
{
    pfunc->setMinSamples(value);
    plotFuncPlt->redrawPlot();
}

void TestWidgetFunctionPlots::setPFuncMaxRefine(int value)
{
    pfunc->setMaxRefinementDegree(value);
    plotFuncPlt->redrawPlot();
}

void TestWidgetFunctionPlots::setPFunc(const QString &expression)
{
    pfunc->setFunction(expression);
    plotFuncPlt->redrawPlot();
}

void TestWidgetFunctionPlots::setPFuncE(const QString &expression)
{
    pfunc->setErrorFunction(expression);
    plotFuncPlt->redrawPlot();
}
