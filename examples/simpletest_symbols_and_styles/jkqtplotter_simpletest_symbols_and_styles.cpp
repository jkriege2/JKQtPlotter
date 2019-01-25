/** \example jkqtplotter_simpletest_symbols_and_styles.cpp
 * JKQTPlotter: Examples: Line Graph with Different Symbols and Line Styles
 *
 * \ref JKQTPlotterSymbolsAndStyles
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplottertools/jkqtptools.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data a vector of x-values for a simple plot (a sine curve)
    QVector<double> X;
    const int Ndata=10; // number of plot points in each curve
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*2.0*M_PI;
        X<<x;
    }
    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");


    // 3. now we make several plots with different symbol styles and line-styles
    //    for that we iterate over every symbol style and at the same time over
    //    pen styles from the vector pens
    QVector<Qt::PenStyle> pens {Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine };
    int ipen=0;
    for (int symbolID=0; symbolID<=JKQTPMaxSymbolID; symbolID++) {
        // generate some plot data
        QVector<double> Y;
        for (auto& xx: X) {
            Y<<sin(xx)+static_cast<double>(symbolID)*1.5;
        }

        JKQTPXYLineGraph* graph=new JKQTPXYLineGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->set_xColumn(columnX);
        graph->set_yColumn(ds->addCopiedColumn(Y, "y"+QString::number(symbolID)));

        // set symbol + pen style and color
        graph->set_symbol(static_cast<JKQTPGraphSymbols>(symbolID));
        graph->set_style(pens[ipen]);
        QString lname=jkqtp_QPenStyle2String(graph->get_style());
        graph->set_drawLine(static_cast<JKQTPGraphSymbols>(symbolID)!=JKQTPDot);
        if (!graph->get_drawLine()) lname="";
        // set symbol size
        graph->set_symbolSize(14);
        // set width of symbol lines
        graph->set_symbolWidth(1.5);
        // set width of graph line
        graph->setLineWidth(1);

        // graph title is made from symbol+penstyle
        graph->set_title(JKQTPGraphSymbols2NameString(static_cast<JKQTPGraphSymbols>(graph->get_symbol()))+QString(", ")+lname);

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);

        ipen++;
        if (ipen>=pens.size()) ipen=0;
    }

    // 4. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);

    return app.exec();
}
