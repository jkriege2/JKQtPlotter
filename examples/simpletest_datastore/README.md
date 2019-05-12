# Example (JKQTPlotter): Advanced Usage of JKQTPDatastore             {#JKQTPlotterAdvancedJKQTPDatastore}
This project (see `./examples/simpletest_datastore/`) explains several advanced options of JKQTPDatastore, which is the class used to centrally store the data for (most) graphs on a JKQTPlotter widget.


The source code of the main application can be found in [`jkqtplotter_simpletest_datastore.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/simpletest_datastore/jkqtplotter_simpletest_datastore.cpp). 
This tutorial cites parts of this code to demonstrate different ways of working with data for the graphs.

In every code-segment below, we will use these two declarations from the code to access the internal datastore of the JKQTPlotter instance:
```.cpp
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* datastore=plot.getDatastore();
```

## Copy Data from a Vector into a column of the JKQTPDatastore
First we fill data into a QVector for a simple plot (a sine curve) and add a plot using this data:
```.cpp
    QVector<double> X, Y;
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*M_PI;
        X<<x;
        Y<<sin(x);
    }
```
Now the contents of the vector is copied into the datastore using `JKQTPDatastore::addCopiedColumn()`. After this operation, the contents of the vectors `X` and `Y` can be altered, as the `JKQTPDatastore` inside the `JKQTPlotter` now hosts its own copy of the data.
```.cpp
    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
    linegraph->setXColumn(ds->addCopiedColumn(X, "x"));
    linegraph->setYColumn(ds->addCopiedColumn(Y, "y"));
    linegraph->setTitle(QObject::tr("sine graph"));
```
Note that you could also use a `std::vector` instead, as `JKQTPDatastore::addCopiedColumn()` is a template function that only requires the container to support C++ standard iterations (with `begin()` and `end()`), as well as the function `size()` to determine the number of items in the container. Also other datatypes than `double` are possible, as long as they can be converted to `double` (the function `jkqtp_todouble()` is used to perform the conversion, notably all integer and floating-point types, as well as `bool` are supported out of the box).

Of course if you have your data in a C-array, you can use the same syntax:
```.cpp
    #define NDATA 5
    double X[NDATA]= { 1, 2, 3, 4, 5 };
    double Y[NDATA]= { 1, 2, 3, 2, 1 };
    linegraph->setXColumn(datastore->addCopiedColumn(X, NDATA, "x"));
    linegraph->setYColumn(datastore->addCopiedColumn(Y, NDATA, "Y"));
```


## Reference External Data in a column of the JKQTPDatastore
As an alternative to the method of copying data (see above), you could also just link the data. For this to work, the data has to reside in a C-array of type `double`, as this is the internal datatype of the `JKQTPDatastore`. You can simply replace the two lines with `JKQTPDatastore::addCopiedColumn()` in the example above by (we exploit the fact that `QVector<double>::data()` returns a pointer to the internal C-array of the vector:
```.cpp
    linegraph->setXColumn(datastore->addColumn(X.data(), X.size(), "x"));
    linegraph->setYColumn(datastore->addColumn(Y.data(), Y.size(), "Y"));
```
Of course if you have your data in a C-array, you can use the same syntax:
```.cpp
    #define NDATA 5
    double XCA[NDATA]= { 1, 2, 3, 4, 5 };
    double YCA[NDATA]= { 1, 0, 1, 0, 1 };
    linegraph->setXColumn(datastore->addColumn(XCA, NDATA, "x"));
    linegraph->setYColumn(datastore->addColumn(YCA, NDATA, "Y"));
```
This method is especially useful, when you have large datasets (e.g. images) that you don't want to copy around.

In addition to the variants of `JKQTPDatastore::addColumn()`, that do not transfer ownership of the data to the `JKQTPDatastore`, you can also use `JKQTPDatastore::addInternalColumn()`, which tells the `JKQTPDatastore` to use the external data array and also take over its owner-ship. This implies that the array is freed when the `JKQTPDatastore` is destroyed, by calling `free()` in the array. Therefor data for this method nees to be allocated using `malloc()` or `calloc()`:
```.cpp
    #define NDATA 5
    double* XCA=(double*)malloc(NDATA, sizeof(double));
    ...
    linegraph->setXColumn(datastore->addInternalColumn(XCA, NDATA, "x"));
```
