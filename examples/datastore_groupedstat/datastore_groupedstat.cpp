/** \example datastore_groupedstat.cpp
 * Explains how to use the internal statistics library (see \ref jkqtptools_statistics ) together with JKQTPDatastore to generate grouped statistics (i.e. calculates errorbars or boxplots from groups of datapoints in a x/y-dataset).
 *
 * \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
 */
 
#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtplotter/graphs/jkqtpstatisticsadaptors.h"
#include "jkqtcommon_statistics_and_math/jkqtpstatisticstools.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <random>
#include <cmath>
#include <vector>


int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);



    // 1. create a window with several plotters and get a pointer to the internal datastores (for convenience)
    QWidget mainWidget;
    QGridLayout* lay;
    mainWidget.setLayout(lay=new QGridLayout);
    JKQTPlotter* plotbarchart=new JKQTPlotter(&mainWidget);
    plotbarchart->getPlotter()->setPlotLabel("Barcharts");
    JKQTPDatastore* datastore1=plotbarchart->getDatastore();
    lay->addWidget(plotbarchart,0,0);
    JKQTPlotter* plotboxplot=new JKQTPlotter(datastore1, &mainWidget);
    plotboxplot->getPlotter()->setPlotLabel("Boxplots");
    lay->addWidget(plotboxplot,0,1);
    JKQTPlotter* plotscattererrors=new JKQTPlotter(datastore1, &mainWidget);
    plotscattererrors->getPlotter()->setPlotLabel("Scatter Plot with Error Indicators");
    lay->addWidget(plotscattererrors,0,2);
    lay->setColumnStretch(0,1);
    lay->setColumnStretch(1,1);
    lay->setColumnStretch(2,2);



    // 2.   Barcharts from categorized data:
    // 2.1. First we generate a dataset, which is going to be used for a barchart
    //      The datapoints consist of pairs <group,value>, where the groups are encoded
    //      by the numbers 1,2,3 and in each group, several measurements are taken
    size_t colBarRawGroup=datastore1->addColumn("barchart, rawdata, group");
    size_t colBarRawValue=datastore1->addColumn("barchart, rawdata, value");

    //      data for group 1
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 1.1);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 1.5);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 0.8);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 1.2);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 1.4);
    //      data for group 2
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 2.2);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 2.4);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 1.9);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 2.6);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 2.1);
    //      data for group 3
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 4.1);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 4.4);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 3.8);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 4.5);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 3.7);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 4.0);
    // now some more datapoint, in mixed order
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 0.9);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 2.3);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 2.0);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 1.0);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 4.2);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 1.25);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 2.35);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 3.7);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 0.75);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 1.85);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 4.5);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 0.95);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 1.65);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 3, 4.1);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 1, 1.15);
    datastore1->appendToColumns(colBarRawGroup, colBarRawValue, 2, 2.15);


    // 2.2. This dataset can be visualized with a simple scatter plot:
    JKQTPXYLineGraph* gScatterForBar;
    plotbarchart->addGraph(gScatterForBar=new JKQTPXYLineGraph(plotbarchart));
    gScatterForBar->setXYColumns(colBarRawGroup, colBarRawValue);
    gScatterForBar->setDrawLine(false);
    gScatterForBar->setSymbolType(JKQTPCross);
    gScatterForBar->setSymbolSize(5);
    gScatterForBar->setSymbolColor(QColorWithAlphaF(QColor("red"), 0.5));

    // 2.3. Now we want to draw a barchart for every group, which indicates the
    //      average in each group. This is done using methods from the statistics
    //      library.
    //      First we need to group the data using jkqtpstatGroupData(), which assembles
    //      the data points in each group groupeddataBar
    std::map<double, std::vector<double> > groupeddataBar;
    jkqtpstatGroupData(datastore1->begin(colBarRawGroup), datastore1->end(colBarRawGroup),
                       datastore1->begin(colBarRawValue), datastore1->end(colBarRawValue),
                       groupeddataBar);
    //       now we can calculate the statistics for each group separately:
    //       Data is collected in two new columns
    size_t colBarGroup=datastore1->addColumn("barchart, group");
    size_t colBarAverage=datastore1->addColumn("barchart, group-average");
    size_t colBarStdDev=datastore1->addColumn("barchart, group-stddev");
    //       Statistics is calculated by simply iterating over groupeddataBar
    //       and calling functions like jkqtpstatAverage() for each group
    for (auto it=groupeddataBar.begin(); it!=groupeddataBar.end(); ++it) {
        datastore1->appendToColumn(colBarGroup, it->first);
        datastore1->appendToColumn(colBarAverage, jkqtpstatAverage(it->second.begin(), it->second.end()));
        datastore1->appendToColumn(colBarStdDev, jkqtpstatStdDev(it->second.begin(), it->second.end()));
    }

    // 2.4. Finally the calculated groups are drawn
    JKQTPBarVerticalErrorGraph* gBar;
    plotbarchart->addGraph(gBar=new JKQTPBarVerticalErrorGraph(plotbarchart));
    gBar->setXYColumns(colBarGroup, colBarAverage);
    gBar->setYErrorColumn(static_cast<int>(colBarStdDev));

    // 2.5. With the methods above we can also calculate more advanced statistics, like e.g. boxplots:
    size_t colBarMedian=datastore1->addColumn("barchart, group-median");
    size_t colBarMin=datastore1->addColumn("barchart, group-min");
    size_t colBarMax=datastore1->addColumn("barchart, group-max");
    size_t colBarQ25=datastore1->addColumn("barchart, group-Q25");
    size_t colBarQ75=datastore1->addColumn("barchart, group-Q75");
    for (auto it=groupeddataBar.begin(); it!=groupeddataBar.end(); ++it) {
        datastore1->appendToColumn(colBarMedian, jkqtpstatMedian(it->second.begin(), it->second.end()));
        datastore1->appendToColumn(colBarMin, jkqtpstatMinimum(it->second.begin(), it->second.end()));
        datastore1->appendToColumn(colBarMax, jkqtpstatMaximum(it->second.begin(), it->second.end()));
        datastore1->appendToColumn(colBarQ25, jkqtpstatQuantile(it->second.begin(), it->second.end(), 0.25));
        datastore1->appendToColumn(colBarQ75, jkqtpstatQuantile(it->second.begin(), it->second.end(), 0.75));
    }
    // 2.6. The result can be plotted using JKQTPBoxplotVerticalGraph, which receives a column for each value class of the final plot:
    JKQTPBoxplotVerticalGraph* gBoxplot;
    plotboxplot->addGraph(gBoxplot=new JKQTPBoxplotVerticalGraph(plotboxplot));
    gBoxplot->setPositionColumn(colBarGroup);
    gBoxplot->setMinColumn(colBarMin);
    gBoxplot->setMaxColumn(colBarMax);
    gBoxplot->setMedianColumn(colBarMedian);
    gBoxplot->setPercentile25Column(colBarQ25);
    gBoxplot->setPercentile75Column(colBarQ75);
    // 2.7. In order to safe yo the typing of the code above, shortcuts in the form of adaptors exist:
    /*
    jkqtpstatAddYErrorBarGraph(plotbarchart->getPlotter(),
                               datastore1->begin(colBarRawGroup), datastore1->end(colBarRawGroup),
                               datastore1->begin(colBarRawValue), datastore1->end(colBarRawValue));
    jkqtpstatAddHBoxplotsAndOutliers(plotboxplot->getPlotter(),
                                    datastore1->begin(colBarRawGroup), datastore1->end(colBarRawGroup),
                                    datastore1->begin(colBarRawValue), datastore1->end(colBarRawValue));
    */





    // 3.   Scatterplots from categorized data:
    // 3.1. First we generate a second dataset, which is going to be used for a scaterplot
    //      The datapoints consist of pairs <x,y>, that are based on a parabula with random
    //      deviations, both in x- and y-direction
    size_t colScatterRawX=datastore1->addColumn("scatterplot, rawdata, x");
    size_t colScatterRawY=datastore1->addColumn("scatterplot, rawdata, y");
    std::random_device rd; // random number generators:
    std::mt19937 gen{rd()};
    std::normal_distribution<> d1{0,0.5};
    const size_t N=100;
    const double xmax=3.5;
    for (size_t i=0; i<N; i++) {
        const double x=(static_cast<double>(i)-static_cast<double>(N)/2.0)*xmax/(static_cast<double>(N)/2.0);
        const double y=jkqtp_sqr(x)+2.0;
        datastore1->appendToColumns(colScatterRawX, colScatterRawY, x+d1(gen), y+d1(gen));
    }
    // 3.2. Now we can also add the raw dataset to the plot for visualization:
    JKQTPXYParametrizedScatterGraph* gScatterRaw;
    plotscattererrors->addGraph(gScatterRaw=new JKQTPXYParametrizedScatterGraph(plotscattererrors));
    gScatterRaw->setXYColumns(colScatterRawX, colScatterRawY);
    gScatterRaw->setDrawLine(false);
    gScatterRaw->setSymbolType(JKQTPCross);
    gScatterRaw->setSymbolSize(5);

    // 3.3. Now we want to draw a scatterchart of the data, where data-points should be grouped
    //      together, in x-intervals of width 0.5. From all the points in each interval, we calculate the
    //      in both x- and y-direction the average and standard deviation.
    //      First we need to group the data using jkqtpstatGroupData(), which assembles
    //      the data points in each group groupeddataScatter. For the custom grouping of the datapoints
    //      we use the optional functor provided to jkqtpstatGroupData(): We use jkqtpstatGroupingCustomRound1D()
    //      with given parameters 0.25 for the (center) location of the first bin and bin width 0.5. The functor
    //      is not built by hand (which would be possible using std::bind), but with the generator function
    //      jkqtpstatMakeGroupingCustomRound1D().
    //      in addition we use a variant of jkqtpstatGroupData(), which outputs a column with the category
    //      assigned to every data pair in the input data range
    std::map<double, std::pair<std::vector<double>,std::vector<double> > > groupeddataScatter;
    size_t colScatterRawGroup=datastore1->addColumn("scatterplot, rawdata, assigned-group");
    jkqtpstatGroupData(datastore1->begin(colScatterRawX), datastore1->end(colScatterRawX),
                       datastore1->begin(colScatterRawY), datastore1->end(colScatterRawY),
                       datastore1->backInserter(colScatterRawGroup),
                       groupeddataScatter,
                       jkqtpstatMakeGroupingCustomRound1D(0.25, 0.5));
    //       The column colScatterRawGroup can now be used to color the scatter graph:
    gScatterRaw->setColorColumn(colScatterRawGroup);
    //       now we can calculate the statistics for each group separately:
    //       Data is collected in two new columns
    size_t colScatterXAvg=datastore1->addColumn("scatter, x, average");
    size_t colScatterXStd=datastore1->addColumn("scatter, x, stddev");
    size_t colScatterYAvg=datastore1->addColumn("scatter, y, average");
    size_t colScatterYStd=datastore1->addColumn("scatter, y, stddev");
    //       Statistics is calculated by simply iterating over groupeddataScatter
    //       and calling functions like jkqtpstatAverage() for each group
    for (auto it=groupeddataScatter.begin(); it!=groupeddataScatter.end(); ++it) {
        datastore1->appendToColumn(colScatterXAvg, jkqtpstatAverage(it->second.first.begin(), it->second.first.end()));
        datastore1->appendToColumn(colScatterXStd, jkqtpstatStdDev(it->second.first.begin(), it->second.first.end()));
        datastore1->appendToColumn(colScatterYAvg, jkqtpstatAverage(it->second.second.begin(), it->second.second.end()));
        datastore1->appendToColumn(colScatterYStd, jkqtpstatStdDev(it->second.second.begin(), it->second.second.end()));
    }

    // 3.4. Finally the calculated groups are drawn
    JKQTPXYLineErrorGraph* gScatterErr;
    plotscattererrors->addGraph(gScatterErr=new JKQTPXYLineErrorGraph(plotscattererrors));
    gScatterErr->setXYColumns(colScatterXAvg, colScatterYAvg);
    gScatterErr->setXErrorColumn(static_cast<int>(colScatterXStd));
    gScatterErr->setYErrorColumn(static_cast<int>(colScatterYStd));
    gScatterErr->setSymbolType(JKQTPFilledTriangle);
    gScatterErr->setDrawLine(false);

    // 3.5. also here an adaptor exists, which makes the task easier:
    /*
    jkqtpstatAddXYErrorLineGraph(plotscattererrors->getPlotter(),
                                 datastore1->begin(colScatterRawX), datastore1->end(colScatterRawX),
                                 datastore1->begin(colScatterRawY), datastore1->end(colScatterRawY),
                                 jkqtpstatMakeGroupingCustomRound1D(0.25, 0.5));
    */




    // autoscale the plot so the graph is contained
    plotboxplot->synchronizeToMaster(plotbarchart, JKQTBasePlotter::sdXYAxes);
    plotboxplot->zoomToFit();
    plotboxplot->setGrid(false);
    plotboxplot->setShowZeroAxes(false);
    plotboxplot->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plotbarchart->setAbsoluteY(0,5);
    plotboxplot->setAbsoluteY(0,5);
    plotbarchart->zoomToFit();
    plotbarchart->setGrid(false);
    plotbarchart->setShowZeroAxes(false);
    plotbarchart->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plotbarchart->moveGraphTop(gScatterForBar);
    plotscattererrors->zoomToFit();
    plotscattererrors->setGrid(false);
    plotscattererrors->setShowZeroAxes(false);
    plotscattererrors->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);


    // show plotter and make it a decent size
    mainWidget.show();
    mainWidget.resize(1200,400);

    return app.exec();
}
