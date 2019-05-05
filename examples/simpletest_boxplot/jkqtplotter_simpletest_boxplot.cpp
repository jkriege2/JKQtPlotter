/** \example jkqtplotter_simpletest_boxplot.cpp
 * Shows how to draw Boxplots with JKQTPlotter
 *
 * \ref JKQTPlotterBoxplotsGraphs
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsscatter.h"
#include "jkqtplotter/jkqtpgraphsboxplot.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data for the boxplots
    QVector<double> POS, POSY, MEDIAN, MEAN, Q25, Q75, MIN, MAX, OUTLIERSX, OUTLIERSY;
    POS    << 1   << 4   << 7   << 10;
    POSY   << -1  << -2  << -3  << -4;
    MIN    << 2   << 3   << 2.5 << 6;
    Q25    << 4   << 4.5 << 5   << 7;
    MEDIAN << 5   << 6   << 7   << 9;
    MEAN   << 5.5 << 5.2 << 8   << 8;
    Q75    << 6   << 7   << 9   << 11;
    MAX    << 8   << 7.5 << 11  << 12;
    // ... and some outliers (as (x,y)-pairs)
    OUTLIERSX << 4 << 4   << 4   << 4 << 4      << 10 << 10 << 10   << 10 << 10   << 10   << 10;
    OUTLIERSY << 1 << 0.5 << 1.3 << 8 << 8.1    << 5  << 4  << 12.2 << 13 << 12.5 << 13.5 << 13.1;

    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    size_t columnPOS=ds->addCopiedColumn(POS, "POS");
    size_t columnPOSY=ds->addCopiedColumn(POSY, "POSY");
    size_t columnMIN=ds->addCopiedColumn(MIN, "MIN");
    size_t columnQ25=ds->addCopiedColumn(Q25, "Q25");
    size_t columnMEDIAN=ds->addCopiedColumn(MEDIAN, "MEDIAN");
    size_t columnMEAN=ds->addCopiedColumn(MEAN, "MEAN");
    size_t columnQ75=ds->addCopiedColumn(Q75, "Q75");
    size_t columnMAX=ds->addCopiedColumn(MAX, "MAX");
    size_t columnOUTLIERSX=ds->addCopiedColumn(OUTLIERSX, "OUTLIERSX");
    size_t columnOUTLIERSY=ds->addCopiedColumn(OUTLIERSY, "OUTLIERSY");

    // 4. create a graph of vertical boxplots:
    JKQTPBoxplotVerticalGraph* graph=new JKQTPBoxplotVerticalGraph(&plot);
    graph->setPositionColumn(columnPOS);
    graph->setMinColumn(columnMIN);
    graph->setPercentile25Column(columnQ25);
    graph->setMedianColumn(columnMEDIAN);
    graph->setMeanColumn(columnMEAN);
    graph->setPercentile75Column(columnQ75);
    graph->setMaxColumn(columnMAX);
    graph->setTitle("vertical Boxplots");

    // 5. outliers need to be drawn separately
    JKQTPXYLineGraph* graphOutliers=new JKQTPXYLineGraph(&plot);
    graphOutliers->setXColumn(columnOUTLIERSX);
    graphOutliers->setYColumn(columnOUTLIERSY);
    graphOutliers->setTitle("outliers");
    // make the color a darker shade of the color of graph
    graphOutliers->setColor(graph->getLineColor().darker());
    graphOutliers->setSymbolFillColor(QColor("white"));
    // draw outliers as small circles, without lines
    graphOutliers->setSymbolType(JKQTPCircle);
    graphOutliers->setDrawLine(false);
    graphOutliers->setSymbolSize(7);

    // 6. create a graph of horizontal boxplots:
    JKQTPBoxplotHorizontalGraph* graphh=new JKQTPBoxplotHorizontalGraph(&plot);
    graphh->setPositionColumn(columnPOSY);
    graphh->setMinColumn(columnMIN);
    graphh->setPercentile25Column(columnQ25);
    graphh->setMedianColumn(columnMEDIAN);
    graphh->setMeanColumn(columnMEAN);
    graphh->setPercentile75Column(columnQ75);
    graphh->setMaxColumn(columnMAX);
    graphh->setTitle("horizontal Boxplots");
    // 6.1 make fill collor a lighter shade of the outline color
    graphh->setFillColor(graphh->getLineColor().lighter());
    // 6.2 make whiskers dashed
    graphh->setWhiskerLineStyle(Qt::DashLine);
    graphh->setWhiskerLineColor(graphh->getLineColor().darker());
    // 6.3 change mean symbol
    graphh->setSymbolType(JKQTPFilledStar);
    graphh->setSymbolFillColor(QColor("silver"));
    // 6.4 change median line color
    graphh->setMedianLineColor(QColor("darkgreen"));
    // 6.5 change box width to 75% of distance
    graphh->setBoxWidth(0.75);

    // 7. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);
    plot.addGraph(graphOutliers);
    plot.addGraph(graphh);


    // 8. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 9. Move key to top-left
    plot.getPlotter()->setKeyPosition(JKQTPKeyInsideTopLeft);

    // show plotter and make it a decent size
    plot.show();
    plot.resize(800,800);

    return app.exec();
}
