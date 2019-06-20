/** \example jkqtplotter_simpletest_paramscatterplot.cpp
 * JKQTPlotter: Examples: Scatter Graph with Parametrized Symbols/Colors
 *
 * \ref JKQTPlotterParamScatter
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create a vector of x- and y-values for a simple plot (a sine curve)
    //    in addition, a vector P will hold values that will be mapped onto different plot styles
    //    and a vector C that will be mapped onto different colors
    QVector<double> X, Y1, Y2, Y3, Y4, Y5, Y6, Y7, P, C, S, RGB, LW;
    const int Ndata=10; // number of plot points in each curve
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*2.0*M_PI;
        X<<x;
        Y1<<1.5+sin(x);
        Y2<<3.5+sin(x);
        Y3<<5.5+sin(x);
        Y4<<7.5+sin(x);
        Y5<<9.5+sin(x);
        Y6<<11.5+sin(x);
        Y7<<12.5+sin(x);
        P<<(i+2);
        C<<x;
        S<<5*(i+1);
        LW<<(i+1)*1.5;
        RGB<<double(qRgb(double(i)/double(Ndata)*255,0,255-double(i)/double(Ndata)*255));
    }
    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY1=ds->addCopiedColumn(Y1, "y1");
    size_t columnY2=ds->addCopiedColumn(Y2, "y2");
    size_t columnY3=ds->addCopiedColumn(Y3, "y3");
    size_t columnY4=ds->addCopiedColumn(Y4, "y4");
    size_t columnY5=ds->addCopiedColumn(Y5, "y5");
    size_t columnY6=ds->addCopiedColumn(Y6, "y6");
    size_t columnY7=ds->addCopiedColumn(Y7, "y7");
    size_t columnP=ds->addCopiedColumn(P, "point_style");
    size_t columnC=ds->addCopiedColumn(C, "color_value");
    size_t columnS=ds->addCopiedColumn(S, "size");
    size_t columnLW=ds->addCopiedColumn(LW, "linewidth");
    size_t columnRGB=ds->addCopiedColumn(RGB, "rgb");


    // 3. now we make several graphs. In each one, another property of the graph is controlled by
    //    a datacolumn

    // 3.1 for graph1, we use dataset X/Y1
    //     and the symbol type is choose according to the contents of column P:
    JKQTPXYParametrizedScatterGraph* graph1=new JKQTPXYParametrizedScatterGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY1);
    graph1->setSymbolColumn(columnP);
    graph1->setDrawLine(true);
    graph1->setColor(QColor("blueviolet"));
    graph1->setTitle("1: symbol type");
    /*graph1->setSymbolColumnFunctor([](double x, double y, double sym) -> JKQTPGraphSymbols {
        if (sym<Ndata/2) {
            return JKQTPGraphSymbols::JKQTPCircle;
        } else if (sym>Ndata/2) {
            return JKQTPGraphSymbols::JKQTPFilledCircle;
        } else {
            return JKQTPGraphSymbols::JKQTPPlus;
        }
    });*/
    /*QMap<double, JKQTPGraphSymbols> mapped;
    mapped[0]=JKQTPGraphSymbols::JKQTPCircle;
    mapped[5]=JKQTPGraphSymbols::JKQTPFilledCircle;
    mapped[10]=JKQTPGraphSymbols::JKQTPPlus;
    graph1->setMappedSymbolColumnFunctor(mapped);*/
    plot.addGraph(graph1);

    // 3.2 for graph2, we use dataset X/Y2
    //     and the symbol color is choose according to the contents of column C
    //     and the actual colors are chose from palette JKQTPMathImageRYGB:
    JKQTPXYParametrizedScatterGraph* graph2=new JKQTPXYParametrizedScatterGraph(&plot);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setColorColumn(columnC);
    graph2->setPalette(JKQTPMathImageRYGB);
    graph2->setSymbolType(JKQTPFilledRect);
    graph2->setDrawLine(true);
    graph2->setTitle("2: color");
    graph2->getColorBarRightAxis()->setAxisLabel("color scale for graph2");
    plot.addGraph(graph2);

    // 3.3 for graph3, we use dataset X/Y3
    //     and the symbol size is choose according to the contents of column S:
    JKQTPXYParametrizedScatterGraph* graph3=new JKQTPXYParametrizedScatterGraph(&plot);
    graph3->setXColumn(columnX);
    graph3->setYColumn(columnY3);
    graph3->setSizeColumn(columnS);
    graph3->setSymbolType(JKQTPFilledCircle);
    graph3->setDrawLine(true);
    graph3->setTitle("3: symbol size");
    plot.addGraph(graph3);

    // 3.4 for graph4, we use X/Y4
    //     and the symbol color is choose according to the contents of column RGB,
    //     which directly contains the RGB values, as set with qRgb():
    JKQTPXYParametrizedScatterGraph* graph4=new JKQTPXYParametrizedScatterGraph(&plot);
    graph4->setXColumn(columnX);
    graph4->setYColumn(columnY4);
    graph4->setColorColumn(columnRGB);
    graph4->setColorColumnContainsRGB(true);
    graph4->setDrawLine(true);
    graph4->setSymbolType(JKQTPFilledDownTriangle);
    graph4->setTitle("4: RGB-color");
    plot.addGraph(graph4);

    // 3.5 for graph2, we use dataset X/Y5
    //     and the symbol color and size are choose according to the contents
    //     of columns C and S respectively.
    //     The actual colors are chose from palette JKQTPMathImageBLUEYELLOW:
    JKQTPXYParametrizedScatterGraph* graph5=new JKQTPXYParametrizedScatterGraph(&plot);
    graph5->setXColumn(columnX);
    graph5->setYColumn(columnY5);
    graph5->setColorColumn(columnC);
    graph5->setSizeColumn(columnS);
    graph5->setPalette(JKQTPMathImageBLUEYELLOW);
    graph5->setDrawLine(true);
    graph5->setTitle("5: color+size");
    graph5->getColorBarRightAxis()->setAxisLabel("color scale for graph5");
    plot.addGraph(graph5);

    // 3.6 for graph2, we use dataset X/Y6
    //     and the line width is set by column LW:
    JKQTPXYParametrizedScatterGraph* graph6=new JKQTPXYParametrizedScatterGraph(&plot);
    graph6->setXColumn(columnX);
    graph6->setYColumn(columnY6);
    graph6->setLinewidthColumn(columnLW);
    graph6->setDrawLine(true);
    graph6->setSymbolType(JKQTPNoSymbol);
    graph6->setTitle("6: line width");
    /*graph6->setLinewidthColumnFunctor([](double x, double y, double w) {
        return fabs(sin(w/3.0)*25.0);
    });*/
    plot.addGraph(graph6);

    // 3.7 for graph2, we use dataset X/Y7
    //     and the line width is set by column LW and the color from palette JKQTPMathImageBLUEYELLOW via column C:
    JKQTPXYParametrizedScatterGraph* graph7=new JKQTPXYParametrizedScatterGraph(&plot);
    graph7->setXColumn(columnX);
    graph7->setYColumn(columnY7);
    graph7->setLinewidthColumn(columnLW);
    graph7->setDrawLine(true);
    graph7->setColorColumn(columnC);
    graph7->setPalette(JKQTPMathImageBLUEYELLOW);
    graph7->setSymbolType(JKQTPNoSymbol);
    graph7->setColorBarRightVisible(false);
    graph7->setColorBarTopVisible(false);
    graph7->setTitle("7: color+line width");
    plot.addGraph(graph7);

    // 4. autoscale the plot so the graph is contained and format the coordinate system and key
    plot.getPlotter()->setKeyPosition(JKQTPKeyOutsideBottomLeft);
    plot.getPlotter()->setKeyLayout(JKQTPKeyLayoutOneRow);
    plot.getXAxis()->setAxisLabel("x-axis");
    plot.getYAxis()->setAxisLabel("y-axis");
    plot.getXAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawGrid(false);
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);

    return app.exec();
}
