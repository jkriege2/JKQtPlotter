/** \example graphlabels.cpp
 * Shows how to draw datapoint labels using JKQTPXYGraphLabels
 *
 * \ref JKQTPlotterGraphLabelsExample
 */
 
#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtplotter/graphs/jkqtpgraphlabels.h"
#include "jkqtpexampleapplication.h"

#define Ndata 6


template <class TCHART>
std::pair<TCHART*, JKQTPXYGraphLabels*> doExample(JKQTPlotter& plot, const QString& title, JKQTPGraphLabelPosition labPos)
{
    // 1. setup a plotter window and get a pointer to the internal datastore (for convenience)
    plot.getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot.getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* ds=plot.getDatastore();



    // 2. make up some arbitrary data to be used for plotting
    const size_t columnX=ds->addLinearColumn(Ndata, -1,1,"x");
    const size_t columnY=ds->addCalculatedColumnFromColumn(columnX, [](double x) { return jkqtp_roundToDigits(-sin(x*3.0),2);}, "data");


    // 3. create barchart and line-chart to display the data:
    TCHART* graph1=new TCHART(&plot);
    graph1->setBarPositionColumn(columnX);
    graph1->setBarHeightColumn(columnY);
    graph1->setTitle(QObject::tr("dataset: bars"));
    graph1->setColor(QColorWithAlphaF(graph1->getFillColor(),0.25));
    JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(&plot);
    if (typeid(TCHART)==typeid(JKQTPBarVerticalGraph)) {
        graph2->setXColumn(columnX);
        graph2->setYColumn(columnY);
    } else {
        graph2->setXColumn(columnY);
        graph2->setYColumn(columnX);
    }
    graph2->setTitle(QObject::tr("dataset: lines"));
    graph2->setColor(QColorWithAlphaF(graph2->getLineColor(),0.5));

    // 4.1. create barchart to display the data:
    JKQTPXYGraphLabels* graphLabels=new JKQTPXYGraphLabels((typeid(TCHART)==typeid(JKQTPBarVerticalGraph)) ? JKQTPXYGraphLabels::YValueLabel : JKQTPXYGraphLabels::XValueLabel, &plot);
    graphLabels->setXColumn(graph1->getXColumn());
    graphLabels->setYColumn(graph1->getYColumn());

    // 4.2. set position of labels and some styling options
    graphLabels->setLabelPosition(labPos);
    graphLabels->setTextFontSize(14);

    // 5. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph1);
    plot.addGraph(graph2);
    plot.addGraph(graphLabels);

    // 6. scale the plot so the graph is contained
    plot.setXY(-1.5,1.5,-1.5,1.5);

    // 7.modify the plot style a bit
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine);
    plot.getXAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getYAxis()->setDrawMode1(JKQTPCADMLine);
    plot.getYAxis()->setDrawMode2(JKQTPCADMLine);

    // show plotter and make it a decent size
    plot.setWindowTitle(title);
    plot.show();
    plot.resize(400/plot.devicePixelRatioF(),250/plot.devicePixelRatioF());

    return {graph1, graphLabels};
}

int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);

    JKQTPlotter plotV;
    auto gV=doExample<JKQTPBarVerticalGraph>(plotV, "1: JKQTPXYGraphLabels+JKQTPBarVerticalGraph", JKQTPGLabelAwayFromXAxis);
    JKQTPlotter plotH;
    auto gH=doExample<JKQTPBarHorizontalGraph>(plotH, "2: JKQTPXYGraphLabels+JKQTPBarHorizontalGraph", JKQTPGLabelAwayFromYAxis);

    app.addExportStepFunctor([&](){
        gV.second->setLabelPosition(JKQTPGLabelTowardsXAxis);
        gH.second->setLabelPosition(JKQTPGLabelTowardsYAxis);
        plotV.redrawPlot();
        plotH.redrawPlot();
    });

    app.addExportStepFunctor([&](){
        gV.second->setLabelPosition(JKQTPGLabelAboveData);
        gH.second->setLabelPosition(JKQTPGLabelRightHandSide);
        plotV.redrawPlot();
        plotH.redrawPlot();
    });

    app.addExportStepFunctor([&](){
        gV.second->setLabelPosition(JKQTPGLabelBelowData);
        gH.second->setLabelPosition(JKQTPGLabelLeftHandSide);
        plotV.redrawPlot();
        plotH.redrawPlot();
    });

    app.addExportStepFunctor([&](){
        gV.second->setLabelPosition(JKQTPGLabelCenteredOnData);
        gH.second->setLabelPosition(JKQTPGLabelCenteredOnData);
        gV.first->setVisible(false);
        gH.first->setVisible(false);
        plotV.redrawPlot();
        plotH.redrawPlot();
    });

    app.addExportStepFunctor([&](){
        gV.first->setVisible(true);
        gH.first->setVisible(true);
        gV.second->setLabelPosition(JKQTPGLabelAwayFromXAxis);
        gV.second->setLabelBoxType(JKQTPGLSimpleBox);
        gV.second->setDrawLabelBoxFrame(true);
        gH.second->setLabelPosition(JKQTPGLabelAwayFromYAxis);
        gH.second->setLabelBoxType(JKQTPGLSimpleBox);
        gH.second->setDrawLabelBoxFrame(true);
        plotV.redrawPlot();
        plotH.redrawPlot();
    });
    app.addExportStepFunctor([&](){
        gV.second->setLabelPosition(JKQTPGLabelAwayFromXAxis);
        gV.second->setLabelBoxType(JKQTPGLSimpleBoxAndLine);
        gV.second->setDrawLabelBoxFrame(true);
        gH.second->setLabelPosition(JKQTPGLabelAwayFromYAxis);
        gH.second->setLabelBoxType(JKQTPGLSimpleBoxAndLine);
        gH.second->setDrawLabelBoxFrame(true);
        plotV.redrawPlot();
        plotH.redrawPlot();
    });
    app.addExportStepFunctor([&](){
        //plotV.getPlotter()->setOnlyGraphVisible(plotV.getPlotter()->indexOfGraph(gV.second));
        //plotH.getPlotter()->setOnlyGraphVisible(plotH.getPlotter()->indexOfGraph(gH.second));
        gV.first->setVisible(false);
        gH.first->setVisible(false);

        plotV.redrawPlot();
        plotH.redrawPlot();
    });

    return app.exec();
}
