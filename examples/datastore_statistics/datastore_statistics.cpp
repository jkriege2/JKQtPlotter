/** \example datastore_statistics.cpp
 * Explains how to use the internal statistics library (see \ref jkqtptools_statistics ) together with JKQTPDatastore to generate advanced plots for 1-dimensional data.
 *
 * \ref JKQTPlotterBasicJKQTPDatastoreStatistics
 */
 
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtppeakstream.h"
#include "jkqtplotter/graphs/jkqtpboxplot.h"
#include "jkqtplotter/graphs/jkqtpstatisticsadaptors.h"
#include "jkqtplotter/graphs/jkqtpevaluatedfunction.h"
#include "jkqtcommon/jkqtpstatisticstools.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <random>
#include <cmath>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);


    // 1. create a window with several plotters and get a pointer to the internal datastores (for convenience)
    QWidget mainWidget;
    QGridLayout* lay;
    mainWidget.setLayout(lay=new QGridLayout);
    JKQTPlotter* plot1=new JKQTPlotter(&mainWidget);
    plot1->getPlotter()->setPlotLabel("Histograms and KDE");
    JKQTPDatastore* datastore1=plot1->getDatastore();
    lay->addWidget(plot1,1,0);
    JKQTPlotter* plot1cum=new JKQTPlotter(datastore1, &mainWidget);
    plot1cum->getPlotter()->setPlotLabel("Cummulative Histogram");
    lay->addWidget(plot1cum,1,1);
    JKQTPlotter* plot1kde=new JKQTPlotter(datastore1, &mainWidget);
    plot1kde->getPlotter()->setPlotLabel("Kernel Density Estimate");
    lay->addWidget(plot1kde,0,1);
    JKQTPlotter* plot1box=new JKQTPlotter(datastore1, &mainWidget);
    plot1box->getPlotter()->setPlotLabel("Boxplots");
    lay->addWidget(plot1box,0,0);


    // 2. Now we create two vectors with random values
    //    vector 1: The values are drawn from two different normal distributions d1 and d2,
    //    where for each datapoint the distribution is chosen randomly (by ddecide)
    //    vector 2: same values as in vector 1, if the value is drawn from d1
    //    vector 3: same values as in vector 1, if the value is drawn from d2
    size_t randomdatacol1=datastore1->addColumn("random data 1");
    size_t randomdatacol2=datastore1->addColumn("random data 2");
    size_t randomdatacol3=datastore1->addColumn("random data 3");
    std::random_device rd; // random number generators:
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> ddecide(0,1);
    std::normal_distribution<> d1{0,1};
    std::normal_distribution<> d2{6,1.2};
    for (size_t i=0; i<150; i++) {
        double v=0;
        const int decide=ddecide(gen);
        if (decide==0) v=d1(gen);
        else v=d2(gen);
        datastore1->appendToColumn(randomdatacol1, v);
        if (decide==0) datastore1->appendToColumn(randomdatacol2, v);
        else datastore1->appendToColumn(randomdatacol3, v);
    }
    QString d1_latex="$\\mathcal{N}("+jkqtp_floattolatexqstr(d1.mean(), 1)+","+jkqtp_floattolatexqstr(d1.stddev(), 1)+")$";
    QString d2_latex="$\\mathcal{N}("+jkqtp_floattolatexqstr(d2.mean(), 1)+","+jkqtp_floattolatexqstr(d2.stddev(), 1)+")$";

    // 3.1. To visualize the data, a simple JKQTPPeakStreamGraph is used:
    JKQTPPeakStreamGraph* gData1;
    plot1box->addGraph(gData1=new JKQTPPeakStreamGraph(plot1box));
    gData1->setDataColumn(randomdatacol1);
    gData1->setBaseline(-0.1);
    gData1->setPeakHeight(-0.05);
    gData1->setDrawBaseline(false);
    // 3.2. We calculate some basic statistics of that column and display it in the graph legend (via the graph title):
    //      Here we use functions of the statistics library for the first time. The statistics library uses an iterator interface
    //      scheme, much like the algorithms of the C++ standard library. Therefore we the iterator interface of JKQTPDatastore
    //      when calling the statistics functions.
    size_t N=0;
    double mean=jkqtpstatAverage(datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), &N);
    double std=jkqtpstatStdDev(datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1));
    gData1->setTitle(QString("random data $"+d1_latex+"+"+d2_latex+"$: $\\overline{X_1}=%1, \\sigma_{X_1}=%2, N_{X_3}=%3$").arg(jkqtp_floattolatexqstr(mean, 2)).arg(jkqtp_floattolatexqstr(std, 2)).arg(N));

    // 3.3. same as 3.1-3.2, but for the second and thirdcolumn of data:
    JKQTPPeakStreamGraph* gData2;
    plot1box->addGraph(gData2=new JKQTPPeakStreamGraph(plot1box));
    gData2->setDataColumn(randomdatacol2);
    gData2->setBaseline(-0.1);
    gData2->setPeakHeight(0.05);
    gData2->setDrawBaseline(false);
    N=0;
    mean=jkqtpstatAverage(datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2), &N);
    std=jkqtpstatStdDev(datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2));
    gData2->setTitle(QString("random data subset $"+d1_latex+"$: $\\overline{X_2}=%1, \\sigma_{X_3}=%2, N_{X_3}=%3$").arg(jkqtp_floattolatexqstr(mean, 2)).arg(jkqtp_floattolatexqstr(std, 2)).arg(N));
    JKQTPPeakStreamGraph* gData3;
    plot1box->addGraph(gData3=new JKQTPPeakStreamGraph(plot1box));
    gData3->setDataColumn(randomdatacol3);
    gData3->setBaseline(-0.15);
    gData3->setPeakHeight(-0.05);
    gData3->setDrawBaseline(false);
    N=0;
    mean=jkqtpstatAverage(datastore1->begin(randomdatacol3), datastore1->end(randomdatacol3), &N);
    std=jkqtpstatStdDev(datastore1->begin(randomdatacol3), datastore1->end(randomdatacol3));
    gData3->setTitle(QString("random data subset $"+d2_latex+"$: $\\overline{X_3}=%1, \\sigma_{X_3}=%2, N_{X_3}=%3$").arg(jkqtp_floattolatexqstr(mean, 2)).arg(jkqtp_floattolatexqstr(std, 2)).arg(N));


    // 3.4. Now we calculate a 5-Value Summary of the two datasets and use it to plot corresponding boxplots
    //      This can be done by hand, or you can call jkqtpstatAddHBoxplot() which saves some typing. This function
    //      uses jkqtpstat5NumberStatistics() internally to calculate the statistics.
    JKQTPBoxplotHorizontalElement* gBox2=jkqtpstatAddHBoxplot(plot1box->getPlotter(), datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2), -0.25);
    gBox2->setColor(gData2->getKeyLabelColor());
    gBox2->setBoxWidthAbsolute(16);
    JKQTPBoxplotHorizontalElement* gBox3=jkqtpstatAddHBoxplot(plot1box->getPlotter(), datastore1->begin(randomdatacol3), datastore1->end(randomdatacol3), -0.35);
    gBox3->setColor(gData3->getKeyLabelColor());
    gBox3->setBoxWidthAbsolute(16);
    // 3.5. In addition to jkqtpstatAddHBoxplot() there is also jkqtpstatAddHBoxplotAndOutliers(), which generates two graphs:
    //      one JKQTPBoxplotHorizontalElement for the boxplot and one JKQTPSingleColumnSymbolsGraph for the outliers
    //      Note that this function generates additional data columns in the datastore of the provided plotter to represent
    //      the outlier locations.
    //      jkqtpstatAddHBoxplotAndOutliers() calculates the 3% and 97% Quantiles for the boxplots whiskers' ends. You can change that
    //      by supplying other quantiles to the call
    std::pair<JKQTPBoxplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*> gBox1=jkqtpstatAddHBoxplotAndOutliers(plot1box->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -0.3);
    //      jkqtpstatAddHBoxplotAndOutliers() calculates the 3% and 97% Quantiles for the boxplots whiskers' ends. You can change that
    //      by supplying other quantiles to the call
    //std::pair<JKQTPBoxplotHorizontalElement*,JKQTPXYLineGraph*> gBox1=jkqtpstatAddHBoxplotAndOutliers(plot1box->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -0.3,
    //                                                                                                  0.25, 0.75, // 1. and 3. Quartile for the boxplot box
    //                                                                                                  0.05, 0.95 // Quantiles for the boxplot box whiskers' ends
    //                                                                                                  );
    gBox1.first->setColor(gData1->getKeyLabelColor());
    gBox1.second->setColor(gData1->getKeyLabelColor());
    gBox1.second->setSymbolType(JKQTPGraphSymbols::JKQTPCircle);
    gBox1.second->setSymbolSize(7);
    gBox1.first->setBoxWidthAbsolute(16);
    //      the simple alternative would have been:
    //JKQTPBoxplotHorizontalElement* gBox1;
    //plot1box->addGraph(gBox1=jkqtpstatAddHBoxplot(plot1box->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1)));
    //gBox1->setPos(-0.3);
    //gBox1->setColor(gData1->getKeyLabelColor());
    //gBox1->setBoxWidthAbsolute(16);




    // 4.1. We repeat the JKQTPPeakStreamGraph visualization from above:
    plot1->addGraph(gData1=new JKQTPPeakStreamGraph(plot1));
    gData1->setDataColumn(randomdatacol1);
    gData1->setBaseline(-0.1);
    gData1->setPeakHeight(-0.05);
    gData1->setDrawBaseline(false);
    gData1->setTitle("random data $"+d1_latex+"+"+d2_latex+"$");

    // 4.2. same as 3.1-3.2, but for the second and thirdcolumn of data:
    plot1->addGraph(gData2=new JKQTPPeakStreamGraph(plot1));
    gData2->setDataColumn(randomdatacol2);
    gData2->setBaseline(-0.1);
    gData2->setPeakHeight(0.05);
    gData2->setDrawBaseline(false);
    gData2->setTitle("random data subset $"+d1_latex+"$");
    plot1->addGraph(gData3=new JKQTPPeakStreamGraph(plot1));
    gData3->setDataColumn(randomdatacol3);
    gData3->setBaseline(-0.15);
    gData3->setPeakHeight(-0.05);
    gData3->setDrawBaseline(false);
    gData3->setTitle("random data subset $"+d2_latex+"$");

    // 4.3. for comparison we add plots of the initial distributions:
    plot1->addGraph(new JKQTPXFunctionLineGraph(std::bind(&jkqtp_gaussdist, std::placeholders::_1, d1.mean(), d1.stddev()), d1_latex, plot1));
    plot1->addGraph(new JKQTPXFunctionLineGraph(std::bind(&jkqtp_gaussdist, std::placeholders::_1, d2.mean(), d2.stddev()), d2_latex, plot1));



    // 4.1. next we calculate a histogram of the data and add a plot to the graph:
    JKQTPBarVerticalGraph* hist1=jkqtpstatAddHHistogram1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 15);
    //      here the bins are defined by the full range of the data and the bin count (15) is specified
    //      alternatively you could specify the bin width and the number would be calculated automatically:
    //JKQTPBarVerticalGraph* hist1=jkqtpstatAddHHistogram1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 0.5);
    //      a third option is to define the bins via a vector of values (lower borders):
    //std::vector<double> bins{-2,-1.5,-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6,7,8,9,10};
    //JKQTPBarVerticalGraph* hist1=jkqtpstatAddHHistogram1D(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), bins.begin(), bins.end());
    hist1->setColor(QColorWithAlphaF(gData1->getKeyLabelColor(), 0.5)); // use same color as gData1, but with alpha set to 0.5 (50% transparency)




    // 5.1. instead of histograms, it can also make sense to calculate Kernel Density Estimates, especially when only few datapoints are available.
    //      To demonstrate this, we first calculate take a subset of the values in randomdatacol1 as a small test dataset.
    size_t randomdatacol1subset=datastore1->copyColumn(randomdatacol1, 1, 7, "subset of "+datastore1->getColumnName(randomdatacol1));
    JKQTPPeakStreamGraph* gData2kde;
    plot1kde->addGraph(gData2kde=new JKQTPPeakStreamGraph(plot1kde));
    gData2kde->setDataColumn(randomdatacol1subset);
    gData2kde->setBaseline(-0.05);
    gData2kde->setPeakHeight(-0.1);
    gData2kde->setDrawBaseline(false);
    gData2kde->setTitle("data");
    //      first we plot the histogram of this dataset, with 0.5 bin width:
    JKQTPBarVerticalGraph* hist1kde=jkqtpstatAddHHistogram1DAutoranged(plot1kde->getPlotter(),  datastore1->begin(randomdatacol1subset), datastore1->end(randomdatacol1subset), 0.5);
    hist1kde->setTitle("histogram");
    hist1kde->setColor(QColorWithAlphaF(gData2kde->getKeyLabelColor(), 0.25)); // use same color as gData1, but with alpha set to 0.5 (50% transparency)
    // 5.2. now we first extimate the bandwidth:
    double kdeBandwidth=jkqtpstatEstimateKDEBandwidth(datastore1->begin(randomdatacol1subset), datastore1->end(randomdatacol1subset));
    //      and generate a vector of positions, where we want to evaluate the KDE:
    std::vector<double> xKDE;
    for (double x=-5; x<=10; x+=0.01) xKDE.push_back(x);
    //      now the KDE can be added (gaussian kernel, evaluated at the positions in xKDE):
    JKQTPXYLineGraph* kde1=jkqtpstatAddHKDE1D(plot1kde->getPlotter(),  datastore1->begin(randomdatacol1subset), datastore1->end(randomdatacol1subset),
    //                                                  evaluate at locations in xKDE
                                                        xKDE.begin(), xKDE.end(),
    //                                                  use a gaussian kernel
                                                        &jkqtpstatKernel1DGaussian,
    //                                                  estimate the bandwidth
                                                        kdeBandwidth);
    kde1->setTitle("KDE, gaussian, $\\mbox{BW}="+jkqtp_floattolatexqstr(kdeBandwidth, 3)+"$");
    JKQTPXYLineGraph* kde11=jkqtpstatAddHKDE1D(plot1kde->getPlotter(),  datastore1->begin(randomdatacol1subset), datastore1->end(randomdatacol1subset),
    //                                                  evaluate at locations in xKDE
                                                        xKDE.begin(), xKDE.end(),
    //                                                  use a gaussian kernel
                                                        &jkqtpstatKernel1DGaussian,
    //                                                  a very small bandwidth
                                                        0.1);
    kde11->setTitle("KDE, gaussian, $\\mbox{BW}="+jkqtp_floattolatexqstr(0.1, 3)+"$");
    //      here a second KDE with a different kernel (Epanechnikov) and the range of evaluation positions defined via three numbers:
    JKQTPXYLineGraph* kde2=jkqtpstatAddHKDE1D(plot1kde->getPlotter(),  datastore1->begin(randomdatacol1subset), datastore1->end(randomdatacol1subset),
    //                                                  evaluate at locations between -5 and 10, in steps of 0.01 (equivalent to the line above, but without pre-calculating a vector)
                                                        -5.0,0.01,10.0,
    //                                                  use a gaussian kernel
                                                        &jkqtpstatKernel1DEpanechnikov,
    //                                                  estimate the bandwidth
                                                        kdeBandwidth);
    kde2->setTitle("KDE, epanechnikov, $\\mbox{BW}="+jkqtp_floattolatexqstr(kdeBandwidth, 3)+"$");
    kde1->setColor(QColorWithAlphaF(gData2kde->getKeyLabelColor(), 0.5)); // use same color as gData1, but with alpha set to 0.5 (50% transparency)
    // 5.3. for comparison we add plots of the initial distributions:
    plot1kde->addGraph(new JKQTPXFunctionLineGraph(std::bind(&jkqtp_gaussdist, std::placeholders::_1, d1.mean(), d1.stddev()), d1_latex, plot1));
    plot1kde->addGraph(new JKQTPXFunctionLineGraph(std::bind(&jkqtp_gaussdist, std::placeholders::_1, d2.mean(), d2.stddev()), d2_latex, plot1));



    // 6.1. now we calculate a cummulative histogram:
    JKQTPPeakStreamGraph* gData2com;
    plot1cum->addGraph(gData2com=new JKQTPPeakStreamGraph(plot1cum));
    gData2com->setDataColumn(randomdatacol2);
    gData2com->setBaseline(-1);
    gData2com->setPeakHeight(-10);
    gData2com->setDrawBaseline(false);
    JKQTPBarVerticalGraph* histcum2=jkqtpstatAddHHistogram1DAutoranged(plot1cum->getPlotter(),  datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2),
    //                                                                          bin width
                                                                                0.1,
    //                                                                          normalized, cummulative
                                                                                false,       true);
    histcum2->setColor(QColorWithAlphaF(gData2com->getKeyLabelColor(), 0.2)); // use same color as gData1, but with alpha set to 0.5 (50% transparency)
    // 6.2. also a kernel density estimate can be accumulated:
    JKQTPXYLineGraph* kdecum2=jkqtpstatAddHKDE1D(plot1cum->getPlotter(),  datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2),
    //                                                  evaluate at locations between -3.5 and 3.5, in steps of 0.01
                                                        -3.5,0.01,3.5,
    //                                                  use a uniform/box kernel
                                                        &jkqtpstatKernel1DUniform,
    //                                                  estimate the bandwidth
                                                        jkqtpstatEstimateKDEBandwidth(datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2)),
    //                                                  cummulative KDE:
                                                        true);
    kdecum2->setColor(gData2com->getKeyLabelColor()); // use same color as gData1, but with alpha set to 0.5 (50% transparency)


    // autoscale the plot so the graph is contained
    plot1->zoomToFit();
    plot1->setGrid(false);
    plot1->getXAxis()->setShowZeroAxis(false);
    plot1->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plot1->setY(-0.25, 0.45);
    plot1cum->zoomToFit();
    plot1cum->setGrid(false);
    plot1cum->getXAxis()->setShowZeroAxis(false);
    plot1cum->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plot1kde->zoomToFit();
    plot1kde->setGrid(false);
    plot1kde->getXAxis()->setShowZeroAxis(false);
    plot1kde->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plot1kde->setY(-0.155, 0.45);
    plot1box->zoomToFit();
    plot1box->setGrid(false);
    plot1box->getXAxis()->setShowZeroAxis(false);
    plot1box->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plot1box->setY(-0.4, 0.0);

    // show plotter and make it a decent size
    mainWidget.show();
    mainWidget.resize(1200,800);

    return app.exec();
}
