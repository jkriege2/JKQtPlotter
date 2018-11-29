#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgeoelements.h"
#include "jkqtplottertools/jkqtptools.h"

#define sqr(x) ((x)*(x))

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQtPlotter plot;
    JKQTPdatastore* ds=plot.getDatastore();

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

        JKQTPxyLineGraph* graph=new JKQTPxyLineGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->set_xColumn(columnX);
        graph->set_yColumn(ds->addCopiedColumn(Y, "y"+QString::number(id)));

        // don't use symbols
        graph->set_symbol(JKQTPnoSymbol);
        // use one of different pens
        graph->set_style(pens[id%pens.size()]);
        // set width of graph line
        graph->set_lineWidth(1.5);

        // graph title is made from symbol+penstyle
        graph->set_title(QString("D=\\delta/\\omega_0=%1").arg(D[id]));

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);
    }

    // 4. Also we add a text-element in the plot to show the plotted function
    //    This element (JKQTPgeoText) is taken from the set of geometric elements
    //    and is simply parametrized by a position (1.25/10) and the text to display.
    //    In addition you can also set the font size (here to 15)
    plot.addGraph(new JKQTPgeoText(&plot, 1.25, 10, "\\frac{A}{A_{stat}}=\\frac{1}{\\sqrt{\\left(1-\\eta^2\\right)^2+\\left(2{\\eta}D\\right)^2}}", 15));
    // for nicer rendering we set the fonts used by the internal LaTeX parser instance to XITS
    //plot.get_plotter()->get_mathText()->useXITS();

    // 4. set y-axis to logarithmic (x-axis would be analogous)
    //    and set axis labels (using LaTeX notation)
    plot.get_yAxis()->set_logAxis(true);
    plot.get_yAxis()->set_axisLabel("Amplitude A/A_{stat}");
    plot.get_xAxis()->set_axisLabel("relative driving frequency \\eta=\\omega/\\omega_0");


    // 5. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 6. show plotter and make it a decent size
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
