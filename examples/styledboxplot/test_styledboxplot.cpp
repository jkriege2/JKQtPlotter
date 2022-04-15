/** \example test_styledboxplot.cpp
 * Shows how to style different aspects of boxplots.
 *
 * \ref JKQTPlotterBoxplotStyling
 */
 
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpboxplot.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering


    JKQTPBoxplotVerticalElement* graphBoxPlot;
    double x=1;
    double dx=1;
    double y0=11;

    // 2. create a basic boxplot
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{\\begin{matrix}basic boxplot\\\\\"Turkey's style\"\\end{matrix}}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"),QColor("white"), plot.getPlotter());
    x+=dx;

    // 3. create a basic boxplot with mean as symbol
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{\\begin{matrix}basic boxplot\\\\ + mean symbol\\end{matrix}}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"),QColor("white"), plot.getPlotter());
    // set mean value to show mean symbol
    graphBoxPlot->setMean(y0+3.5);
    graphBoxPlot->setDrawMean(true);
    graphBoxPlot->setMeanMode(JKQTPGraphBoxplotStyleMixin::MeanAsSymbol);
    graphBoxPlot->setMeanColor(QColor("darkgreen"));
    graphBoxPlot->setMeanSymbolType(JKQTPFilledTriangle);
    x+=dx;

    // 4. create a basic boxplot with mean as line
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{\\begin{matrix}basic boxplot\\\\ + mean line\\end{matrix}}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"),QColor("white"), plot.getPlotter());
    // set mean value to show mean symbol
    graphBoxPlot->setMean(y0+3.5);
    graphBoxPlot->setDrawMean(true);
    graphBoxPlot->setMeanMode(JKQTPGraphBoxplotStyleMixin::MeanAsLine);
    graphBoxPlot->setMeanColor(QColor("darkgreen"));
    graphBoxPlot->setMeanLineStyle(Qt::DashLine);
    graphBoxPlot->setMeanLineWidth(2);
    x+=dx;

    // 5. create a notched boxplot
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{notched boxplot}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"), QColor("white"), plot.getPlotter());
    // for a notched plot, you need to set the confidence interval
    graphBoxPlot->setMedianConfidenceIntervalWidth(1);
    graphBoxPlot->setDrawNotch(true);
    graphBoxPlot->setRelativeNotchIndent(0.5);
    x+=dx;


    // 6. create a notched boxplot
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{\\begin{matrix}basic boxplot\\\\\"Tufte's style\"\\end{matrix}}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"), QColor("white"), plot.getPlotter());
    // restyle as Tufte's box-less
    graphBoxPlot->setDrawBox(false);
    x+=dx;



    x=1;
    y0=0;

    // 7. style box
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{styled box}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"),QColor("white"), plot.getPlotter());
    // style box line&fill
    graphBoxPlot->setLineColor(QColor("red"));
    graphBoxPlot->setLineStyle(Qt::DotLine);
    graphBoxPlot->setLineWidth(1);
    graphBoxPlot->setFillColor(QColor("yellow"));
    x+=dx;

    // 8. fancy style box
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{styled box}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"),QColor("white"), plot.getPlotter());
    // style box fill
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(1, 1));
    QColor c1(Qt::red);
    QColor c2(Qt::yellow);
    QColor c3(Qt::blue);
    linearGrad.setColorAt(0, c1);
    linearGrad.setColorAt(0.3, c2);
    linearGrad.setColorAt(1, c3);
    // use this CoordinateMode, so the gradient fills the whole graph area
    linearGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    graphBoxPlot->setFillGradient(linearGrad);
    x+=dx;

    // 9. style median
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{styled median}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"),QColor("white"), plot.getPlotter());
    // style box line
    graphBoxPlot->setMedianLineColor(QColor("red"));
    graphBoxPlot->setMedianLineWidth(3);
    x+=dx;


    // 10. style whiskers&caps
    plot.addGraph(graphBoxPlot=new JKQTPBoxplotVerticalElement(&plot));
    plot.addGraph(new JKQTPGeoText(&plot, x-dx/2.0, y0+9, "\\textbf{styled whiskers}", 8, QColor("black")));
    graphBoxPlot->setPos(x);
    graphBoxPlot->setMin(y0+1);
    graphBoxPlot->setPercentile25(y0+2.5);
    graphBoxPlot->setMedian(y0+4);
    graphBoxPlot->setPercentile75(y0+5.5);
    graphBoxPlot->setMax(y0+8);
    // set color of all elements
    graphBoxPlot->setBoxplotColor(QColor("black"),QColor("white"), plot.getPlotter());
    // style box line
    graphBoxPlot->setWhiskerLineColor(QColor("red"));
    graphBoxPlot->setWhiskerLineStyle(Qt::DotLine);
    graphBoxPlot->setWhiskerLineWidth(1);
    graphBoxPlot->setWhiskerCapLineColor(QColor("blue"));
    graphBoxPlot->setWhiskerCapLineStyle(Qt::SolidLine);
    graphBoxPlot->setWhiskerCapLineWidth(4);
    graphBoxPlot->setRelativeWhiskerWidth(0.9);
    x+=dx;





    // 11. autoscale the plot so the graph is contained
    plot.setXY(0.5,5.5,0, 21);

    // 12. style plot
    plot.getPlotter()->setShowKey(false);
    plot.getPlotter()->setGrid(false);

    // 13. show plotter and make it a decent size
    plot.show();
    plot.resize(600,500);

    return app.exec();
}
