#include "TestWidgetPeaksPlots.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>







TestWidgetPeaksPlots::TestWidgetPeaksPlots(QWidget *parent) :
    QWidget(parent)
{


    QVBoxLayout* layoutpeaks=new QVBoxLayout(this);
    setLayout(layoutpeaks);
    resize(1000, 800);
    plotPeaks=new JKQtPlotter(true, this);
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
    int phot1=plotPeaks->getDatastore()->addCopiedColumn(photons1, "photons 1");
    int phot2=plotPeaks->getDatastore()->addCopiedColumn(photons2, "photons 2");

    graphPeakStream1=new JKQTPPeakStreamGraph(phot1, 0.05, 1, QColor("darkgreen"), plotPeaks->get_plotter());
    graphPeakStream1->set_title("channel 1");
    plotPeaks->get_plotter()->addGraph(graphPeakStream1);
    graphPeakStream2=new JKQTPPeakStreamGraph(phot2, -0.05, -1, QColor("darkred"), plotPeaks->get_plotter());
    graphPeakStream2->set_title("channel 2");
    plotPeaks->get_plotter()->addGraph(graphPeakStream2);

    plotPeaks->set_doDrawing(true);
    plotPeaks->zoomToFit();






}

void TestWidgetPeaksPlots::setDrawBaseline(bool checked)
{
    graphPeakStream1->set_drawBaseline(checked);
    graphPeakStream2->set_drawBaseline(checked);
    plotPeaks->update_plot();
}

void TestWidgetPeaksPlots::setYPeaks(bool checked)
{
    graphPeakStream1->set_yPeaks(checked);
    graphPeakStream2->set_yPeaks(checked);
    plotPeaks->update_plot();
}
