#ifndef BUILD_TEST_GRAPHS_H
#define BUILD_TEST_GRAPHS_H

#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"
#include "jkqtplotter/graphs/jkqtpgeoannotations.h"
#include "jkqtplotter/graphs/jkqtpboxplot.h"
#include "jkqtplotter/graphs/jkqtpfilledcurve.h"
#include "jkqtplotter/graphs/jkqtpimpulses.h"
#include "jkqtplotter/jkqtpbaseplotter.h"

#define DEFAULT_PLOT_SIZE QSize(4,2)

inline void setupGridPrintingAndDatastores(QVector<JKQTBasePlotter*>& plots, QSize size) {
    // connect plots
    for  (auto plt: plots) {

        for (int x=0; x<size.width(); x++) {
            for (int y=0; y<size.height(); y++) {
                plt->addGridPrintingPlotter(x,y, plots[y*size.width()+x]);
            }
        }
        plt->setGridPrinting(true);

        // use same datastore for each plot
        if (plt!=plots.first()) plt->useExternalDatastore(plots.first()->getDatastore());
    }
}

inline void buildPlots(QVector<JKQTBasePlotter*>& plots) {

    // 0. get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plots.first()->getDatastore();

    // 1. clear old graphs/data
    for (auto plt: plots) {
        plt->clearGraphs(true);
    }
    ds->clear();

    // 2. now we create data for a simple plot (a sine curve)
    QVector<double> X, Y1, Y2, img, X3, Y3, Y3err, Xbar, Ybar, Ybar2, Ybar3, Y4, Y5, XImp, YImp, XBP, MinBP, Q25BP, MeanBP, MedianBP, Q75BP, MaxBP;
    const int Ndata=50;
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
        X<<x;
        Y1<<3.1+sin(x)+x/25.0*2.0;
        Y2<<3.1+1.5*sin(x)*exp(-0.2*x)+x/25.0*2.0;
        Y4<<4.5*exp(-x/10.0);
        Y5<<2.0*exp(-x/10.0)+0.8;
        if (i%5==0) {
            X3<<x;
            Y3<<(4.5)+sin(x/25.0*JKQTPSTATISTICS_PI)*2.0-x/12.5;
            Y3err<<(double(i+5)/double(Ndata)*0.5);
        }
        if (i>0 && i%16==0) {
            Xbar<<x;
            Ybar<<(double(i+5)/double(Ndata)*(1.5));
            Ybar2<<(double(i+5)/double(Ndata)*(1.2));
            Ybar3<<(3.0-(double(i+5)/double(Ndata)*(3.7)));
        }
        if (i%8==0) {
            XImp<<1+x/1.2;
            YImp<<1.5+1.1*cos(x/2.0);
        }
        if (i<=5) {
            XBP<<(i-1);
            MinBP<<(i+1);
            Q25BP<<(i*1.1+3.0);
            MeanBP<<(i*1.12+4.0);
            MedianBP<<(i*1.2+3.5);
            Q75BP<<(i*1.3+6.0);
            MaxBP<<(i*1.35+7.5);
        }
    }
    auto fgauss=[](double x, double y, double x0, double y0, double sx, double sy) {
        return exp(-2.0*(x-x0)*(x-x0)/sx/sx-2.0*(y-y0)*(y-y0)/sy/sy);
    };
    for (int i=0; i<Ndata; i++) {
        for (int j=0; j<Ndata; j++) {
            img.push_back(fgauss(j,i,Ndata/3,Ndata/3,Ndata/5,Ndata/3)+fgauss(j,i,Ndata*3/4,Ndata*2/3,Ndata/2,Ndata/4));
        }
    }

    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    //    Note: In this step the data is copied (of not specified otherwise), so you can
    //          reuse X and Y afterwards!
    //    the variables columnX and columnY will contain the internal column ID of the newly
    //    created columns with names "x" and "y" and the (copied) data from X and Y.
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY1=ds->addCopiedColumn(Y1, "y1");
    size_t columnY2=ds->addCopiedColumn(Y2, "y2");
    size_t columnImg=ds->addCopiedColumn(img, "img");
    size_t columnX3=ds->addCopiedColumn(X3, "x3");
    size_t columnY3=ds->addCopiedColumn(Y3, "y3");
    size_t columnY3err=ds->addCopiedColumn(Y3err, "y3err");
    size_t columnXbar=ds->addCopiedColumn(Xbar, "xbar");
    size_t columnYbar=ds->addCopiedColumn(Ybar, "ybar");
    size_t columnYbar2=ds->addCopiedColumn(Ybar2, "Ybar2");
    size_t columnYbar3=ds->addCopiedColumn(Ybar3, "Ybar3");
    size_t columnY4=ds->addCopiedColumn(Y4, "y4");
    size_t columnY5=ds->addCopiedColumn(Y5, "y5");
    size_t columnXImp=ds->addCopiedColumn(XImp, "ximp");
    size_t columnYImp=ds->addCopiedColumn(YImp, "yimp");

    size_t columnXBP=ds->addCopiedColumn(XBP, "XBP");
    size_t columnMinBP=ds->addCopiedColumn(MinBP, "MinBP");
    size_t columnQ25BP=ds->addCopiedColumn(Q25BP, "Q25BP");
    size_t columnMeanBP=ds->addCopiedColumn(MeanBP, "MeanBP");
    size_t columnMedianBP=ds->addCopiedColumn(MedianBP, "MedianBP");
    size_t columnQ75BP=ds->addCopiedColumn(Q75BP, "Q75BP");
    size_t columnMaxBP=ds->addCopiedColumn(MaxBP, "MaxBP");

    // 4. create diverse graphs in the plot:
    JKQTPFilledCurveXGraph* graphf4=new JKQTPFilledCurveXGraph(plots[2]);
    graphf4->setXColumn(columnX);
    graphf4->setYColumn(columnY4);
    graphf4->setTitle(QObject::tr("filled exp-function"));
    plots[2]->addGraph(graphf4);

    JKQTPFilledCurveXGraph* graphf5=new JKQTPFilledCurveXGraph(plots[2]);
    graphf5->setXColumn(columnX);
    graphf5->setYColumn(columnY5);
    graphf5->setTitle(QObject::tr("other filled exp-function"));
    plots[2]->addGraph(graphf5);


    JKQTPColumnMathImage* graphI=new JKQTPColumnMathImage(plots.last());
    graphI->setImageColumn(columnImg);
    graphI->setNx(Ndata);
    graphI->setNy(Ndata);
    graphI->setColorBarTopVisible(true);
    graphI->setColorBarRightVisible(true);
    graphI->setX(0.5);
    graphI->setY(-2.0);
    graphI->setWidth(5);
    graphI->setHeight(1);
    graphI->setTitle(QObject::tr("2{\\times}Gauss"));
    plots.last()->addGraph(graphI);

    JKQTPXYLineErrorGraph* graph1=new JKQTPXYLineErrorGraph(plots[0]);
    graph1->setXColumn(columnX3);
    graph1->setYColumn(columnY3);
    graph1->setYErrorColumn(columnY3err);
    graph1->setDrawLine(true);
    graph1->setYErrorStyle(JKQTPErrorBarsLines);
    graph1->setSymbolType(JKQTPNoSymbol);
    graph1->setTitle(QObject::tr("sine \\pm errors graph"));
    plots[0]->addGraph(graph1);

    JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(plots[0]);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY1);
    graph2->setSymbolType(JKQTPNoSymbol);
    graph2->setTitle(QObject::tr("sine graph $f(x)=\\sin(x)$"));
    plots[0]->addGraph(graph2);

    JKQTPXYLineGraph* graph3=new JKQTPXYLineGraph(plots[0]);
    graph3->setXColumn(columnX);
    graph3->setYColumn(columnY2);
    graph3->setSymbolType(JKQTPNoSymbol);
    //graph2->setTitle(QObject::tr("sine{\\cdot}exp graph $f(x)=\\sin(x)\\cdot\\exp\\left(-\\frac{x}{5}\\right)$"));
    graph3->setTitle(QObject::tr("sine*exp graph"));
    plots[0]->addGraph(graph3);


    JKQTPImpulsesVerticalGraph* graphi2=new JKQTPImpulsesVerticalGraph(plots[0]);
    graphi2->setXColumn(columnXImp);
    graphi2->setYColumn(columnYImp);
    graphi2->setTitle(QObject::tr("impulses"));
    plots[0]->addGraph(graphi2);



    JKQTPXYLineErrorGraph* graph1s=new JKQTPXYLineErrorGraph(plots[1]);
    graph1s->setXColumn(columnX3);
    graph1s->setYColumn(columnY3);
    graph1s->setYErrorColumn(columnY3err);
    graph1s->setDrawLine(false);
    graph1s->setYErrorStyle(JKQTPErrorBars);
    graph1s->setTitle(QObject::tr("sine \\pm errors graph"));
    plots[1]->addGraph(graph1s);

    JKQTPXYScatterGraph* graph2s=new JKQTPXYScatterGraph(plots[1]);
    graph2s->setXColumn(columnX);
    graph2s->setYColumn(columnY1);
    graph2s->setTitle(QObject::tr("sine graph $f(x)=\\sin(x)$"));
    plots[1]->addGraph(graph2s);

    JKQTPXYScatterGraph* graph3s=new JKQTPXYScatterGraph(plots[1]);
    graph3s->setXColumn(columnX);
    graph3s->setYColumn(columnY2);
    graph3s->setTitle(QObject::tr("sine*exp graph"));
    plots[1]->addGraph(graph3s);

    JKQTPBarVerticalGraph* graphb=new JKQTPBarVerticalGraph(plots[3]);
    graphb->setXColumn(columnXbar);
    graphb->setYColumn(columnYbar);
    graphb->setTitle(QObject::tr("barchart"));
    plots[3]->addGraph(graphb);

    JKQTPBarVerticalGraph* graphb2=new JKQTPBarVerticalGraph(plots[3]);
    graphb2->setXColumn(columnXbar);
    graphb2->setYColumn(columnYbar2);
    graphb2->setTitle(QObject::tr("2^{nd} barchart"));
    plots[3]->addGraph(graphb2);

    JKQTPBarVerticalGraph* graphb3=new JKQTPBarVerticalGraph(plots[3]);
    graphb3->setXColumn(columnXbar);
    graphb3->setYColumn(columnYbar3);
    graphb3->setTitle(QObject::tr("3^{nd} barchart"));
    plots[3]->addGraph(graphb3);
    graphb3->autoscaleBarWidthAndShiftSeparatedGroups();


    JKQTPBoxplotVerticalGraph* graphBP=new JKQTPBoxplotVerticalGraph(plots[plots.size()/2]);
    graphBP->setPositionColumn(columnXBP);
    graphBP->setMinColumn(columnMinBP);
    graphBP->setPercentile25Column(columnQ25BP);
    graphBP->setMeanColumn(columnMeanBP);
    graphBP->setMedianColumn(columnMedianBP);
    graphBP->setPercentile75Column(columnQ75BP);
    graphBP->setMaxColumn(columnMaxBP);
    plots[plots.size()/2]->addGraph(graphBP);

    auto plotgeo=*(plots.rbegin()+1);
    JKQTPGeoSymbol* annotSym=new JKQTPGeoSymbol(plotgeo, -1.5, 3);
    plotgeo->addGraph(annotSym);

    JKQTPGeoText* annotTxt=new JKQTPGeoText(plotgeo, annotSym->getX()+0.5, annotSym->getY(), "Annotation $\\int_{0}^{2\\pi}\\sin(x)\\;\\mathrm{d}x$");
    plotgeo->addGraph(annotTxt);

    JKQTPGeoEllipse* geoEll=new JKQTPGeoEllipse(plotgeo, 1.5,0.5,3.5,1.2);
    geoEll->setAngle(-25);
    plotgeo->addGraph(geoEll);

    plotgeo->addGraph(new JKQTPGeoArrow(plotgeo, geoEll->getCenter(),annotSym->getP()-QPointF(0.5,0.5)));

    plotgeo->addGraph(new JKQTPGeoLine(plotgeo, geoEll->getCenter(),QPointF(annotSym->getX(),annotSym->getY()-2.0)));

    // 6. autoscale the plot so the graph is contained
    for (auto plt: plots) {
        plt->zoomToFit();
        plt->getXAxis()->setAxisLabel("x-axis");
        plt->getYAxis()->setAxisLabel("y-axis");
    }
    plots[0]->getYAxis()->setRange(plots[0]->getYAxis()->getMin(), 8);
    plots[1]->getYAxis()->setRange(plots[1]->getYAxis()->getMin(), 8);
    plots[3]->getYAxis()->setRange(plots[3]->getYAxis()->getMin(), 2.5);
    plotgeo->setMaintainAspectRatio(true);
    plotgeo->setMaintainAxisAspectRatio(true);
    plotgeo->getXAxis()->setRange(-2.5,4);
    plotgeo->getYAxis()->setRange(-2.5,4);


    plots[0]->setPlotLabel("Line Graphs");
    plots[1]->setPlotLabel("Scatter Graphs");
    plots[2]->setPlotLabel("Filled Graphs");
    plots[3]->setPlotLabel("Bar Charts");
    plots[plots.size()/2]->setPlotLabel("Boxplots");
    plotgeo->setPlotLabel("Geometry & Annotations");
    plots.last()->setPlotLabel("Image Plot");

}

inline void buildLineSymbolSampler(JKQTBasePlotter* plotExtra, JKQTBasePlotter* baseplot) {
    plotExtra->clearGraphs(true);
    plotExtra->setBackgroundColor(baseplot->getBackgroundColor());
    plotExtra->setExportBackgroundColor(baseplot->getExportBackgroundColor());
    plotExtra->setPlotBackgroundColor(baseplot->getPlotBackgroundColor());
    plotExtra->setPlotLabelColor(baseplot->getPlotLabelColor());
    plotExtra->getXAxis()->setDrawMode1(JKQTPCADMnone);
    plotExtra->getXAxis()->setDrawMode2(JKQTPCADMnone);
    plotExtra->getYAxis()->setDrawMode1(JKQTPCADMnone);
    plotExtra->getYAxis()->setDrawMode2(JKQTPCADMnone);
    double x=1;
    double xlineend=6;
    double y=1;
    double xmax=15;
    int cnt=0;
    const QColor defaultColor=baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphColors.first();
    for (auto s: baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphSymbols) {
        plotExtra->addGraph(new JKQTPGeoSymbol(plotExtra, x, y, s, 12, defaultColor, defaultColor.lighter() ));
        x+=1;
        xmax=qMax(x, xmax);
        if (cnt>0 && cnt%25==0) {
            x=1;
            y+=1.0;
        }
        cnt++;
    }
    y=y+1;
    double ycoltest=y;
    for (auto s: baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphPenStyles) {
        JKQTPGeoLine* l;
        plotExtra->addGraph(l=new JKQTPGeoLine(plotExtra, 1,y,xlineend,y)); l->setStyle(defaultColor, 2, s);
        plotExtra->addGraph(l=new JKQTPGeoLine(plotExtra, 1,y-0.25,xlineend,y-0.25)); l->setStyle(defaultColor.darker(), 1, s);
        y+=0.75;
    }
    x=xlineend+1;
    const int cntFillTests=qMax(baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphColors.size(), baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphFillStyles.size());
    double dx=(xmax-xlineend-1.0)/static_cast<double>(cntFillTests);
    for (int i=0; i<cntFillTests; i++) {
        const auto col=baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphColors.value(i%baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphColors.size(), Qt::black);
        const auto fs=baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphFillStyles.value(i%baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphFillStyles.size(), Qt::SolidPattern);
        JKQTPGeoLine* l;
        plotExtra->addGraph(l=new JKQTPGeoLine(plotExtra, x,ycoltest,x,y-0.5)); l->setStyle(col,5);
        JKQTPGeoRectangle* r;
        plotExtra->addGraph(r=new JKQTPGeoRectangle(plotExtra, x+dx/2,(ycoltest+y-0.5)/2.0, dx*0.5, y-ycoltest-0.5));
        r->setStyle(col,1, Qt::SolidLine, JKQTPGetDerivedColor(baseplot->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.fillColorDerivationMode, col), fs);
        x+=dx;
    }
    plotExtra->zoomToFit();
    plotExtra->setX(0, plotExtra->getXMax()+2);
    plotExtra->setY(0, plotExtra->getYMax()+1);
    plotExtra->setGrid(false);
    plotExtra->setPlotLabel("default symbols/lines/colors");

}

#endif // BUILD_TEST_GRAPHS_H
