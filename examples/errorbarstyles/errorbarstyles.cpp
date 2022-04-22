/** \example errorbarstyles.cpp
 * Shows how to use different error indicator styles with JKQTPlotter
 *
 * \ref JKQTPlotterErrorBarStyles
 */

#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtptools.h"


int main(int argc, char* argv[])
{
        
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create data a vector of x-values for a simple plot
    //    and generate some values for varying errors
    //    y-data for the pot will be generated later to position the
    //    plot at different locations.
    QVector<double> X, Yerror, Xerror;
    const int Ndata=7; // number of plot points in each curve
    for (int i=0; i<Ndata; i++) {
        const double x=10.0+double(i)*2.0;
        X<<x;
        Yerror<<0.35+0.3*sin(x);
        Xerror<<0.35+0.3*sin(x);
    }
    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnXError=ds->addCopiedColumn(Xerror, "xerror");
    size_t columnYError=ds->addCopiedColumn(Yerror, "yerror");


    // 3. now we make several plots with different error styles
    //    for that we iterate over every style from the vector errorStyles
    //    the array showXandYError indicates whether we want x- and y-error
    //    for a style for all stywhere this is false, only y-error-indicators
    //    are shown
    QVector<JKQTPErrorPlotstyle> errorStyles    {JKQTPNoError, JKQTPErrorBars, JKQTPErrorSimpleBars, JKQTPErrorLines, JKQTPErrorPolygons, JKQTPErrorBoxes, JKQTPErrorEllipses, JKQTPErrorBarsPolygons, JKQTPErrorBarsLines, JKQTPErrorSimpleBarsLines, JKQTPErrorSimpleBarsPolygons };
    QVector<bool>                showXandYError { false      , true          , true                , false          , false             , true           , true              , false                 , false              , false                    , false                        };

    for (int errorID=0; errorID<errorStyles.size(); errorID++) {
        // generate some plot data
        QVector<double> Y;
        for (auto& xx: X) {
            Y<<xx*0.5+static_cast<double>(errorID)*2.5;
        }

        JKQTPXYLineErrorGraph* graph=new JKQTPXYLineErrorGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->setXColumn(columnX);
        graph->setYColumn(ds->addCopiedColumn(Y, "y"+QString::number(errorID)));
        graph->setXErrorColumn(columnXError);
        graph->setYErrorColumn(columnYError);

        // set error style, for the y-axis
        graph->setYErrorStyle(errorStyles[errorID]);
        // no error indicators for the x-values
        graph->setXErrorStyle(JKQTPNoError);
        // ... unless: for some error styles we want error in both directions
        if (showXandYError[errorID]) {
            graph->setXErrorStyle(errorStyles[errorID]);
            graph->setDrawLine(false);
        }
        // make error indicator 30% transparent
        QColor c=graph->getErrorFillColor();
        c.setAlphaF(0.3);
        graph->setErrorFillColor(c);
        // set error indicator line width
        graph->setErrorLineWidth(1);
        // set length of small bars at the end of error bars
        graph->setErrorBarCapSize(15);


        // set symbol (cross/X) + pen style (and color)dashed)
        graph->setSymbolType(JKQTPCross);
        graph->setLineStyle(Qt::DashLine);
        // set symbol size
        graph->setSymbolSize(5);
        // set width of symbol lines
        graph->setSymbolLineWidth(1);
        // set width of graph line
        graph->setLineWidth(1);

        // graph title is made from symbol+errorStylestyle, we use the LaTeX instruction \verb around the
        // result of JKQTPErrorPlotstyle2String(), because it contains underscores that would otherwise
        // lead to lower-case letter, which we don't want
        graph->setTitle("\\verb{"+JKQTPErrorPlotstyle2String(errorStyles[errorID])+"}");

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);
    }

    // 4. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 6. change locaion of key (outside top-right)
    plot.getPlotter()->setKeyPosition(JKQTPKeyOutsideRightTop);
    // ... and switch off the grid
    plot.getXAxis()->setDrawGrid(false);
    plot.getXAxis()->setDrawMinorGrid(false);
    plot.getYAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawMinorGrid(false);

    // 7. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);

    return app.exec();
}
