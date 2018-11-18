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
    plotFuncPlt=new JKQtPlotter(true, this);
    plotFuncLayout->addWidget(plotFuncPlt);
    pfunc=new JKQTPxParsedFunctionLineGraph(plotFuncPlt->get_plotter());
    pfunc->set_function("x^2/10+sin(x*pi*10)");
    pfunc->set_errorFunction("x^2/20");
    pfunc->set_drawErrorPolygons(true);
    pfunc->set_style(Qt::DashLine);
    plotFuncPlt->addGraph(pfunc);
    plotFuncPlt->update_plot();

    pfuncErrorStyle=new JKQTPerrorPlotstyleComboBox(this);
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
    spinPFunc->setValue(pfunc->get_minSamples());
    plotFuncLayout->addWidget(new QLabel(tr("min samples:"), this));
    plotFuncLayout->addWidget(spinPFunc);
    connect(spinPFunc, SIGNAL(valueChanged(int)), this, SLOT(setPFuncMinPoint(int)));
    spinPFunc=new QSpinBox(this);
    spinPFunc->setValue(pfunc->get_maxRefinementDegree());
    plotFuncLayout->addWidget(new QLabel(tr("max refinement degree:"), this));
    plotFuncLayout->addWidget(spinPFunc);
    connect(spinPFunc, SIGNAL(valueChanged(int)), this, SLOT(setPFuncMaxRefine(int)));
    QComboBox* edtPFunc=new QComboBox(this);
    edtPFunc->addItem(pfunc->get_function());
    edtPFunc->addItem("x");
    edtPFunc->addItem("x^2");
    edtPFunc->addItem("x^2-x^3");
    edtPFunc->addItem("sin(x)");
    edtPFunc->setCurrentIndex(0);
    plotFuncLayout->addWidget(new QLabel(tr("function:"), this));
    plotFuncLayout->addWidget(edtPFunc);
    connect(edtPFunc, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPFunc(QString)));
    edtPFunc=new QComboBox(this);
    edtPFunc->addItem(pfunc->get_errorFunction());
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
    connect(chkFLogX, SIGNAL(toggled(bool)), plotFuncPlt->getXAxis(), SLOT(set_logAxis(bool)));
    plotFuncLayout->addWidget(chkFLogX);
    QCheckBox* chkFLogY=new QCheckBox(tr("y-axis: log-scale"));
    chkFLogY->setChecked(false);
    connect(chkFLogY, SIGNAL(toggled(bool)), plotFuncPlt->getYAxis(), SLOT(set_logAxis(bool)));
    plotFuncLayout->addWidget(chkFLogY);


}


void TestWidgetFunctionPlots::setPFuncStyle()
{
    pfunc->set_drawErrorLines(false);
    pfunc->set_drawErrorPolygons(false);
    if (pfuncErrorStyle->getErrorStyle()==JKQTPerrorLines) pfunc->set_drawErrorLines(true);
    if (pfuncErrorStyle->getErrorStyle()==JKQTPerrorPolygons) pfunc->set_drawErrorPolygons(true);
    pfunc->set_drawLine(chkPFuncDrawLine->isChecked());
    pfunc->set_displaySamplePoints(chkPFuncDrawSamples->isChecked());
    plotFuncPlt->update_plot();
}

void TestWidgetFunctionPlots::setPFuncMinPoint(int value)
{
    pfunc->set_minSamples(value);
    plotFuncPlt->update_plot();
}

void TestWidgetFunctionPlots::setPFuncMaxRefine(int value)
{
    pfunc->set_maxRefinementDegree(value);
    plotFuncPlt->update_plot();
}

void TestWidgetFunctionPlots::setPFunc(const QString &expression)
{
    pfunc->set_function(expression);
    plotFuncPlt->update_plot();
}

void TestWidgetFunctionPlots::setPFuncE(const QString &expression)
{
    pfunc->set_errorFunction(expression);
    plotFuncPlt->update_plot();
}
