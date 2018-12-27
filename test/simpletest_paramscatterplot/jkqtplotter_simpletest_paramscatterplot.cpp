#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQtPlotter plot;
    JKQTPdatastore* ds=plot.getDatastore();

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
    JKQTPxyParametrizedScatterGraph* graph1=new JKQTPxyParametrizedScatterGraph(&plot);
    graph1->set_xColumn(columnX);
    graph1->set_yColumn(columnY1);
    graph1->set_symbolColumn(columnP);
    graph1->set_drawLine(true);
    graph1->set_color(QColor("blueviolet"));
    graph1->set_title("1: symbol type");
    plot.addGraph(graph1);

    // 3.2 for graph2, we use dataset X/Y2
    //     and the symbol color is choose according to the contents of column C
    //     and the actual colors are chose from palette JKQTPMathImageRYGB:
    JKQTPxyParametrizedScatterGraph* graph2=new JKQTPxyParametrizedScatterGraph(&plot);
    graph2->set_xColumn(columnX);
    graph2->set_yColumn(columnY2);
    graph2->set_colorColumn(columnC);
    graph2->set_palette(JKQTPMathImageRYGB);
    graph2->set_symbol(JKQTPfilledRect);
    graph2->set_drawLine(true);
    graph2->set_title("2: color");
    graph2->get_colorBarRightAxis()->set_axisLabel("color scale for graph2");
    plot.addGraph(graph2);

    // 3.3 for graph3, we use dataset X/Y3
    //     and the symbol size is choose according to the contents of column S:
    JKQTPxyParametrizedScatterGraph* graph3=new JKQTPxyParametrizedScatterGraph(&plot);
    graph3->set_xColumn(columnX);
    graph3->set_yColumn(columnY3);
    graph3->set_sizeColumn(columnS);
    graph3->set_symbol(JKQTPfilledCircle);
    graph3->set_drawLine(true);
    graph3->set_title("3: symbol size");
    plot.addGraph(graph3);

    // 3.4 for graph4, we use X/Y4
    //     and the symbol color is choose according to the contents of column RGB,
    //     which directly contains the RGB values, as set with qRgb():
    JKQTPxyParametrizedScatterGraph* graph4=new JKQTPxyParametrizedScatterGraph(&plot);
    graph4->set_xColumn(columnX);
    graph4->set_yColumn(columnY4);
    graph4->set_colorColumn(columnRGB);
    graph4->set_colorColumnContainsRGB(true);
    graph4->set_drawLine(true);
    graph4->set_symbol(JKQTPfilledDownTriangle);
    graph4->set_title("4: RGB-color");
    plot.addGraph(graph4);

    // 3.5 for graph2, we use dataset X/Y5
    //     and the symbol color and size are choose according to the contents
    //     of columns C and S respectively.
    //     The actual colors are chose from palette JKQTPMathImageBLUEYELLOW:
    JKQTPxyParametrizedScatterGraph* graph5=new JKQTPxyParametrizedScatterGraph(&plot);
    graph5->set_xColumn(columnX);
    graph5->set_yColumn(columnY5);
    graph5->set_colorColumn(columnC);
    graph5->set_sizeColumn(columnS);
    graph5->set_palette(JKQTPMathImageBLUEYELLOW);
    graph5->set_drawLine(true);
    graph5->set_title("5: color+size");
    graph5->get_colorBarRightAxis()->set_axisLabel("color scale for graph5");
    plot.addGraph(graph5);

    // 3.6 for graph2, we use dataset X/Y6
    //     and the line width is set by column LW:
    JKQTPxyParametrizedScatterGraph* graph6=new JKQTPxyParametrizedScatterGraph(&plot);
    graph6->set_xColumn(columnX);
    graph6->set_yColumn(columnY6);
    graph6->set_linewidthColumn(columnLW);
    graph6->set_drawLine(true);
    graph6->set_symbol(JKQTPnoSymbol);
    graph6->set_title("6: line width");
    plot.addGraph(graph6);

    // 3.7 for graph2, we use dataset X/Y7
    //     and the line width is set by column LW and the color from palette JKQTPMathImageBLUEYELLOW via column C:
    JKQTPxyParametrizedScatterGraph* graph7=new JKQTPxyParametrizedScatterGraph(&plot);
    graph7->set_xColumn(columnX);
    graph7->set_yColumn(columnY7);
    graph7->set_linewidthColumn(columnLW);
    graph7->set_drawLine(true);
    graph7->set_colorColumn(columnC);
    graph7->set_palette(JKQTPMathImageBLUEYELLOW);
    graph7->set_symbol(JKQTPnoSymbol);
    graph7->set_colorBarRightVisible(false);
    graph7->set_colorBarTopVisible(false);
    graph7->set_title("7: color+line width");
    plot.addGraph(graph7);

    // 4. autoscale the plot so the graph is contained and format the coordinate system and key
    plot.get_plotter()->set_keyPosition(JKQTPkeyOutsideBottomLeft);
    plot.get_plotter()->set_keyLayout(JKQTPkeyLayoutOneRow);
    plot.get_xAxis()->set_axisLabel("x-axis");
    plot.get_yAxis()->set_axisLabel("y-axis");
    plot.get_xAxis()->set_drawGrid(false);
    plot.get_yAxis()->set_drawGrid(false);
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);

    return app.exec();
}
