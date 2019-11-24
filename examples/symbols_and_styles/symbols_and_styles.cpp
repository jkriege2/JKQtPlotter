/** \example symbols_and_styles.cpp
 * JKQTPlotter: Examples: Line Graph with Different Symbols and Line Styles
 *
 * \ref JKQTPlotterSymbolsAndStyles
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtptools.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data a vector of x-values for a simple plot (a sine curve)
    QVector<double> X1, X2;
    const int Ndata=10; // number of plot points in each curve
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*2.0*JKQTPSTATISTICS_PI;
        if (i<Ndata/2) X1<<x;
        if (i>Ndata/2) X2<<x;
    }
    // and copy it to the datastore
    size_t columnX1=ds->addCopiedColumn(X1, "x1");
    size_t columnX2=ds->addCopiedColumn(X2, "x2");


    // 3. now we make several plots with different symbol styles and line-styles
    //    for that we iterate over every symbol style and at the same time over
    //    pen styles from the vector pens
    QVector<Qt::PenStyle> pens {Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine };
    int ipen=0;
    for (int symbolID=0; symbolID<=static_cast<int>(JKQTPMaxSymbolID); symbolID++) {
        // generate some plot data
        QVector<double> Y;
        const QVector<double>& X=(symbolID<=static_cast<int>(JKQTPMaxSymbolID)/2)?X1:X2;
        const double dy=(symbolID<=static_cast<int>(JKQTPMaxSymbolID)/2)?(static_cast<double>(symbolID)*1.5):(static_cast<double>(symbolID-static_cast<int>(JKQTPMaxSymbolID)/2)*1.5);
        for (auto& xx: X) {
            Y<<sin(xx)+dy;
        }

        JKQTPXYLineGraph* graph=new JKQTPXYLineGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->setXColumn((symbolID<=JKQTPMaxSymbolID/2)?columnX1:columnX2);
        graph->setYColumn(ds->addCopiedColumn(Y, "y"+QString::number(symbolID)));

        // set symbol + pen style and color
        graph->setSymbolType(static_cast<JKQTPGraphSymbols>(symbolID));
        graph->setLineStyle(pens[ipen]);
        QString lname=jkqtp_QPenStyle2String(graph->getLineStyle());
        graph->setDrawLine(static_cast<JKQTPGraphSymbols>(symbolID)!=JKQTPDot);
        if (!graph->getDrawLine()) lname="";
        // set symbol size
        graph->setSymbolSize(12);
        // set width of symbol lines
        graph->setSymbolLineWidth(1.5);
        // set width of graph line
        graph->setLineWidth(1);

        // graph title is made from symbol+penstyle
        graph->setTitle(JKQTPGraphSymbols2NameString(static_cast<JKQTPGraphSymbols>(graph->getSymbolType()))+QString(", ")+lname);

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);

        ipen++;
        if (ipen>=pens.size()) ipen=0;

        // create PNG with the symbol
        /*QPixmap pix(24,24);
        pix.fill(Qt::transparent);
        JKQTPEnhancedPainter p;
        p.begin(&pix);
        p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
        p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
        JKQTPPlotSymbol(p, pix.width()/2,pix.height()/2,static_cast<JKQTPGraphSymbols>(symbolID),pix.width()/1.2,1.5,QColor("blue"), QColor("blue").lighter());
        p.end();
        pix.save(JKQTPGraphSymbols2String(static_cast<JKQTPGraphSymbols>(symbolID))+".png", "PNG");*/

    }


    // 4. autoscale the plot so the graph is contained
    plot.getPlotter()->setKeyPosition(JKQTPKeyPosition::JKQTPKeyOutsideBottomLeft);
    plot.getPlotter()->setKeyLayout(JKQTPKeyLayout::JKQTPKeyLayoutMultiColumn);
    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.setGrid(false);
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.show();
    plot.resize(1200, 900);

    return app.exec();
}
