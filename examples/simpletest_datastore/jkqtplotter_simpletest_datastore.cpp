/** \example jkqtplotter_simpletest_datastore.cpp
 * A very basic example for the usage of JKQTPlotter
 *
 * \ref JKQTPlottersimpletest_datastore
 */
 
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsscatter.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* datastore=plot.getDatastore();

    JKQTPXYLineGraph* linegraph;

    // 2. first we create data inside a QVector for a simple plot (a sine curve) ... and add the plot
    //    note that you could use a std::vector equally well
    QVector<double> X, Y;
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*M_PI;
        X<<x;
        Y<<sin(x);
    }
    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
    // by calling JKQTPDatastore::addCopiedColumn() the data is COPIED from the vector into the datastore
    linegraph->setXColumn(datastore->addCopiedColumn(X, "x"));
    linegraph->setYColumn(datastore->addCopiedColumn(Y, "y"));
    // alternatively you can also tell JKQTPDatastore to just reference an external array:
    //linegraph->setXColumn(datastore->addColumn(X.data(), X.size(), "x"));
    //linegraph->setYColumn(datastore->addColumn(Y.data(), Y.size(), "Y"));
    linegraph->setTitle(QObject::tr("sine graph"));



    // 3. Now we generate a plot from data in a C-array, just reference in the JKQTPDatastore
    #define NDATA 5
    double XCA[NDATA]= { 1, 2, 3, 4, 5 };
    double YCA[NDATA]= { 1, 0, 1, 0, 1 };
    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
    linegraph->setXColumn(datastore->addColumn(XCA, NDATA, "xca (C-array)"));
    linegraph->setYColumn(datastore->addColumn(YCA, NDATA, "yca (C-array)"));
    // of course you could also simply copy the data with a comparable syntax:
    //linegraph->setXColumn(datastore->addCopiedColumn(XCA, NDATA, "xca (C-array)"));
    //linegraph->setYColumn(datastore->addCopiedColumn(YCA, NDATA, "yca (C-array)"));
    linegraph->setTitle(QObject::tr("linked C-array data"));


    // 4. Since graphs often display (x,y)-pairs, it may make sense to store them in a map (e.g. for histograms)
    //    There there are also functions that copy the contents of a map into a JKQTPDatastore, resulting in
    //    two columns beeing added:
    std::map<int, double> datamap;
    datamap[1]=1.1;
    datamap[2]=1.4;
    datamap[4]=1.2;
    datamap[5]=1.8;
    datamap[7]=0.9;
    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
    linegraph->setXYColumns(datastore->addCopiedMap(datamap, "map_x", "map_y"));
    linegraph->setTitle(QObject::tr("copied map"));



    // 5. It is also possible to leave the data mangement completely to the JKQTPDatastore
    //    and just edit the data with access functions from JKQTPDatastore:
    //   4.1 this adds a column with 10 values, linearly spaced between 5 and 10 (inclusive).
    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
    size_t colLinX=datastore->addLinearColumn(40, 0, 20, "x_lin");
    linegraph->setXColumn(colLinX);
    linegraph->setYColumn(datastore->addColumnCalculatedFromColumn(colLinX, [](double x)->double { return cos(x)*x/20.0; }, "cos(x_lin)*x_lin/20.0"));
    linegraph->setTitle(QObject::tr("calculated column(s)"));



    // 6. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
