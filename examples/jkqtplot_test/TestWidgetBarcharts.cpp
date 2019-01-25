#include "TestWidgetBarcharts.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>
#include "jkqtplotter/jkqtpgraphsbarchart.h"





TestWidgetBarcharts::TestWidgetBarcharts(QWidget *parent) :
    QWidget(parent)
{


    QVBoxLayout* barchartLayout=new QVBoxLayout(this);
    setLayout(barchartLayout);
    resize(1000, 800);
    plotBarchart=new JKQTPlotter(true, this);
    plotBarchart->setPlotUpdateEnabled(false);
    plotBarchart->getPlotter()->setPlotLabel(tr("\\textbf{bar charts}"));
    plotBarchart->setObjectName("plotBarchart");
    plotBarchart->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");
    plotBarchart->getPlotter()->getXAxis()->set_labelType(JKQTPCALTdate);
    plotBarchart->getPlotter()->getXAxis()->set_tickDateFormat("dd. MMM yyyy");

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
    size_t bcpx=plotBarchart->getDatastore()->addCopiedColumn(dataBCX, "x");
    size_t bcpxd=plotBarchart->getDatastore()->addCopiedColumn(dataBCXD, "x_date");
    size_t bcpy1=plotBarchart->getDatastore()->addCopiedColumn(dataBCY1, "y1");
    size_t bcpye1=plotBarchart->getDatastore()->addCopiedColumn(dataBCYE1, "ye1");
    size_t bcpy2=plotBarchart->getDatastore()->addCopiedColumn(dataBCY2, "y2");
    size_t bcpye2=plotBarchart->getDatastore()->addCopiedColumn(dataBCYE2, "ye2");

    JKQTPBarVerticalErrorGraph* plteBar1=new JKQTPBarVerticalErrorGraph(plotBarchart->getPlotter());
    plteBar1->set_title(tr("bars 1"));
    plteBar1->set_xColumn(bcpxd);
    plteBar1->set_yColumn(bcpy1);
    plteBar1->set_yErrorColumn(bcpye1);
    plteBar1->set_width(0.45);
    plteBar1->set_shift(-0.25);
    plotBarchart->getPlotter()->addGraph(plteBar1);

    JKQTPBarVerticalErrorGraph* plteBar2=new JKQTPBarVerticalErrorGraph(plotBarchart->getPlotter());
    plteBar2->set_xColumn(bcpxd);
    plteBar2->set_yColumn(bcpy2);
    plteBar2->set_yErrorColumn(bcpye2);
    plteBar2->set_title(tr("bars 2"));
    plteBar2->set_width(0.45);
    plteBar2->set_shift(0.25);
    plteBar2->set_baseline(0.5);
    plteBar2->set_yErrorSymmetric(false);
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
    plteBar3->set_title(tr("bars 1"));
    plteBar3->set_yColumn(bcpx);
    plteBar3->set_xColumn(bcpy1);
    plteBar3->set_xErrorColumn(bcpye1);
    plteBar3->set_width(0.45);
    plteBar3->set_shift(-0.25);
    plotBarchart2->getPlotter()->addGraph(plteBar3);

    JKQTPBarHorizontalErrorGraph* plteBar4=new JKQTPBarHorizontalErrorGraph(plotBarchart2->getPlotter());
    plteBar4->set_yColumn(bcpx);
    plteBar4->set_xColumn(bcpy2);
    plteBar4->set_xErrorColumn(bcpye2);
    plteBar4->set_title(tr("bars 2"));
    plteBar4->set_width(0.45);
    plteBar4->set_shift(0.25);
    plteBar4->set_xErrorSymmetric(false);
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


}


void TestWidgetBarcharts::setBarchartLogLog(bool checked)
{
    plotBarchart->getXAxis()->set_logAxis(checked);
    plotBarchart->getYAxis()->set_logAxis(checked);
    plotBarchart->zoomToFit();
    plotBarchart2->getXAxis()->set_logAxis(checked);
    plotBarchart2->getYAxis()->set_logAxis(checked);
    plotBarchart2->zoomToFit();
}

void TestWidgetBarcharts::setBarchartAngele(int angle)
{
    plotBarchart->getXAxis()->set_tickLabelAngle(angle);
    plotBarchart->getXAxis()->set_drawMode2(JKQTPCADMcomplete);
}

void TestWidgetBarcharts::setBarchartAngele2(int angle)
{
    plotBarchart->getYAxis()->set_tickLabelAngle(angle);
    plotBarchart->getYAxis()->set_drawMode2(JKQTPCADMcomplete);
}
