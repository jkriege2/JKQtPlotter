/** \example jkqtplotter_simpletest_boxplot.cpp
 * Shows how to draw Boxplots with JKQTPlotter
 *
 * \ref JKQTPlotterBoxplotsGraphs
 */
 
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsboxplot.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    plot.get_plotter()->set_useAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.get_plotter()->set_useAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.get_plotter()->set_useAntiAliasingForText(true); // nicer (but slower) text rendering
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
    graph->set_posColumn(columnPOS);
    graph->set_minColumn(columnMIN);
    graph->set_percentile25Column(columnQ25);
    graph->set_medianColumn(columnMEDIAN);
    graph->set_meanColumn(columnMEAN);
    graph->set_percentile75Column(columnQ75);
    graph->set_maxColumn(columnMAX);
    graph->set_title("vertical Boxplots");

    // 5. outliers need to be drawn separately
    JKQTPXYLineGraph* graphOutliers=new JKQTPXYLineGraph(&plot);
    graphOutliers->set_xColumn(columnOUTLIERSX);
    graphOutliers->set_yColumn(columnOUTLIERSY);
    graphOutliers->set_title("outliers");
    // make the color a darker shade of the color of graph
    graphOutliers->set_color(graph->get_color().darker());
    graphOutliers->set_fillColor(QColor("white"));
    // draw outliers as small circles, without lines
    graphOutliers->set_symbol(JKQTPCircle);
    graphOutliers->set_drawLine(false);
    graphOutliers->set_symbolSize(7);

    // 6. create a graph of horizontal boxplots:
    JKQTPBoxplotHorizontalGraph* graphh=new JKQTPBoxplotHorizontalGraph(&plot);
    graphh->set_posColumn(columnPOSY);
    graphh->set_minColumn(columnMIN);
    graphh->set_percentile25Column(columnQ25);
    graphh->set_medianColumn(columnMEDIAN);
    graphh->set_meanColumn(columnMEAN);
    graphh->set_percentile75Column(columnQ75);
    graphh->set_maxColumn(columnMAX);
    graphh->set_title("horizontal Boxplots");
    // 6.1 make fill collor a lighter shade of the outline color
    graphh->set_fillColor(graphh->get_color().lighter());
    // 6.2 make whiskers dashed
    graphh->set_whiskerStyle(Qt::DashLine);
    // 6.3 change mean symbol
    graphh->set_meanSymbol(JKQTPstar);


    // 7. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph);
    plot.addGraph(graphOutliers);
    plot.addGraph(graphh);


    // 8. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 9. Move key to top-left
    plot.get_plotter()->set_keyPosition(JKQTPKeyInsideTopLeft);

    // show plotter and make it a decent size
    plot.show();
    plot.resize(800,800);

    return app.exec();
}
