/** \example violinplot.cpp
 * Shows how to draw Violin-Plot with JKQTPlotter
 *
 * \ref JKQTPlotterViolinplotGraphs
 */

#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpviolinplot.h"
#include "jkqtplotter/graphs/jkqtpsinglecolumnsymbols.h"
#include "jkqtplotter/graphs/jkqtpstatisticsadaptors.h"
#include "jkqtmath/jkqtpstatisticstools.h"
#include <random>
#include <cmath>

template <class TGraph, JKQTPSingleColumnSymbolsGraph::DataDirection DataOrientation>
JKQTPlotter* showPlot(const QString& title) {

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter* plot=new JKQTPlotter();
    plot->getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot->getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot->getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* datastore1=plot->getDatastore();


    // 2. Now we create random data drawn from a bimodal distribution,
    //    also we add each single distribution as a separate column
    size_t randomdatacol1=datastore1->addColumn("random data N(1,1)+N(6,2)");
    size_t randomdatacol2=datastore1->addColumn("random data N(1,1)");
    size_t randomdatacol3=datastore1->addColumn("random data N(6,2)");
    std::random_device rd; // random number generators:
    std::mt19937 gen{rd()};
    gen.seed(1236);
    std::uniform_int_distribution<> ddecide(0,1);
    std::normal_distribution<> d1{1,1};
    std::normal_distribution<> d2{6,2};
    for (size_t i=0; i<40; i++) {
        double v=0;
        if (i%2==0) {
            v=d1(gen);
            datastore1->appendToColumn(randomdatacol2, v);
        } else {
            v=d2(gen);
            datastore1->appendToColumn(randomdatacol3, v);
        }
        datastore1->appendToColumn(randomdatacol1, v);
    }
    const QString d1_latex="$\\mathcal{N}_{"+jkqtp_floattolatexqstr(d1.mean(), 0, true)+","+jkqtp_floattolatexqstr(d1.stddev(), 0, true)+"}+\\mathcal{N}_{"+jkqtp_floattolatexqstr(d2.mean(), 0, true)+","+jkqtp_floattolatexqstr(d2.stddev(), 0, true)+"}$";
    const QString d2_latex="$\\mathcal{N}_{"+jkqtp_floattolatexqstr(d1.mean(), 0, true)+","+jkqtp_floattolatexqstr(d1.stddev(), 0, true)+"}$";
    const QString d3_latex="$\\mathcal{N}_{"+jkqtp_floattolatexqstr(d2.mean(), 0, true)+","+jkqtp_floattolatexqstr(d2.stddev(), 0, true)+"}$";


    // 3. we visualize the data as scatter plots
    JKQTPSingleColumnSymbolsGraph* gData1;
    plot->addGraph(gData1=new JKQTPSingleColumnSymbolsGraph(plot));
    gData1->setPosition(0);
    gData1->setWidth(0.5);
    gData1->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::RugPlot);
    gData1->setDataColumn(randomdatacol1);
    gData1->setTitle(d3_latex);
    gData1->setDataDirection(DataOrientation);

    JKQTPSingleColumnSymbolsGraph* gData2;
    plot->addGraph(gData2=new JKQTPSingleColumnSymbolsGraph(plot));
    gData2->setPosition(7.5);
    gData2->setWidth(0.5);
    gData2->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::RugPlot);
    gData2->setDataColumn(randomdatacol2);
    gData2->setTitle(d3_latex);
    gData2->setDataDirection(DataOrientation);

    JKQTPSingleColumnSymbolsGraph* gData3;
    plot->addGraph(gData3=new JKQTPSingleColumnSymbolsGraph(plot));
    gData3->setPosition(15);
    gData3->setWidth(0.5);
    gData3->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::RugPlot);
    gData3->setDataColumn(randomdatacol3);
    gData3->setTitle(d3_latex);
    gData3->setDataDirection(DataOrientation);

    auto pos=JKQTPGraphViolinplotStyleMixin::ViolinBoth;

    // 4. In order to draw violin plots we first need to calculate the
    //    kernel density estimate (KDE) for the violin
    size_t cViol1Cat=datastore1->addColumn("violin 1, cat");
    size_t cViol1Freq=datastore1->addColumn("violin 1, KDE");
    jkqtpstatKDE1DAutoranged(datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1),
                             datastore1->backInserter(cViol1Cat), datastore1->backInserter(cViol1Freq),
                             51, jkqtpstatKernel1DEpanechnikov,
                             jkqtpstatEstimateKDEBandwidth(datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1)));
    TGraph* gViol1;
    plot->addGraph(gViol1=new TGraph(plot));
    gViol1->setPos(7.5/2);
    gViol1->setMin(jkqtpstatMinimum(datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1)));
    gViol1->setMax(jkqtpstatMaximum(datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1)));
    gViol1->setMean(jkqtpstatAverage(datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1)));
    gViol1->setMedian(jkqtpstatMedian(datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1)));
    gViol1->setViolinPositionColumn(cViol1Cat);
    gViol1->setViolinFrequencyColumn(cViol1Freq);
    gViol1->setColor(gData1->getSymbolColor());
    gViol1->setViolinStyle(JKQTPGraphViolinplotStyleMixin::SmoothViolin);
    gViol1->setViolinPositionMode(pos);
    gViol1->setTitle("violin "+d1_latex);

    size_t cViol2Cat=datastore1->addColumn("violin 2, cat");
    size_t cViol2Freq=datastore1->addColumn("violin 2, Histogram");
    jkqtpstatHistogram1DAutoranged(datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2),
                             datastore1->backInserter(cViol2Cat), datastore1->backInserter(cViol2Freq),
                             11, true, false, JKQTPStatHistogramBinXMode::XIsMid);
    TGraph* gViol2;
    plot->addGraph(gViol2=new TGraph(plot));
    gViol2->setPos(7.5*1.5);
    gViol2->setMin(jkqtpstatMinimum(datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2)));
    gViol2->setMax(jkqtpstatMaximum(datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2)));
    gViol2->setMean(jkqtpstatAverage(datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2)));
    gViol2->setMedian(jkqtpstatMedian(datastore1->begin(randomdatacol2), datastore1->end(randomdatacol2)));
    gViol2->setViolinPositionColumn(cViol2Cat);
    gViol2->setViolinFrequencyColumn(cViol2Freq);
    gViol2->setColor(gData2->getSymbolColor());
    gViol2->setViolinStyle(JKQTPGraphViolinplotStyleMixin::StepViolin);
    gViol2->setViolinPositionMode(pos);
    gViol2->setTitle("step violin "+d2_latex);

    size_t cViol3Cat=datastore1->addColumn("violin 3, cat");
    size_t cViol3Freq=datastore1->addColumn("violin 3, Histogram");
    jkqtpstatHistogram1DAutoranged(datastore1->begin(randomdatacol3), datastore1->end(randomdatacol3),
                             datastore1->backInserter(cViol3Cat), datastore1->backInserter(cViol3Freq),
                             11, true, false, JKQTPStatHistogramBinXMode::XIsMid);
    TGraph* gViol3;
    plot->addGraph(gViol3=new TGraph(plot));
    gViol3->setPos(7.5*2.5);
    gViol3->setMin(jkqtpstatMinimum(datastore1->begin(randomdatacol3), datastore1->end(randomdatacol3)));
    gViol3->setMax(jkqtpstatMaximum(datastore1->begin(randomdatacol3), datastore1->end(randomdatacol3)));
    gViol3->setMean(jkqtpstatAverage(datastore1->begin(randomdatacol3), datastore1->end(randomdatacol3)));
    gViol3->setMedian(jkqtpstatMedian(datastore1->begin(randomdatacol3), datastore1->end(randomdatacol3)));
    gViol3->setViolinPositionColumn(cViol3Cat);
    gViol3->setViolinFrequencyColumn(cViol3Freq);
    gViol3->setColor(gData3->getSymbolColor());
    gViol3->setViolinStyle(JKQTPGraphViolinplotStyleMixin::BoxViolin);
    gViol3->setViolinPositionMode(pos);
    gViol3->setTitle("box violin "+d3_latex);

    // Note that there also exist "adapters" that allow to draw violin plots in one line of code:
    /*jkqtpstatAddVViolinplotHistogramAndOutliers(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -5);
    jkqtpstatAddHViolinplotHistogramAndOutliers(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -5);
    jkqtpstatAddVViolinplotHistogram(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -10);
    jkqtpstatAddHViolinplotHistogram(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -10);
    jkqtpstatAddVViolinplotKDEAndOutliers(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -15);
    jkqtpstatAddHViolinplotKDEAndOutliers(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -15);
    jkqtpstatAddVViolinplotKDE(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -20);
    jkqtpstatAddHViolinplotKDE(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -20);
    */

    // autoscale and style the plot
    plot->zoomToFit();
    plot->getPlotter()->setKeyPosition(JKQTPKeyInsideBottomRight);
    plot->setShowZeroAxes(false);
    plot->getPlotter()->setGridColor(QColor(230,230,230));

    // show plotter and make it a decent size
    plot->setWindowTitle(title);
    plot->show();
    plot->resize(500,450);

    return plot;
}

int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    JKQTPlotter* plot1=showPlot<JKQTPViolinplotHorizontalElement,JKQTPSingleColumnSymbolsGraph::DataDirection::X>("1: JKQTPViolinplotHorizontalElement");
    //plot1->resize(500,450);
    plot1->getPlotter()->setKeyPosition(JKQTPKeyInsideTopLeft);
    JKQTPlotter* plot2=showPlot<JKQTPViolinplotVerticalElement,JKQTPSingleColumnSymbolsGraph::DataDirection::Y>("2: JKQTPViolinplotVerticalElement");


    return app.exec();
}
