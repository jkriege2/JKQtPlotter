# Tutorial (JKQTPlotter): Basic Usage of JKQTPDatastore             {#JKQTPlotterBasicJKQTPDatastore}

This tutorial project (see `./examples/simpletest_datastore/`) explains several options of JKQTPDatastore, which is the class used to centrally store the data for (most) graphs on a JKQTPlotter widget.

[TOC]

The source code of the main application can be found in [`jkqtplotter_simpletest_datastore.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/simpletest_datastore/jkqtplotter_simpletest_datastore.cpp). 
This tutorial cites parts of this code to demonstrate different ways of working with data for the graphs.

In every code-segment below, we will use these two declarations from the code to access the internal datastore of the JKQTPlotter instance:
```.cpp
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* datastore=plot.getDatastore();
```

# Copy Data from different data structures into JKQTPDatastore

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

The plot from the code above looks like this:

![simpletest_datastore_sine](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_sine.png)


## Copy Data from a C-array into a Column of the JKQTPDatastore

Of course if you have your data in a C-array, you can use the same syntax:
```.cpp
    #define NDATA 5
    double X[NDATA]= { 1, 2, 3, 4, 5 };
    double Y[NDATA]= { 1, 0, 1, 0, 1 };
    linegraph->setXColumn(datastore->addCopiedColumn(X, NDATA, "x"));
    linegraph->setYColumn(datastore->addCopiedColumn(Y, NDATA, "Y"));
```


The plot from the code above looks like this:

![simpletest_datastore_linkedcarray](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_linkedcarray.png)


## Copy Data from a Map into a JKQTPDatastore

Since graphs often display (x,y)-pairs, it may make sense to store them in a map (e.g. for histograms). There there are also functions that copy the contents of a map into a JKQTPDatastore, resulting in two columns beeing added:
```.cpp
    std::map<int, double> datamap;
    datamap[1]=1.1;
    datamap[2]=1.4;
    datamap[4]=1.2;
    datamap[5]=1.8;
    datamap[7]=0.9;
    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
    linegraph->setXYColumns(datastore->addCopiedMap(datamap, "map_x", "map_y"));
    linegraph->setTitle(QObject::tr("copied map"));
```
This code results in a graph like this:

![simpletest_datastore_map](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_map.png)


# Reference External Data in a column of the JKQTPDatastore

## Referencing without transfer of ownership

As an alternative to the method of copying data (see above), you could also just link the data. For this to work, the data has to reside in a C-array of type `double`, as this is the internal datatype of the `JKQTPDatastore`. You can simply replace the two lines with `JKQTPDatastore::addCopiedColumn()` in the example above by (we exploit the fact that `QVector<double>::data()` returns a pointer to the internal C-array of the vector):
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

Note however that the ownership of the data is not transfered to the JKQTPDatastore, i.e. you have to ensure that the data behind the pointer exists, as long as the column references the array and you have to ensure that the data is freed, when you don't need it any more.

The plot from the code above looks like this:

![simpletest_datastore_linkedcarray](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_linkedcarray.png)

## Referencing with Transfer of Ownership

In addition to the variants of `JKQTPDatastore::addColumn()`, that do not transfer ownership of the data to the `JKQTPDatastore`, you can also use `JKQTPDatastore::addInternalColumn()`, which tells the `JKQTPDatastore` to use the external data array and also take over its owner-ship. This implies that the array is freed when the `JKQTPDatastore` is destroyed, by calling `free()` in the array. Therefor data for this method needs to be allocated by using `malloc()` or `calloc()`:
```.cpp
    #define NDATA 5
    double* XCA=(double*)malloc(NDATA, sizeof(double));
    ...
    linegraph->setXColumn(datastore->addInternalColumn(XCA, NDATA, "x"));
```


# JKQTPDatastore-internal Data Management

It is also possible to leave the data mangement completely to the JKQTPDatastore and just edit the data with access functions from JKQTPDatastore.

## Generating Columns Non-Initialized Columns and Filling Them

The most basic way to generate data for a plot is to generate two non-initialized columns for the x- and y-coordinates of the graph points
```.cpp
    const int Ndata=100;
    size_t colX=datastore->addColumn(Ndata, "x");
    size_t colY=datastore->addColumn(Ndata, "y");
```
These calls to JKQTPDatastore::addColumn() will generate a column each with with `ndata` uninitialized entries.
Then use JKQTPDatastore::set() to fill them with data:
```.cpp
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*M_PI;
        datastore->set(colX, i, x);
        datastore->set(colY, i, sin(x));
    }
```

Plotting these two columns versus each other results in a simple sine graph:

![simpletest_datastore_sine](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_sine.png)

## Iterator Interface

Alternatively you can also access the column via a C++ iterator:
```.cpp
    auto itX=datastore->begin(colX))
    auto itY=datastore->begin(colY))
    for (; itX!=datastore->end(colX); ++itX, ++itY) {
        const double x=double(i)/double(Ndata)*8.0*M_PI;
        *itX=x;
        *itY=sin(x);
    }
```

This, together with `JKQTPDatastore::backInserter()` allows to use `JKQTDatastore` together with algorithms from the C++ standard template libarary and other templated algorithms based on the same iterator-based interfaces (e.g. in boost).




## Generating Columns Preinitialized Columns

For your convenience there are also function that simply create such a linear vector with one call:
```.cpp
    size_t colLinX=datastore->addLinearColumn(count, 0, 20, "x_lin");
```
This call is equivalent to
```.cpp
    for (size_t i=0; i<count; i++) {
        datastore->set(colLinX, i, 0.0+static_cast<double>(i)/static_cast<double>(count-1)*20.0);
    }
```

In both cases the column `colLinX` will contain these numbers afterwards:
```
    0, 0.512821, 1.02564, 1.53846, 2.05128, 2.5641, 3.07692, ..., 18.9744, 19.4872, 20
```

Comparable functions exist for logarithmically spaced columns (`JKQTPDatastore::addLogColumn()` and `JKQTPDatastore::addDecadeLogColumn()` which only differ in the way the start and end values are specified):
```.cpp
    size_t colLogX=datastore->addLogColumn(30, 1, 1000, "x_log");
```
This call results in a column with these 30 values spanning the range between 1 and 1000:
```
    1, 1.26896, 1.61026, 2.04336, ..., 8.53168, 10.8264, 13.7382, ..., 72.7895, 92.3671, ..., 788.046, 1000
```

## Appending to Columns

You can use the methods `JKQTPDatastore::appendToColumn()` and `JKQTPDatastore::appendFromContainerToColumn()` to extend columns with additional values, e.g.:
```.cpp
    for (double ii=10; ii<=20; ii++) datastore->appendToColumn(columnID, ii);
```
Note that this operation changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards! If the first append is called on a column that cannot be extended, the contents will be copied and the column will reference the new, internally managed, memory afterwards.

Alterantively there is also a `std::back_inserter`-like interface to append to a column:
```.cpp
    auto it=datastore->backInserter(columnID);
    for (double ii=10; ii<=20; ii++) *++it=ii;
```

This, together with `JKQTPDatastore::begin()` and `JKQTPDatatstore::end()` allows to use `JKQTDatastore` together with algorithms from the C++ standard template libarary and other templated algorithms based on the same iterator-based interfaces (e.g. in boost).



## Using Data from one Column to Calculate Another

After generating columns, as shown above, you can also use the data in these columns to calculate a second column based on the values in the first. You can do this explicitly:
```.cpp
    size_t colLinX=datastore->addLinearColumn(40, 0, 20, "x_lin");
    size_t colFunc1=datastore->addColumn(datastore->getRows(colLinX), "cos(x_lin)*x_lin/20.0");
    for (size_t i=0; i<datastore->getRows(colLinX); i++) {
        double x=datastore->get(colLinX, i);
        datastore->set(colFunc1, i, cos(x)*x/20.0);
    }
```
In this example the function JKQTPDatastore::get() is used to retrieve a value from a column and then JKQTPDatastore::set() is used to store a new value calculated from the read values into another column.

Or use a special function that gets a column with values and a functor f: double->double as parameters:
```.cpp
    size_t colLinX=datastore->addLinearColumn(40, 0, 20, "x_lin");
    size_t colFunc1=datastore->addColumnCalculatedFromColumn(colLinX, [](double x)->double { return cos(x)*x/20.0; }, "cos(x_lin)*x_lin/20.0");
```

Plotting these two columns versus each other 
```.cpp
    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
    linegraph->setXColumn(colLinX);
    linegraph->setYColumn(colFunc1);
    linegraph->setTitle(QObject::tr("calculated column(s)"));
```

results in:

![simpletest_datastore_calccolumns](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_calccolumns.png)


## 2D-Datasets and Images

There is also a function `JKQTPDatastore::addLinearGridColumns(size_t width, double startX, double endX, size_t height, double startY, double endY, const QString &nameX, const QString &nameY)` that generate two columns simultaneously that conatin the x- and y-coordinates of the points on a rectangular grid, in a column-major order:
```.cpp
    std::pair<size_t,size_t> colLinXY=datastore->addLinearGridColumns(10, 10, 20, 10, 1.5, 3);
```
This call will generate these columns with x-values scaling between 10 and 20 (in 10 steps) and y-values between 1.5 and 3 (also in 10 steps):
```
     x          y
  ---------------------- 
    10,         1.5
    11.1111,    1.5
    12.2222,    1.5
    13.3333,    1.5
    14.4444,    1.5
    15.5556,    1.5
    16.6667,    1.5
    17.7778,    1.5
    18.8889,    1.5
    20,         1.5
    10,         1.66667
    11.1111,    1.66667
    12.2222,    1.66667
    13.3333,    1.66667
    14.4444,    1.66667
    15.5556,    1.66667
    16.6667,    1.66667
    17.7778,    1.66667
    18.8889,    1.66667
    20,         1.66667
    10,         1.83333
    11.1111,    1.83333
    ...         ...
    ...         ...
    18.8889,    3
    20,         3
```
Such x-y-coordinate columns are especially usefull when calculating data for an image (plotted by `JKQTPColumnMathImage`), or for a parametrized scatter plot (plotted by `JKQTPXYParametrizedScatterGraph`). To demonstrate this, we can can add another column with 10*10=100 entries and fill it with some values calculated from the the x and y-values in colLinXY:
```.cpp
    size_t imgColumn=datastore->addImageColumn(10, 10, "image values");
    for (size_t i=0; i<datastore->getRows(imgColumn); i++) {
        double x=datastore->get(colLinXY.first, i);
        double y=datastore->get(colLinXY.second, i);
        datastore->set(imgColumn, i, cos((x-15.0))/(x-15.0)*cos((y-2.0))/(x-2.0));
    }
```
Note that we used `JKQTPDatastore::addImageColumn(width,height,name)` here, which generates a column with `width*height` entries that are meant to be interpreted as a 2D image with width columns and height rows. This function therefore also save this image size in the column metadata and graphs can later extract this dimension and use it to set them up internally (e.g. JKQTPColumnMathImage::setImageColumn() does just that).
In the example above we used the simple JKQTPDatastore::set() store values into our image column. This function treats the column as linearized in row-major order. Alternatively you can also use code like this to access the imageColumn as a real image or matrix:
```.cpp
    for (int iy=0; iy<10; iy++) {
        for (int ix=0; ix<10; ix++) {
            datastore->setPixel(imgColumn, ix, iy, sin(ix*iy));
        }
    }
```
Drawing this column as an image (using JKQTPColumnMathImage) will look like this:

![simpletest_datastore_sineimg](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_sineimg.png)

In these examples, the iteration and calculation is written out with an explicit for-loop. JKQTPDatastore also offers a shortcut function for this that accepts a functor with the calculation only:
```.cpp
    imgColumn=datastore->addColumnCalculatedFromColumn(colLinXY.first, colLinXY.second, 
                                                       [](double x, double y)->double { 
                                                           return cos((x-15.0))/(x-15.0)*cos((y-2.0))/(x-2.0); 
                                                       }, 
                                                       "image value");
```

Then you can plot these as a parametrized scatter graph (`JKQTPXYParametrizedScatterGraph`) using:
```.cpp
    plot.addGraph(paramscattergraph=new JKQTPXYParametrizedScatterGraph(&plot));
    paramscattergraph->setXYColumns(colLinXY);
    paramscattergraph->setColorColumn(imgColumn);
    paramscattergraph->setTitle(QObject::tr("parametrized scatter"));
```
... or alternatively you can only use the column imgColumn in a `JKQTPColumnMathImage`
```.cpp
    plot.addGraph(imggraph=new JKQTPColumnMathImage(&plot));
    imggraph->setImageColumn(imgColumn);
    imggraph->setX(21);
    imggraph->setY(1.5);
    imggraph->setWidth(10);
    imggraph->setHeight(1.5);
    imggraph->setTitle(QObject::tr("imgColumn"));
```
The result will look like this (`JKQTPXYParametrizedScatterGraph` on the left and `JKQTPColumnMathImage` on the right):

![simpletest_datastore_image](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_image.png)


