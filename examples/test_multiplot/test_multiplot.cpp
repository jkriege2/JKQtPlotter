/** \example test_multiplot.cpp
 * JKQTPlotter: Examples: Laying out Several Plots
 *
 * \ref JKQTPlotterMultiPlotLayout
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsparsedfunction.h"
#include "jkqtplotter/jkqtpgraphsbarchart.h"
#include <random>
#include <cmath>
#include "test_multiplot_ui.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a widget
    QWidget mainWidget;
    mainWidget.setWindowTitle("JKQTPlotter(s) in a QGridLayout");

    // 2. Create a QGridLayout for the plots and add it to the widget.
    QGridLayout* layout=new QGridLayout();
    mainWidget.setLayout(layout);

    // 3.1 create a main plotter widget and add it to the layout
    JKQTPlotter* plotMain=new JKQTPlotter(&mainWidget);
    layout->addWidget(plotMain, 0,0);
    JKQTPDatastore* ds=plotMain->getDatastore();

    // 3.2 create a second and third plotter widget and add them to the
    //     layout below and at the bottom right of the plotMain.
    //     Also configure it to use the same datastore as plotMain
    JKQTPlotter* plotResid=new JKQTPlotter(false, &mainWidget, ds);
    layout->addWidget(plotResid, 1,0);
    JKQTPlotter* plotResidHist=new JKQTPlotter(false, &mainWidget, ds);
    layout->addWidget(plotResidHist, 1,1);

    // 3.3 synchronize width/x-axis of plotResid to width/x-axis of plotMain
    plotResid->synchronizeToMaster(plotMain, JKQTBasePlotter::sdXAxis, true, true, true);

    // 3.4 synchronize y-axis of width/plotResidHist to y-axis of width/plotResid
    plotResidHist->synchronizeToMaster(plotResid, JKQTBasePlotter::sdYAxis, true, true, true);

    // 3.5 ensure that the plot are printed/exported in whole, when printing in plotMain
    plotMain->getPlotter()->setGridPrinting(true);
    plotMain->getPlotter()->addGridPrintingPlotter(0,1,plotResid->getPlotter());
    plotMain->getPlotter()->addGridPrintingPlotter(1,1,plotResidHist->getPlotter());

    // 3.6 set relative sizes of the plots via the layout (small plots have 1/3 the width and height of the large plot
    layout->setRowStretch(0,3);
    layout->setRowStretch(1,1);
    layout->setColumnStretch(0,3);
    layout->setColumnStretch(1,1);



    // 4. now we create some (artificial) data:
    //     - in plotMain we show an exponential curve and and some datapoints
    //     - in plotResid we show the residuals, i.e. the difference between the curve and the datapoints
    //     - in plotResidHist we will show a histogram of the residuals (calculated in histogram)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0,0.5);

    std::vector<double> dataX, dataY, dataRY;
    std::map<double, int> histogram;
    const int Ndata=60;
    for(int n=0; n<Ndata; ++n) {
        const double x=3.0*M_PI/static_cast<double>(Ndata)*static_cast<double>(n)-0.5;
        const double y=2.0*(1.0+cos(x));
        const double yd=y+d(gen);
        dataX.push_back(x);
        dataY.push_back(yd);
        dataRY.push_back(y-yd);
        // calculate a simple type of histogram in a std:map, bins are 1/4=0.25 wide
        const double hbin=std::round((y-yd)*4.0)/4.0;
        if (histogram.find(hbin)==histogram.end()) {
            histogram[hbin]=0;
        } else {
            histogram[hbin]++;
        }
    }

    // 5. and add the data to the datastore
    size_t cX=ds->addCopiedColumn(dataX, "dataX");
    size_t cY=ds->addCopiedColumn(dataY, "dataY");
    size_t cRY=ds->addCopiedColumn(dataRY, "residY");
    std::pair<size_t,size_t> cH=ds->addCopiedMap(histogram, "histX", "histY");

    // 5.1 plot of the data
    JKQTPXYLineGraph* graphD=new JKQTPXYLineGraph(plotMain);
    graphD->setXColumn(cX);
    graphD->setYColumn(cY);
    graphD->setDrawLine(false);
    graphD->setSymbol(JKQTPCross);
    graphD->setSymbolSize(10);
    graphD->setTitle("measurement data");
    plotMain->addGraph(graphD);

    // 5.2 plot of the graph as an interpreted function, set as string "2*(1+cos(x))"
    JKQTPXParsedFunctionLineGraph* graphFit=new JKQTPXParsedFunctionLineGraph(plotMain);
    graphFit->setFunction("2*(1+cos(x))");
    graphFit->setTitle("fit");
    plotMain->addGraph(graphFit);

    // 5.3 residuals plot
    JKQTPXYLineGraph* graphResid=new JKQTPXYLineGraph(plotResid);
    graphResid->setXColumn(cX);
    graphResid->setYColumn(cRY);
    graphResid->setDrawLine(false);
    graphResid->setSymbol(JKQTPPlus);
    graphResid->setSymbolSize(10);
    graphResid->setDrawLine(true);
    graphResid->setLineWidth(0.5);
    graphResid->setTitle("residuals");
    plotResid->addGraph(graphResid);

    // 5.3 residual histogram plot
    JKQTPBarHorizontalGraph* graphResidHist=new JKQTPBarHorizontalGraph(plotResidHist);
    graphResidHist->setXColumn(cH.second);
    graphResidHist->setYColumn(cH.first);
    graphResidHist->setTitle("histogram");
    plotResidHist->addGraph(graphResidHist);

    // 6.1 axis labels, distributed over the several plots
    plotMain->getYAxis()->setAxisLabel("y axis");
    plotResid->getXAxis()->setAxisLabel("x axis");
    plotResid->getYAxis()->setAxisLabel("residuals");
    plotResidHist->getXAxis()->setAxisLabel("frequency");
    // 6.2 switch off the tick labels on the axes that directly face another plot
    plotMain->getXAxis()->setDrawMode1(JKQTPCADMLineTicks);
    plotResidHist->getYAxis()->setDrawMode1(JKQTPCADMLineTicks);
    // 6.3 show tick labels on the rhs y-axis of the residual histogram plot
    plotResidHist->getYAxis()->setDrawMode2(JKQTPCADMLineTicksTickLabels);
    // 6.4 hide keys in all plots but the main plot
    plotResid->getPlotter()->setShowKey(false);
    plotResidHist->getPlotter()->setShowKey(false);
    // 6.5 hide position label and toolbars in the plots except main plot
    plotResid->setToolbarEnabled(false);
    plotResid->setMousePositionShown(false);
    plotResidHist->setToolbarEnabled(false);
    plotResidHist->setMousePositionShown(false);
    plotMain->setToolbarEnabled(true);


    // 7. scale plots automatically to data
    plotResidHist->setAbsoluteX(0, 20);
    plotResid->zoomToFit();
    plotResidHist->zoomToFit();
    plotMain->zoomToFit();

    // 8. show plotter and make it a decent size
    mainWidget.show();
    mainWidget.move(32,32);
    mainWidget.resize(800,600);

    // 9. create a second window, which controls the synchronization and layout between the plots
    TestMultiplotUI controlWindow(plotMain, plotResid, plotResidHist, layout);
    controlWindow.move(850,32);
    controlWindow.show();

    return app.exec();
}
