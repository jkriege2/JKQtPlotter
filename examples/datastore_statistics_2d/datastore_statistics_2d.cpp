/** \example datastore_statistics_2d.cpp
 * Explains how to use the internal statistics library (see \ref jkqtptools_statistics ) together with JKQTPDatastore to generate advanced plots for 2-dimensional data.
 *
 * \ref JKQTPlotterBasicJKQTPDatastoreStatistics2D
 */
 
#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/graphs/jkqtpstatisticsadaptors.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtmath/jkqtpstatisticstools.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <random>
#include <cmath>


int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);

    QElapsedTimer timer;


    // 1. create a window with several plotters and get a pointer to the internal datastores (for convenience)
    QWidget mainWidget;
    QGridLayout* lay;
    mainWidget.setLayout(lay=new QGridLayout);
    JKQTPlotter* plothist=new JKQTPlotter(&mainWidget);
    lay->addWidget(plothist,0,1);
    plothist->getPlotter()->setPlotLabel("Histograms");
    JKQTPDatastore* datastore1=plothist->getDatastore();
    JKQTPlotter* plothistLeft=new JKQTPlotter(datastore1, &mainWidget);
    lay->addWidget(plothistLeft,0,0);
    JKQTPlotter* plothistBottom=new JKQTPlotter(datastore1, &mainWidget);
    lay->addWidget(plothistBottom,1,1);
    plothistLeft->synchronizeYToMaster(plothist);
    plothistLeft->setAbsoluteX(0,1);
    plothistLeft->getXAxis()->setInverted(true);
    plothistLeft->getXAxis()->setShowZeroAxis(false);
    plothistLeft->getYAxis()->setShowZeroAxis(false);
    plothistLeft->getYAxis()->setDrawMode1(JKQTPCADMLine);
    plothistLeft->getYAxis()->setDrawMode2(JKQTPCADMLineTicks);
    plothistLeft->setGrid(false);
    plothistBottom->synchronizeXToMaster(plothist);
    plothistBottom->setAbsoluteY(0,1);
    plothistBottom->getYAxis()->setInverted(true);
    plothistBottom->getXAxis()->setShowZeroAxis(false);
    plothistBottom->getYAxis()->setShowZeroAxis(false);
    plothistBottom->getXAxis()->setDrawMode1(JKQTPCADMLine);
    plothistBottom->getXAxis()->setDrawMode2(JKQTPCADMLineTicks);
    plothistBottom->setGrid(false);
    plothistBottom->setMousePositionShown(false);


    JKQTPlotter* plotkde=new JKQTPlotter(datastore1, &mainWidget);
    plotkde->getPlotter()->setPlotLabel("Kernel Density Estimate");
    lay->addWidget(plotkde,0,2);
    JKQTPlotter* plotkdeRight=new JKQTPlotter(datastore1, &mainWidget);
    lay->addWidget(plotkdeRight,0,3);
    JKQTPlotter* plotkdeBottom=new JKQTPlotter(datastore1, &mainWidget);
    lay->addWidget(plotkdeBottom,1,2);
    plotkdeRight->synchronizeYToMaster(plotkde);
    plotkdeRight->setAbsoluteX(0,1);
    plotkdeRight->getXAxis()->setShowZeroAxis(false);
    plotkdeRight->getYAxis()->setShowZeroAxis(false);
    plotkdeRight->getYAxis()->setDrawMode1(JKQTPCADMLine);
    plotkdeRight->getYAxis()->setDrawMode2(JKQTPCADMLineTicks);
    plotkdeRight->setGrid(false);
    plotkdeBottom->synchronizeXToMaster(plotkde);
    plotkdeBottom->setAbsoluteY(0,1);
    plotkdeBottom->getYAxis()->setInverted(true);
    plotkdeBottom->getXAxis()->setShowZeroAxis(false);
    plotkdeBottom->getYAxis()->setShowZeroAxis(false);
    plotkdeBottom->getXAxis()->setDrawMode1(JKQTPCADMLine);
    plotkdeBottom->getXAxis()->setDrawMode2(JKQTPCADMLineTicks);
    plotkdeBottom->setGrid(false);
    plotkdeBottom->setMousePositionShown(false);

    lay->setColumnStretch(0,1);
    lay->setColumnStretch(1,3);
    lay->setColumnStretch(2,3);
    lay->setColumnStretch(3,1);
    lay->setRowStretch(0,3);
    lay->setRowStretch(1,1);


    // 2.1. Now we create two vectors with random values
    //    randomdatacolx: random x-positions, drawn from one of two gaussian distributions
    //    randomdatacoly: random y-positions, drawn from one of two gaussian distributions
    //    randomdatacoldist: indicates, which if the two sets of gaussian distributions was chosen for each datapoint
    size_t randomdatacolx_small=datastore1->addColumn("random data, x");
    size_t randomdatacoly_small=datastore1->addColumn("random data, y");
    size_t randomdatacoldist_small=datastore1->addColumn("random data, distribution/class");
    size_t randomdatacolx=datastore1->addColumn("random data, x");
    size_t randomdatacoly=datastore1->addColumn("random data, y");
    size_t randomdatacoldist=datastore1->addColumn("random data, distribution/class");
    // random number generators:
    std::random_device rd;
    std::mt19937 gen{rd()};
    gen.seed(12345);
    std::uniform_int_distribution<> ddecide(0,2);
    std::normal_distribution<> d1x{5,3};
    std::normal_distribution<> d1y{5,1};
    std::normal_distribution<> d2x{10,2};
    std::normal_distribution<> d2y{10,5};
    for (size_t i=0; i<500; i++) {
        double rx=0,ry=0;
        const int decide=ddecide(gen);
        if (decide==0) {
            rx=d1x(gen);
            ry=d1y(gen);
        } else {
            rx=d2x(gen);
            ry=d2y(gen);
        }
        if (i<150) {
            datastore1->appendToColumn(randomdatacolx_small, rx);
            datastore1->appendToColumn(randomdatacoly_small, ry);
            datastore1->appendToColumn(randomdatacoldist_small, std::min(1,decide));
        }
        datastore1->appendToColumn(randomdatacolx, rx);
        datastore1->appendToColumn(randomdatacoly, ry);
        datastore1->appendToColumn(randomdatacoldist, std::min(1,decide));
    }

    // 2.2. To visualize the data, a simple JKQTPPeakStreamGraph is used:
    JKQTPXYParametrizedScatterGraph* gDataHist;
    plothist->addGraph(gDataHist=new JKQTPXYParametrizedScatterGraph(plothist));
    gDataHist->setXYColumns(randomdatacolx,randomdatacoly);
    gDataHist->setSymbolColumn(randomdatacoldist);
    QMap<double, JKQTPGraphSymbols> mapped;
    mapped[0]=JKQTPGraphSymbols::JKQTPCross;
    mapped[1]=JKQTPGraphSymbols::JKQTPPlus;
    gDataHist->setMappedSymbolColumnFunctor(mapped);
    gDataHist->setSymbolSize(5);
    gDataHist->setSymbolColor(QColorWithAlphaF(QColor("red"), 0.7));
    gDataHist->setDrawLine(false);
    gDataHist->setTitle(QString("random data, $N="+QString::number(datastore1->getRows(randomdatacoldist))+"$"));
    JKQTPXYParametrizedScatterGraph* gDataKDE;
    plotkde->addGraph(gDataKDE=new JKQTPXYParametrizedScatterGraph(plotkde));
    gDataKDE->setXYColumns(randomdatacolx_small,randomdatacoly_small);
    gDataKDE->setSymbolColumn(randomdatacoldist_small);
    gDataKDE->setSymbolSize(3);
    gDataKDE->setSymbolColor(QColorWithAlphaF(QColor("red"), 0.7));
    gDataKDE->setMappedSymbolColumnFunctor(mapped);
    gDataKDE->setDrawLine(false);
    gDataKDE->setTitle(QString("random data, $N="+QString::number(datastore1->getRows(randomdatacoldist_small))+"$"));

    // 2.3. to visualize the initial distributions, we draw an ellipse indicating the
    //      variance of the distributions
    JKQTPGeoEllipse* gEll1Hist;
    JKQTPGeoEllipse* gEll2Hist;
    JKQTPGeoEllipse* gEll1KDE;
    JKQTPGeoEllipse* gEll2KDE;
    plothist->addGraph(gEll1Hist=new JKQTPGeoEllipse(plothist, d1x.mean(), d1y.mean(),d1x.stddev()*2.0,d1y.stddev()*2.0));
    plothist->addGraph(gEll2Hist=new JKQTPGeoEllipse(plothist, d2x.mean(), d2y.mean(),d2x.stddev()*2.0,d2y.stddev()*2.0));
    plotkde->addGraph(gEll1KDE=new JKQTPGeoEllipse(plothist, d1x.mean(), d1y.mean(),d1x.stddev()*2.0,d1y.stddev()*2.0));
    plotkde->addGraph(gEll2KDE=new JKQTPGeoEllipse(plothist, d2x.mean(), d2y.mean(),d2x.stddev()*2.0,d2y.stddev()*2.0));


    // 3. Marginal (1D) Statistics of the x-position and y-position deistributions:
    // 3.1. First we calculate the x/y marginal histograms, as desribed in https://github.com/jkriege2/JKQtPlotter/tree/master/examples/datastore_statistics
    timer.start();
    jkqtpstatAddHHistogram1DAutoranged(plothistBottom->getPlotter(),  datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), 1.0, true);
    jkqtpstatAddVHistogram1DAutoranged(plothistLeft->getPlotter(),  datastore1->begin(randomdatacoly), datastore1->end(randomdatacoly), 1.0, true);
    qDebug()<<"histogram, 1D: "<<timer.elapsed()/2.0<<"ms";
    // 3.2. Also we calculate the x/y marginal kernel density estimates, as desribed in https://github.com/jkriege2/JKQtPlotter/tree/master/examples/datastore_statistics
    timer.start();
    double bwx=jkqtpstatEstimateKDEBandwidth(datastore1->begin(randomdatacolx_small), datastore1->end(randomdatacolx_small));
    qDebug()<<bwx;
    jkqtpstatAddHKDE1DAutoranged(plotkdeBottom->getPlotter(),  datastore1->begin(randomdatacolx_small), datastore1->end(randomdatacolx_small), 0.01, &jkqtpstatKernel1DGaussian, bwx);
    double bwy=jkqtpstatEstimateKDEBandwidth(datastore1->begin(randomdatacoly_small), datastore1->end(randomdatacoly_small));
    qDebug()<<bwy;
    jkqtpstatAddVKDE1DAutoranged(plotkdeRight->getPlotter(),  datastore1->begin(randomdatacoly_small), datastore1->end(randomdatacoly_small), 0.01, &jkqtpstatKernel1DGaussian, bwy);
    qDebug()<<"KDE+bandwidth, 1D: "<<timer.elapsed()/2.0<<"ms";


    // 4. 2D Histogram
    double xmin=0, xmax=0;
    double ymin=0, ymax=0;
    timer.start();
    jkqtpstatMinMax(datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), xmin,xmax);
    jkqtpstatMinMax(datastore1->begin(randomdatacoly), datastore1->end(randomdatacoly), ymin,ymax);
    size_t Nx=jkqtp_ceilTo<size_t>((xmax-xmin)/2.0);
    size_t Ny=jkqtp_ceilTo<size_t>((ymax-ymin)/2.0);
    size_t histcol=datastore1->addImageColumn(Nx, Ny, "2d histogram");
    jkqtpstatHistogram2D(datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), datastore1->begin(randomdatacoly), datastore1->end(randomdatacoly),
                         datastore1->begin(histcol),
                         xmin, xmax, ymin, ymax,
                         Nx, Ny, true);
    qDebug()<<"histogram, 2D: "<<timer.elapsed()<<"ms";
    JKQTPColumnMathImage* gHist;
    plothist->addGraph(gHist=new JKQTPColumnMathImage(plothist));
    gHist->setImageColumn(static_cast<int>(histcol));
    gHist->setX(xmin);
    gHist->setY(ymin);
    gHist->setWidth(xmax-xmin);
    gHist->setHeight(ymax-ymin);
    gHist->setTitle("2D Histogram");
    qDebug()<<xmin<<xmax<<Nx;
    qDebug()<<ymin<<ymax<<Ny;

    //   There also exist "adaptors", which execute the complete code above in one call.
    //   Two flavors exist:
    //      jkqtpstatAddHistogram2DImage() adds an image plot as shown above
    //      jkqtpstatAddHistogram2DContour() adds a contour plot
    //jkqtpstatAddHistogram2DImage(plothist->getPlotter(), datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), datastore1->begin(randomdatacoly), datastore1->end(randomdatacoly), Nx, Ny, true);
    //jkqtpstatAddHistogram2DContour(plothist->getPlotter(), datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), datastore1->begin(randomdatacoly), datastore1->end(randomdatacoly), size_t(50),size_t(50), true);



    // 5. 2D KDE
    xmin=0; xmax=0;
    ymin=0; ymax=0;
    timer.start();
    jkqtpstatMinMax(datastore1->begin(randomdatacolx_small), datastore1->end(randomdatacolx_small), xmin,xmax);
    jkqtpstatMinMax(datastore1->begin(randomdatacoly_small), datastore1->end(randomdatacoly_small), ymin,ymax);
    Nx=jkqtp_ceilTo<size_t>((xmax-xmin)/0.1);
    Ny=jkqtp_ceilTo<size_t>((ymax-ymin)/0.1);
    bwx=jkqtpstatEstimateKDEBandwidth2D(datastore1->begin(randomdatacolx_small), datastore1->end(randomdatacolx_small));
    qDebug()<<bwx;
    bwy=jkqtpstatEstimateKDEBandwidth2D(datastore1->begin(randomdatacoly_small), datastore1->end(randomdatacoly_small));
    qDebug()<<bwy;
    qDebug()<<xmin<<xmax<<Nx;
    qDebug()<<ymin<<ymax<<Ny;
    size_t kdecol=datastore1->addImageColumn(Nx, Ny, "2d KDE");
    jkqtpstatKDE2D(datastore1->begin(randomdatacolx_small), datastore1->end(randomdatacolx_small), datastore1->begin(randomdatacoly_small), datastore1->end(randomdatacoly_small),
                         datastore1->begin(kdecol),
                         xmin, xmax, ymin, ymax, Nx, Ny,
                         &jkqtpstatKernel2DGaussian, bwx, bwy);
    qDebug()<<"KDE, 2D: "<<timer.elapsed()<<"ms";
    JKQTPColumnMathImage* gKDE;
    plotkde->addGraph(gKDE=new JKQTPColumnMathImage(plotkde));
    gKDE->setImageColumn(static_cast<int>(kdecol));
    gKDE->setX(xmin);
    gKDE->setY(ymin);
    gKDE->setWidth(xmax-xmin);
    gKDE->setHeight(ymax-ymin);
    gKDE->setTitle("2D KDE");

    //   There also exist "adaptors", which execute the complete code above in one call.
    //   Two flavors exist:
    //      jkqtpstatAddKDE2DImage() adds an image plot as shown above
    //      jkqtpstatAddKDE2DContour() adds a contour plot
    //jkqtpstatAddKDE2DImage(plotkde->getPlotter(), datastore1->begin(randomdatacolx_small), datastore1->end(randomdatacolx_small), datastore1->begin(randomdatacoly_small), datastore1->end(randomdatacoly_small), Nx, Ny, &jkqtpstatKernel2DGaussian, bwx, bwy);
    //jkqtpstatAddKDE2DContour(plotkde->getPlotter(), datastore1->begin(randomdatacolx_small), datastore1->end(randomdatacolx_small), datastore1->begin(randomdatacoly_small), datastore1->end(randomdatacoly_small), Nx, Ny, &jkqtpstatKernel2DGaussian, bwx, bwy);



    // autoscale the plot so the graph is contained
    plothist->zoomToFit();
    plothist->setGrid(false);
    plothist->getXAxis()->setShowZeroAxis(false);
    plothist->getYAxis()->setShowZeroAxis(false);
    plothist->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plothist->getPlotter()->moveGraphTop(gDataHist);
    plothist->getPlotter()->moveGraphTop(gEll1Hist);
    plothist->getPlotter()->moveGraphTop(gEll2Hist);
    plothistBottom->zoomToFit(false, true);
    plothistLeft->zoomToFit(true, false);
    plotkde->zoomToFit();
    plotkde->setGrid(false);
    plotkde->getXAxis()->setShowZeroAxis(false);
    plotkde->getYAxis()->setShowZeroAxis(false);
    plotkde->getPlotter()->setKeyBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plotkde->getPlotter()->moveGraphTop(gDataKDE);
    plotkde->getPlotter()->moveGraphTop(gEll1KDE);
    plotkde->getPlotter()->moveGraphTop(gEll2KDE);
    plotkdeBottom->zoomToFit(false, true);
    plotkdeRight->zoomToFit(true, false);

    // show plotter and make it a decent size
    mainWidget.show();
    mainWidget.resize(1200,600);

    return app.exec();
}
