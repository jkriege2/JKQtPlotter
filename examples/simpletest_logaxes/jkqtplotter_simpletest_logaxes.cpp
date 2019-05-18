/** \example jkqtplotter_simpletest_logaxes.cpp
 * Shows how to use logarithmic axes with JKQTPlotter
 *
 * \ref JKQTPlotterLogAxes
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsscatter.h"
#include "jkqtplotter/jkqtpgraphsgeometric.h"
#include "jkqtcommon/jkqtptools.h"

#define sqr(x) ((x)*(x))

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data a vector of x-values for a simple plot (resonance curve with different damping value D)
    //    see https://en.wikipedia.org/wiki/Resonance
    QVector<double> X;
    QVector<double> D { 0.001, 0.1, 1, 10};
    const int Ndata=500; // number of plot points in each curve
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*3;
        X<<x;
        //Y1<<1.0/sqrt(sqr(1-sqr(x))+sqr(2*x*D1));
    }
    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");

    // 3. now we make several plots for different values of D
    QVector<Qt::PenStyle> pens {Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine };
    for (int id=0; id<D.size(); id++) {
        // generate some plot data
        QVector<double> Y;
        for (auto& xx: X) {
            Y<<1.0/sqrt(sqr(1-sqr(xx))+sqr(2*xx*D[id]));
        }

        JKQTPXYLineGraph* graph=new JKQTPXYLineGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->setXColumn(columnX);
        graph->setYColumn(ds->addCopiedColumn(Y, "y"+QString::number(id)));

        // don't use symbols
        graph->setSymbolType(JKQTPNoSymbol);
        // use one of different pens
        graph->setLineStyle(pens[id%pens.size()]);
        // set width of graph line
        graph->setLineWidth(1.5);

        // graph title is made from symbol+penstyle
        graph->setTitle(QString("$D=\\delta/\\omega_0=%1$").arg(D[id]));

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);
    }

    // 4. Also we add a text-element in the plot to show the plotted function
    //    This element (JKQTPGeoText) is taken from the set of geometric elements
    //    and is simply parametrized by a position (1.25/10) and the text to display.
    //    In addition you can also set the font size (here to 15)
    //    Use '$...$' around the actual math string to ensure rendering with a math font
    //    (the internal renderer uses XITS fonts by default, which are free and auto-distributed
    //    and loaded in the library). If you don't use the math-mode modifiers, the default
    //    font of the other rendering text is used, which might not be suitable for
    //    high-quality math rendering.
    plot.addGraph(new JKQTPGeoText(&plot, 1.25, 10, "$\\frac{A}{A_{stat}}=\\frac{1}{\\sqrt{\\left(1-\\eta^2\\right)^2+\\left(2{\\eta}D\\right)^2}}$", 15, QColor("black")));

    // 5. set y-axis to logarithmic (x-axis would be analogous, but using `plot.getXAxis()`)
    plot.getYAxis()->setLogAxis(true);
    //    now we set the number of label ticks to 9 (you give the count if minor between two majors,
    //    so if you want ticks for 1,2,3,...,10,20,30,...,100... you need to use 9:
    plot.getYAxis()->setMinorTicks(9);
    //    the minor grid is not shown by default. You can switch it on:
    plot.getYAxis()->setDrawMinorGrid(true);
    //    usually axis ticks are shown as numbers 0.01, 0.1, 1, 10, ... You can also force the scientific
    //    power-of-10 notation, using:
    plot.getYAxis()->setLabelType(JKQTPCALTexponent);
    //    the number of digits in JKQTPCALTexponent determines which labels are drawn in standard-notation,
    //    as compared to power-notation (e.g. if set to 1, the labels 0.1=10^{-1}, 1, 10 are shown in
    //    standard notation, the rest in power-notation. This tweak improves readability)
    plot.getYAxis()->setLabelDigits(0);
    // minor tick labels are usually not displayed, but you can switch them on, using
    //plot.getYAxis()->setMinorTickLabelsEnabled(true);
    // the axis font sizes can be set with:
    plot.getYAxis()->setTickLabelFontSize(10); // axis tick labels
    plot.getYAxis()->setMinorTickLabelFontSize(7); // minor axis tick labels
    plot.getYAxis()->setLabelFontSize(14); // axis label size
    plot.getXAxis()->setTickLabelFontSize(10); // axis tick labels
    plot.getXAxis()->setMinorTickLabelFontSize(7); // minor axis tick labels
    plot.getXAxis()->setLabelFontSize(14); // axis label size
    //    ... and finally set axis labels (using LaTeX notation and $...$ to improve rendering)
    plot.getYAxis()->setAxisLabel("Amplitude $A/A_{stat}$");
    plot.getXAxis()->setAxisLabel("relative driving frequency $\\eta=\\omega/\\omega_0$");


    // 6. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 7. show plotter and make it a decent size
    plot.show();
    plot.resize(700,500);

    return app.exec();
}
