/** \example datastore.cpp
 * Several basic examples of how to add data to and edit data in a JKQTPDatastore.
 *
 * \ref JKQTPlottersimpletest_datastore
 */
 
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include <algorithm>

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* datastore=plot.getDatastore();

    JKQTPXYLineGraph* linegraph;
    JKQTPColumnMathImage* imggraph;
    JKQTPXYParametrizedScatterGraph* paramscattergraph;


    /////////////////////////////////////////////////////////////////////////////////////////////////
    /// externally provided data (i.e. from different containers)
    /////////////////////////////////////////////////////////////////////////////////////////////////

    // 2. first we create data inside a QVector for a simple plot (a sine curve) ... and add the plot
    //    note that you could use a std::vector equally well
    QVector<double> X, Y;
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
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
    //    Note: JKQTPDatastore does not take ownership of your data!
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



    /////////////////////////////////////////////////////////////////////////////////////////////////
    /// internally managed data
    /////////////////////////////////////////////////////////////////////////////////////////////////

    // 5. It is also possible to leave the data mangement completely to the JKQTPDatastore
    //    and just edit the data with access functions from JKQTPDatastore:
    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
    //    5.1 this adds a column with 40 values, linearly spaced between 0 and 20 (inclusive).
    size_t colLinX=datastore->addLinearColumn(40, 0, 20, "x_lin");
    //    5.2 this adds a column with one entry for every entry x in the column colLinX, where
    //        the entry is calculated by applying a function cos(x)*x/20.0
    size_t colFunc1=datastore->addColumnCalculatedFromColumn(colLinX, [](double x)->double { return cos(x)*x/20.0; }, "cos(x_lin)*x_lin/20.0");
    //        the same can be done by this code explicitly:
    //          5.2.1 add a column with as many rows as column colLinX
    //size_t colFunc1=datastore->addColumn(datastore->getRows(colLinX), "cos(x_lin)*x_lin/20.0");
    //          5.2.2 iterate over the data in column colLinX and set a newly calculated value into a row of column colFunc1
    //for (size_t i=0; i<datastore->getRows(colLinX); i++) {
    //    double x=datastore->get(colLinX, i);
    //    datastore->set(colFunc1, i, cos(x)*x/20.0);
    //}
    linegraph->setXColumn(colLinX);
    linegraph->setYColumn(colFunc1);
    linegraph->setTitle(QObject::tr("calculated column(s)"));


    // 6. The function addLinearGridColumns() generates a rectangular 2D grid of coordinates
    //    in row-major order. Here we generate a 10x10 grid with x-coordinates between 10 and 20 (inclusive)
    //    and y-coordinates between 1.5 and 3:
    std::pair<size_t,size_t> colLinXY=datastore->addLinearGridColumns(10, 10, 20, 10, 1.5, 3);
    // now we can add another column with 10*10=100 entries and fill it with some values
    // calculated from the the x and y-values in colLinXY:
    size_t imgColumn=datastore->addImageColumn(10, 10, "image values");
    for (size_t i=0; i<datastore->getRows(imgColumn); i++) {
        double x=datastore->get(colLinXY.first, i);
        double y=datastore->get(colLinXY.second, i);
        datastore->set(imgColumn, i, cos((x-15.0))/(x-15.0)*cos((y-2.0))/(x-2.0));
    }
    // alternatively you can access image pixels with setPixel():
    //for (int iy=0; iy<10; iy++) {
    //    for (int ix=0; ix<10; ix++) {
    //        datastore->setPixel(imgColumn, ix, iy, sin(ix*iy/30.0));
    //    }
    //}
    // the loop above can be written more compact using addColumnCalculatedFromColumn():
    //imgColumn=datastore->addColumnCalculatedFromColumn(colLinXY.first, colLinXY.second, [](double x, double y)->double { return cos((x-15.0))/(x-15.0)*cos((y-2.0))/(x-2.0); },  "image value");
    // finally we can use a JKQTPXYParametrizedScatterGraph to display the data from our three columns
    // by using colLinXY->first and colLinXY->second as positions for symbols that are colored, based
    // on the respective value in imgColumn:
    plot.addGraph(paramscattergraph=new JKQTPXYParametrizedScatterGraph(&plot));
    paramscattergraph->setXYColumns(colLinXY);
    paramscattergraph->setColorColumn(imgColumn);
    paramscattergraph->setTitle(QObject::tr("parametrized scatter"));
    // alternatively you can only use the column imgColumn in a JKQTPColumnMathImage
    plot.addGraph(imggraph=new JKQTPColumnMathImage(&plot));
    imggraph->setImageColumn(imgColumn);
    imggraph->setX(21);
    imggraph->setY(1.5);
    imggraph->setWidth(10);
    imggraph->setHeight(1.5);
    imggraph->setTitle(QObject::tr("imgColumn"));




    // 6. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
