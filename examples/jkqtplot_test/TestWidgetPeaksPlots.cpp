#include "TestWidgetPeaksPlots.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>







TestWidgetPeaksPlots::TestWidgetPeaksPlots(QWidget *parent) :
    QWidget(parent)
{


    QVBoxLayout* layoutpeaks=new QVBoxLayout();
    setLayout(layoutpeaks);
    resize(1000, 800);
    plotPeaks=new JKQTPlotter(true, this);
    plotPeaks->setPlotUpdateEnabled(false);
    plotPeaks->getPlotter()->setPlotLabel(tr("\\textbf{peaks stream plot}"));
    plotPeaks->setObjectName("plotPeaks");
    plotPeaks->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_peaks/");

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
        const double p=0.33*fabs(sin(x*4.0*JKQTPSTATISTICS_PI))*fabs(sin(x*4.0*JKQTPSTATISTICS_PI));
        if (static_cast<double>(rand())/static_cast<double>(RAND_MAX)<=p) photons1<<x;
        if (static_cast<double>(rand())/static_cast<double>(RAND_MAX)<=p) photons2<<x;
    }
    int phot1=plotPeaks->getDatastore()->addCopiedColumn(photons1, "photons 1");
    int phot2=plotPeaks->getDatastore()->addCopiedColumn(photons2, "photons 2");

    graphPeakStream1=new JKQTPPeakStreamGraph(plotPeaks->getPlotter());
    graphPeakStream1->setDataColumn(phot1);
    graphPeakStream1->setColor(QColor("darkgreen"));
    graphPeakStream1->setPeakHeight(1);
    graphPeakStream1->setBaseline(0.05);
    graphPeakStream1->setTitle("channel 1");
    plotPeaks->getPlotter()->addGraph(graphPeakStream1);
    graphPeakStream2=new JKQTPPeakStreamGraph(plotPeaks->getPlotter());
    graphPeakStream2->setDataColumn(phot2);
    graphPeakStream2->setColor(QColor("darkred"));
    graphPeakStream2->setPeakHeight(-1);
    graphPeakStream2->setBaseline(-0.05);
    graphPeakStream2->setTitle("channel 2");
    plotPeaks->getPlotter()->addGraph(graphPeakStream2);

    plotPeaks->setPlotUpdateEnabled(true);
    plotPeaks->zoomToFit();






}

void TestWidgetPeaksPlots::setDrawBaseline(bool checked)
{
    graphPeakStream1->setDrawBaseline(checked);
    graphPeakStream2->setDrawBaseline(checked);
    plotPeaks->redrawPlot();
}

void TestWidgetPeaksPlots::setYPeaks(bool checked)
{
    graphPeakStream1->setYPeaks(checked);
    graphPeakStream2->setYPeaks(checked);
    plotPeaks->redrawPlot();
}
