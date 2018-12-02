#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplottertools/jkqtptools.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQtPlotter plot;
    JKQTPdatastore* ds=plot.getDatastore();

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
    QVector<JKQTPerrorPlotstyle> errorStyles    {JKQTPnoError, JKQTPerrorBars, JKQTPerrorSimpleBars, JKQTPerrorLines, JKQTPerrorPolygons, JKQTPerrorBoxes, JKQTPerrorEllipses, JKQTPerrorBarsPolygons, JKQTPerrorBarsLines, JKQTPerrorSimpleBarsLines, JKQTPerrorSimpleBarsPolygons };
    QVector<bool>                showXandYError { false      , true          , true                , false          , false             , true           , true              , false                 , false              , false                    , false                        };

    for (int errorID=0; errorID<errorStyles.size(); errorID++) {
        // generate some plot data
        QVector<double> Y;
        for (auto& xx: X) {
            Y<<xx*0.5+static_cast<double>(errorID)*2.5;
        }

        JKQTPxyLineErrorGraph* graph=new JKQTPxyLineErrorGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->set_xColumn(columnX);
        graph->set_yColumn(ds->addCopiedColumn(Y, "y"+QString::number(errorID)));
        graph->set_xErrorColumn(columnXError);
        graph->set_yErrorColumn(columnYError);

        // set error style, for the y-axis
        graph->set_yErrorStyle(errorStyles[errorID]);
        // no error indicators for the x-values
        graph->set_xErrorStyle(JKQTPnoError);
        // ... unless: for some error styles we want error in both directions
        if (showXandYError[errorID]) {
            graph->set_xErrorStyle(errorStyles[errorID]);
            graph->set_drawLine(false);
        }
        // make error indicator 30% transparent
        QColor c=graph->get_errorFillColor();
        c.setAlphaF(0.3);
        graph->set_errorFillColor(c);
        // set error indicator line width
        graph->set_errorWidth(1);
        // set length of small bars at the end of error bars
        graph->set_errorbarSize(15);


        // set symbol (cross/X) + pen style (and color)dashed)
        graph->set_symbol(JKQTPcross);
        graph->set_style(Qt::DashLine);
        // set symbol size
        graph->set_symbolSize(5);
        // set width of symbol lines
        graph->set_symbolWidth(1);
        // set width of graph line
        graph->set_lineWidth(1);

        // graph title is made from symbol+errorStylestyle, we use the LaTeX instruction \verb around the
        // result of JKQTPerrorPlotstyle2String(), because it contains underscores that would otherwise
        // lead to lower-case letter, which we don't want
        graph->set_title("\\verb{"+JKQTPerrorPlotstyle2String(errorStyles[errorID])+"}");

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);
    }

    // 4. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 6. change locaion of key (outside top-right)
    plot.get_plotter()->set_keyPosition(JKQTPkeyOutsideRightTop);
    // ... and switch off the grid
    plot.getXAxis()->set_drawGrid(false);
    plot.getXAxis()->set_drawMinorGrid(false);
    plot.getYAxis()->set_drawGrid(false);
    plot.getYAxis()->set_drawMinorGrid(false);

    // 7. show plotter and make it a decent size
    plot.show();
    plot.resize(800,600);

    return app.exec();
}
