/** \example second_axis.cpp
 * Shows how to draw Barcharts with different colors above and below the baseline in a JKQTPlotter
 *
 * \ref JKQTPlotterSecondaryAxes
 */
 
#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtplines.h"
#include "jkqtpexampleapplication.h"



void exampleSecondYAxis(JKQTPlotter& plot, const QString& title)
{
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create two columns for key and value
    size_t columnX=ds->addLinearColumn(40, 0,10,"x");
    size_t columnY1=ds->addColumnCalculatedFromColumn(columnX, [](double x) { return x; }, "y1");
    size_t columnY2=ds->addColumnCalculatedFromColumn(columnX, [](double x) { return cos(x); }, "y2");
    size_t columnY3=ds->addColumnCalculatedFromColumn(columnX, [](double x) { return x*x; }, "y3");

    // 3. create a second y-axis and set its formating options, so it only draws an axis on the right
    auto yAxisRef2=plot.getPlotter()->addSecondaryYAxis(new JKQTPVerticalAxis(plot.getPlotter(), JKQTPPrimaryAxis));
    plot.getYAxis(yAxisRef2)->setDrawGrid(false);
    plot.getYAxis(yAxisRef2)->setDrawMode1(JKQTPCADMnone);
    plot.getYAxis(yAxisRef2)->setDrawMode2(JKQTPCADMcomplete);
    plot.getYAxis(yAxisRef2)->setDrawMode0(JKQTPCADMnone);
    plot.getYAxis(yAxisRef2)->setShowZeroAxis(false);
    //   ... and ctreate third y-axis
    auto yAxisRef3=plot.getPlotter()->addSecondaryYAxis(new JKQTPVerticalAxis(plot.getPlotter(), JKQTPPrimaryAxis));
    plot.getYAxis(yAxisRef3)->setDrawGrid(false);
    plot.getYAxis(yAxisRef3)->setDrawMode1(JKQTPCADMnone);
    plot.getYAxis(yAxisRef3)->setDrawMode2(JKQTPCADMcomplete);
    plot.getYAxis(yAxisRef3)->setDrawMode0(JKQTPCADMnone);
    plot.getYAxis(yAxisRef3)->setShowZeroAxis(false);
    //   ... reformat the major y-axis, so it does not draw on the right and thus the secondary axis yAxisRef2 replaces it there
    //       if this step is omitted, the secondary axes stack on the right of the primary.
    plot.getYAxis()->setDrawMode2(JKQTPCADMnone);
    plot.getYAxis()->setAxisLabel("graph1");
    

    // 3. create graph in the plot, which plots the dataset:
    // 3.1 the first graph uses the default (primary) x/y-axes
    JKQTPXYLineGraph* graph1=new JKQTPXYLineGraph(&plot);
    graph1->setKeyColumn(columnX);
    graph1->setValueColumn(columnY1);
    plot.addGraph(graph1);
    plot.getYAxis()->setColor(graph1->getLineColor());

    // 3.2 the second graph uses the default (primary) x-axis, but the secondary axis yAxisRef2 as y-axis
    JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(&plot);
    graph2->setKeyColumn(columnX);
    graph2->setValueColumn(columnY2);
    plot.addGraph(graph2);
    //    tell graph2 to use this axis
    graph2->setYAxis(yAxisRef2);
    //    set axis color to match graph2's color
    plot.getYAxis(yAxisRef2)->setColor(graph2->getLineColor());
    //    set axis label
    plot.getYAxis(yAxisRef2)->setAxisLabel("graph2");

    // 3.3 the third graph uses the default (primary) x-axis, but the secondary axis yAxisRef3 as y-axis
    JKQTPXYLineGraph* graph3=new JKQTPXYLineGraph(&plot);
    graph3->setKeyColumn(columnX);
    graph3->setValueColumn(columnY3);
    plot.addGraph(graph3);
    plot.getYAxis(yAxisRef3)->setColor(graph3->getLineColor());
    plot.getYAxis(yAxisRef3)->setAxisLabel("graph3");
    graph3->setYAxis(yAxisRef3);


    // 4. autoscale the plot so the graph is contained
    //    This auto-scales all axes using the graphs (and their data) as assigned to the axes!
    //    i.e. all 3 graphs for x-axis, 
    //         graph1 for primary y-axis, 
    //         graph2 for secondary axis yAxisRef2
    //         graph3 for secondary axis yAxisRef3
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.setWindowTitle(title);
    plot.show();
    plot.resize(500,400);
}



void exampleSecondXAxis(JKQTPlotter& plot, const QString& title)
{
    // same as above, but with swapped x- and y-axes!!!
    
    JKQTPDatastore* ds=plot.getDatastore();

    size_t columnY=ds->addLinearColumn(30, 0,12,"y");
    size_t columnX1=ds->addColumnCalculatedFromColumn(columnY, [](double x) { return x; }, "x1");
    size_t columnX2=ds->addColumnCalculatedFromColumn(columnY, [](double x) { return cos(x); }, "x2");
    size_t columnX3=ds->addColumnCalculatedFromColumn(columnY, [](double x) { return x*x; }, "x3");

    JKQTPXYLineGraph* graph1=new JKQTPXYLineGraph(&plot);
    graph1->setXColumn(columnX1);
    graph1->setYColumn(columnY);
    plot.addGraph(graph1);
    JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(&plot);
    graph2->setXColumn(columnX2);
    graph2->setYColumn(columnY);
    plot.addGraph(graph2);
    JKQTPXYLineGraph* graph3=new JKQTPXYLineGraph(&plot);
    graph3->setXColumn(columnX3);
    graph3->setYColumn(columnY);
    plot.addGraph(graph3);

    plot.getXAxis()->setDrawMode2(JKQTPCADMLineTicksTickLabels);
    plot.getXAxis()->setColor(graph1->getLineColor());
    plot.getXAxis()->setAxisLabel("graph1");

    auto xAxisRef2=plot.getPlotter()->addSecondaryXAxis(new JKQTPHorizontalAxis(plot.getPlotter(), JKQTPPrimaryAxis));
    plot.getXAxis(xAxisRef2)->setDrawGrid(false);
    plot.getXAxis(xAxisRef2)->setDrawMode1(JKQTPCADMnone);
    plot.getXAxis(xAxisRef2)->setDrawMode2(JKQTPCADMcomplete);
    plot.getXAxis(xAxisRef2)->setDrawMode0(JKQTPCADMnone);
    plot.getXAxis(xAxisRef2)->setShowZeroAxis(false);
    plot.getXAxis(xAxisRef2)->setColor(graph2->getLineColor());
    plot.getXAxis(xAxisRef2)->setAxisLabel("graph2");
    graph2->setXAxis(xAxisRef2);

    auto xAxisRef3=plot.getPlotter()->addSecondaryXAxis(new JKQTPHorizontalAxis(plot.getPlotter(), JKQTPPrimaryAxis));
    plot.getXAxis(xAxisRef3)->setDrawGrid(false);
    plot.getXAxis(xAxisRef3)->setDrawMode1(JKQTPCADMnone);
    plot.getXAxis(xAxisRef3)->setDrawMode2(JKQTPCADMcomplete);
    plot.getXAxis(xAxisRef3)->setDrawMode0(JKQTPCADMnone);
    plot.getXAxis(xAxisRef3)->setShowZeroAxis(false);
    plot.getXAxis(xAxisRef3)->setColor(graph3->getLineColor());
    plot.getXAxis(xAxisRef3)->setAxisLabel("graph3");
    graph3->setXAxis(xAxisRef3);

    plot.zoomToFit();

    plot.setWindowTitle(title);
    plot.show();
    plot.resize(400,500);
}

int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    JKQTPlotter plotsecondY;
    exampleSecondYAxis(plotsecondY, "1: Second Y-Axis");
    JKQTPlotter plotsecondX;
    exampleSecondXAxis(plotsecondX, "2: Second X-Axis");

    return app.exec();
}
