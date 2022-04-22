/** \example barchart.cpp
 * Shows how to draw Barcharts with JKQTPlotter
 *
 * \ref JKQTPlotterBarcharts
 */
 
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"

#define Ndata 5


template <class TCHART>
void doExample()
{
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter* plot=new JKQTPlotter();
    plot->getPlotter()->setUseAntiAliasingForGraphs(true); // nicer (but slower) plotting
    plot->getPlotter()->setUseAntiAliasingForSystem(true); // nicer (but slower) plotting
    plot->getPlotter()->setUseAntiAliasingForText(true); // nicer (but slower) text rendering
    JKQTPDatastore* ds=plot->getDatastore();

    // 2. now we create data for three simple barchart
    QString L[Ndata]={  "cat. A", "cat. C", "cat. B", "cat. D", "other"}; // unsorted category axis
    double  X[Ndata]={        1,        3,        2,        4,       5};
    //QString L[Ndata]={  "cat. A", "cat. B", "cat. C", "cat. D", "other"}; // correctly sorted data!
    //double  X[Ndata]={        1,        2,        3,        4,       5};
    double Y1[Ndata]={        5,        4,        3,        4,       5};
    double Y2[Ndata]={       -5,       -3,        1,        3,       6};
    double Y3[Ndata]={        6,        2,        5,        3,       6};

    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    //    Note: In this step the data is copied (of not specified otherwise)
    //    the variables columnX and columnY... will contain the internal column ID of the
    //    newly created columns with names "x" and "y..." and the (copied) data from X
    //    and Y...
    size_t columnX=ds->addCopiedColumn(X, Ndata, "x");
    size_t columnY1=ds->addCopiedColumn(Y1, Ndata, "y1");
    size_t columnY2=ds->addCopiedColumn(Y2, Ndata, "y2");
    size_t columnY3=ds->addCopiedColumn(Y3, Ndata, "y3");

    // 4. create graphs in the plot, which plots the dataset X/Y1, X/Y2 and X/Y3:
    TCHART* graph1=new TCHART(plot);
    graph1->setBarPositionColumn(columnX);
    graph1->setBarHeightColumn(columnY1);
    graph1->setTitle(QObject::tr("dataset 1"));
    TCHART* graph2=new TCHART(plot);
    graph2->setBarPositionColumn(columnX);
    graph2->setBarHeightColumn(columnY2);
    graph2->setTitle(QObject::tr("dataset 2"));
    TCHART* graph3=new TCHART(plot);
    graph3->setBarPositionColumn(columnX);
    graph3->setBarHeightColumn(columnY3);
    graph3->setTitle(QObject::tr("dataset 3"));


    // 5. add the graphs to the plot, so it is actually displayed
    plot->addGraph(graph1);
    plot->addGraph(graph2);
    plot->addGraph(graph3);

    // 6. now we set the graphs, so they are plotted side-by-side
    //    This function searches all JKQTPBarHorizontalGraph in the current
    //    plot and sets their shift/scale so they form a nice plot with
    //    side-by-side groups
    graph1->autoscaleBarWidthAndShift(0.75, 1);

    if (dynamic_cast<JKQTPBarVerticalGraph*>(graph1)!=nullptr) {
        // 7. data is grouped into 5 numbere groups (1..5), but we also have string
        //    labels for these groups (stored in L). In order to display these labels,
        //    we have to tell the x-Axis to use these special labels:
        plot->getXAxis()->addAxisTickLabels(X, L, Ndata);
        //    also we can rotate the labels a bit (by 45 degree), so they fit better
        plot->getXAxis()->setTickLabelAngle(45);
        plot->getXAxis()->setTickLabelFontSize(12);
    } else {
        // 7. data is grouped into 5 numbere groups (1..5), but we also have string
        //    labels for these groups (stored in L). In order to display these labels,
        //    we have to tell the x-Axis to use these special labels:
        plot->getYAxis()->addAxisTickLabels(X, L, Ndata);
        plot->getYAxis()->setTickLabelFontSize(12);
    }

    // 8. finally we move the plot key/legend to the outside, top-right
    //    and lay it out as a single row
    //    NOTE: plot is a descendent of QWidget, which uses an internal object of
    //          type JKQTBasePlotter, which does the actual plotting.
    //          So many properties of the plot are only available in this internal
    //          object, which you can access by plot->getPlotter().
    plot->getPlotter()->setKeyPosition(JKQTPKeyOutsideTopRight);
    plot->getPlotter()->setKeyLayout(JKQTPKeyLayoutOneRow);

    // 9 autoscale the plot so the graph is contained
    plot->zoomToFit();

    // show plotter and make it a decent size
    plot->show();
    plot->resize(600,400);
}

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    doExample<JKQTPBarVerticalGraph>();
    doExample<JKQTPBarHorizontalGraph>();

    return app.exec();
}
