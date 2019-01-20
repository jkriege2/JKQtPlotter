/** \example test_multiplot.cpp
 * JKQTPLotter: Examples: Laying out Several Plots
 *
 * \ref JKQTPLotterMultiPlotLayout
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsparsedfunction.h"
#include "jkqtplotter/jkqtpgraphsbarchart.h"
#include <random>
#include <cmath>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a widget
    QWidget mainWidget;
    mainWidget.setWindowTitle("JKQTPLotter(s) in a QGridLayout");

    // 2. Create a QGridLayout for the plots and add it to the widget.
    QGridLayout* layout=new QGridLayout();
    mainWidget.setLayout(layout);

    // 3.1 create a main plotter widget and add it to the layout
    JKQTPLotter* plotMain=new JKQTPLotter(&mainWidget);
    layout->addWidget(plotMain, 0,0);
    JKQTPDatastore* ds=plotMain->getDatastore();

    // 3.2 create a second and third plotter widget and add them to the
    //     layout below and at the bottom right of the plotMain.
    //     Also configure it to use the same datastore as plotMain
    JKQTPLotter* plotResid=new JKQTPLotter(false, &mainWidget, ds);
    layout->addWidget(plotResid, 1,0);
    JKQTPLotter* plotResidHist=new JKQTPLotter(false, &mainWidget, ds);
    layout->addWidget(plotResidHist, 1,1);

    // 3.3 synchronize width/x-axis of plotResid to width/x-axis of plotMain
    plotResid->get_plotter()->synchronizeToMaster(plotMain->get_plotter(), true, false, true, true);

    // 3.4 synchronize y-axis of width/plotResidHist to y-axis of width/plotResid
    plotResidHist->get_plotter()->synchronizeToMaster(plotResid->get_plotter(), false, true, true, true);

    // 3.5 ensure that the plot are printed/exported in whole, when printing in plotMain
    plotMain->get_plotter()->set_gridPrinting(true);
    plotMain->get_plotter()->addGridPrintingPlotter(0,1,plotResid->get_plotter());
    plotMain->get_plotter()->addGridPrintingPlotter(1,1,plotResidHist->get_plotter());

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
    graphD->set_xColumn(cX);
    graphD->set_yColumn(cY);
    graphD->set_drawLine(false);
    graphD->set_symbol(JKQTPCross);
    graphD->set_symbolSize(10);
    graphD->set_title("measurement data");
    plotMain->addGraph(graphD);

    // 5.2 plot of the graph as an interpreted function, set as string "2*(1+cos(x))"
    JKQTPXParsedFunctionLineGraph* graphFit=new JKQTPXParsedFunctionLineGraph(plotMain);
    graphFit->set_function("2*(1+cos(x))");
    graphFit->set_title("fit");
    plotMain->addGraph(graphFit);

    // 5.3 residuals plot
    JKQTPXYLineGraph* graphResid=new JKQTPXYLineGraph(plotResid);
    graphResid->set_xColumn(cX);
    graphResid->set_yColumn(cRY);
    graphResid->set_drawLine(false);
    graphResid->set_symbol(JKQTPPlus);
    graphResid->set_symbolSize(10);
    graphResid->set_drawLine(true);
    graphResid->set_lineWidth(0.5);
    graphResid->set_title("residuals");
    plotResid->addGraph(graphResid);

    // 5.3 residual histogram plot
    JKQTPBarHorizontalGraph* graphResidHist=new JKQTPBarHorizontalGraph(plotResidHist);
    graphResidHist->set_xColumn(cH.second);
    graphResidHist->set_yColumn(cH.first);
    graphResidHist->set_title("histogram");
    plotResidHist->addGraph(graphResidHist);

    // 6.1 axis labels, distributed over the several plots
    plotMain->get_yAxis()->set_axisLabel("y axis");
    plotResid->get_xAxis()->set_axisLabel("x axis");
    plotResid->get_yAxis()->set_axisLabel("residuals");
    plotResidHist->get_xAxis()->set_axisLabel("frequency");
    // 6.2 switch off the tick labels on the axes that directly face another plot
    plotMain->get_xAxis()->set_drawMode1(JKQTPCADMticks);
    plotResidHist->get_yAxis()->set_drawMode1(JKQTPCADMticks);
    // 6.3 show tick labels on the rhs y-axis of the residual histogram plot
    plotResidHist->get_yAxis()->set_drawMode2(JKQTPCADMticksAndLabels);
    // 6.4 hide keys in all plots but the main plot
    plotResid->get_plotter()->set_showKey(false);
    plotResidHist->get_plotter()->set_showKey(false);
    // 6.5 hide position label and toolbars in the plots except main plot
    plotResid->set_displayToolbar(false);
    plotResid->set_displayMousePosition(false);
    plotResidHist->set_displayToolbar(false);
    plotResidHist->set_displayMousePosition(false);
    plotMain->set_toolbarAlwaysOn(true);


    // 7. scale plots automatically to data
    plotResid->zoomToFit();
    plotResidHist->zoomToFit();
    plotMain->zoomToFit();

    // 8. show plotter and make it a decent size
    mainWidget.show();
    mainWidget.move(32,32);
    mainWidget.resize(800,600);

    return app.exec();
}
