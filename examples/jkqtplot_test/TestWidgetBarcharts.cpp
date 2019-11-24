#include "TestWidgetBarcharts.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>
#include "jkqtplotter/graphs/jkqtpbarchart.h"





TestWidgetBarcharts::TestWidgetBarcharts(QWidget *parent) :
    QWidget(parent)
{


    QVBoxLayout* barchartLayout=new QVBoxLayout();
    setLayout(barchartLayout);
    resize(1000, 800);
    plotBarchart=new JKQTPlotter(true, this);
    plotBarchart->setPlotUpdateEnabled(false);
    plotBarchart->getPlotter()->setPlotLabel(tr("\\textbf{bar charts}"));
    plotBarchart->setObjectName("plotBarchart");
    plotBarchart->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");
    plotBarchart->getPlotter()->getXAxis()->setLabelType(JKQTPCALTdate);
    plotBarchart->getPlotter()->getXAxis()->setTickDateFormat("dd. MMM yyyy");

    barchartLayout->addWidget(plotBarchart);

    QVector<double> dataBCX, dataBCXD, dataBCY1, dataBCY2, dataBCYE1, dataBCYE2;
    for (int i=0; i<8; i++) {
        double x=double(i);
        dataBCX<<x;
        QDateTime dt;
        dt.setDate(QDate(2012,1+i,1));
        //qDebug()<<dt<<dt.toMSecsSinceEpoch();
        dataBCXD<<double(dt.toMSecsSinceEpoch());
		dataBCY1 << 1.0 + sin(x * JKQTPSTATISTICS_PI / 6.0);
		dataBCY2 << 2.0 + cos(x * JKQTPSTATISTICS_PI / 3.0);
        dataBCYE1<<dataBCY1.last()*0.1;
        dataBCYE2<<dataBCY2.last()*0.05;
    }
    size_t bcpx=plotBarchart->getDatastore()->addCopiedColumn(dataBCX, "x");
    size_t bcpxd=plotBarchart->getDatastore()->addCopiedColumn(dataBCXD, "x_date");
    size_t bcpy1=plotBarchart->getDatastore()->addCopiedColumn(dataBCY1, "y1");
    size_t bcpye1=plotBarchart->getDatastore()->addCopiedColumn(dataBCYE1, "ye1");
    size_t bcpy2=plotBarchart->getDatastore()->addCopiedColumn(dataBCY2, "y2");
    size_t bcpye2=plotBarchart->getDatastore()->addCopiedColumn(dataBCYE2, "ye2");

    JKQTPBarVerticalErrorGraph* plteBar1=new JKQTPBarVerticalErrorGraph(plotBarchart->getPlotter());
    plteBar1->setTitle(tr("bars 1"));
    plteBar1->setXColumn(bcpxd);
    plteBar1->setYColumn(bcpy1);
    plteBar1->setYErrorColumn(bcpye1);
    plteBar1->setWidth(0.45);
    plteBar1->setShift(-0.25);
    plotBarchart->getPlotter()->addGraph(plteBar1);

    JKQTPBarVerticalErrorGraph* plteBar2=new JKQTPBarVerticalErrorGraph(plotBarchart->getPlotter());
    plteBar2->setXColumn(bcpxd);
    plteBar2->setYColumn(bcpy2);
    plteBar2->setYErrorColumn(bcpye2);
    plteBar2->setTitle(tr("bars 2"));
    plteBar2->setWidth(0.45);
    plteBar2->setShift(0.25);
    plteBar2->setBaseline(0.5);
    plteBar2->setYErrorSymmetric(false);
    plotBarchart->getPlotter()->addGraph(plteBar2);

    plteBar2->autoscaleBarWidthAndShift();
    plotBarchart->setPlotUpdateEnabled(true);
    plotBarchart->zoomToFit();




    plotBarchart2=new JKQTPlotter(false, this, plotBarchart->getDatastore());
    plotBarchart2->setPlotUpdateEnabled(false);
    plotBarchart2->getPlotter()->setPlotLabel(tr("\\textbf{bar charts}"));
    plotBarchart2->setObjectName("plotBarchart2");
    plotBarchart2->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");

    barchartLayout->addWidget(plotBarchart2);

    JKQTPBarHorizontalErrorGraph* plteBar3=new JKQTPBarHorizontalErrorGraph(plotBarchart2->getPlotter());
    plteBar3->setTitle(tr("bars 1"));
    plteBar3->setYColumn(bcpx);
    plteBar3->setXColumn(bcpy1);
    plteBar3->setXErrorColumn(bcpye1);
    plteBar3->setWidth(0.45);
    plteBar3->setShift(-0.25);
    plotBarchart2->getPlotter()->addGraph(plteBar3);

    JKQTPBarHorizontalErrorGraph* plteBar4=new JKQTPBarHorizontalErrorGraph(plotBarchart2->getPlotter());
    plteBar4->setYColumn(bcpx);
    plteBar4->setXColumn(bcpy2);
    plteBar4->setXErrorColumn(bcpye2);
    plteBar4->setTitle(tr("bars 2"));
    plteBar4->setWidth(0.45);
    plteBar4->setShift(0.25);
    plteBar4->setXErrorSymmetric(false);
    plotBarchart2->getPlotter()->addGraph(plteBar4);

    plteBar4->autoscaleBarWidthAndShift(0.9, 1);
    plotBarchart2->setPlotUpdateEnabled(true);
    plotBarchart2->zoomToFit();

    chkBarLog=new QCheckBox(tr("log-log plots"), this);
    chkBarLog->setChecked(false);
    barchartLayout->addWidget(chkBarLog);
    connect(chkBarLog, SIGNAL(toggled(bool)), this, SLOT(setBarchartLogLog(bool)));

    spinBarLabelAngel=new QSpinBox(this);
    spinBarLabelAngel->setPrefix(tr("rotation angel:  "));
    spinBarLabelAngel->setSuffix(QLatin1String("\xB0"));
    spinBarLabelAngel->setRange(-180,180);
    spinBarLabelAngel->setValue(0);
    spinBarLabelAngel->setSingleStep(15);
    barchartLayout->addWidget(spinBarLabelAngel);
    connect(spinBarLabelAngel, SIGNAL(valueChanged(int)), this, SLOT(setBarchartAngele(int)));

    spinBarLabelAngel2=new QSpinBox(this);
    spinBarLabelAngel2->setPrefix(tr("rotation angel:  "));
    spinBarLabelAngel2->setSuffix(QLatin1String("\xB0"));
    spinBarLabelAngel2->setRange(-180,180);
    spinBarLabelAngel2->setValue(0);
    spinBarLabelAngel2->setSingleStep(15);
    barchartLayout->addWidget(spinBarLabelAngel2);
    connect(spinBarLabelAngel2, SIGNAL(valueChanged(int)), this, SLOT(setBarchartAngele2(int)));


}


void TestWidgetBarcharts::setBarchartLogLog(bool checked)
{
    plotBarchart->getXAxis()->setLogAxis(checked);
    plotBarchart->getYAxis()->setLogAxis(checked);
    plotBarchart->zoomToFit();
    plotBarchart2->getXAxis()->setLogAxis(checked);
    plotBarchart2->getYAxis()->setLogAxis(checked);
    plotBarchart2->zoomToFit();
}

void TestWidgetBarcharts::setBarchartAngele(int angle)
{
    plotBarchart->getXAxis()->setTickLabelAngle(angle);
    plotBarchart->getXAxis()->setDrawMode2(JKQTPCADMcomplete);
}

void TestWidgetBarcharts::setBarchartAngele2(int angle)
{
    plotBarchart->getYAxis()->setTickLabelAngle(angle);
    plotBarchart->getYAxis()->setDrawMode2(JKQTPCADMcomplete);
}
