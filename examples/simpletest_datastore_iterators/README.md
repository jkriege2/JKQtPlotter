# Tutorial (JKQTPDatastore): Iterator-Based usage of JKQTPDatastore             {#JKQTPlotterBasicJKQTPDatastoreIterators}


[JKQTPlotterBasicJKQTPDatastore]: @ref JKQTPlotterBasicJKQTPDatastore "Basic Usage of JKQTPDatastore"
[JKQTPlotterBasicJKQTPDatastoreIterators]: @ref JKQTPlotterBasicJKQTPDatastoreIterators "Iterator-Based usage of JKQTPDatastore"
[JKQTPlotterBasicJKQTPDatastoreStatistics]: @ref JKQTPlotterBasicJKQTPDatastoreStatistics "Advanced 1-Dimensional Statistics with JKQTPDatastore"
[JKQTPlotterBasicJKQTPDatastoreRegression]: @ref JKQTPlotterBasicJKQTPDatastoreRegression "Regression Analysis (with the Statistics Library)"
[JKQTPlotterBasicJKQTPDatastoreStatistics2D]: @ref JKQTPlotterBasicJKQTPDatastoreStatistics2D "Advanced 2-Dimensional Statistics with JKQTPDatastore"
[statisticslibrary]: @ref jkqtptools_math_statistics "JKQTPlotter Statistics Library"

This tutorial project (see `./examples/simpletest_datastore_iterators/`) explains how to use the iterator-based interface to JKQTPDatastore.

***Note*** that there are additional tutorial explaining other aspects of data mangement in JKQTPDatastore:
  - [JKQTPlotterBasicJKQTPDatastore]
  - [JKQTPlotterBasicJKQTPDatastoreIterators]
  - [JKQTPlotterBasicJKQTPDatastoreStatistics]
  - [JKQTPlotterBasicJKQTPDatastoreStatistics2D]
  - [JKQTPlotterBasicJKQTPDatastoreRegression]



[TOC]

The source code of the main application can be found in [`jkqtplotter_simpletest_datastore_iterators.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/simpletest_datastore_iterators/jkqtplotter_simpletest_datastore_iterators.cpp). 
This tutorial cites parts of this code to demonstrate different ways of working with JKQTPDatastore's iterator-interface.

In every code-segment below, we will use these two declarations from the code to access the internal datastore of the JKQTPlotter instance:
```.cpp
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* datastore=plot.getDatastore();
```

In the example [JKQTPlotterBasicJKQTPDatastore] we discussed how to copy data from external container into and explicitly access data in columns inside a JKQTPDatastore. This tutorial explains how to use the iterator interface of JKQTPDatastore to access the data, build columns and also interact with algorithms from the C++ standard template library (or other iterator-based libraries, like e.g. boost). Also have a look at the [statisticslibrary] and [JKQTPlotterBasicJKQTPDatastoreStatistics], as these also use the iterator-interface of JKQTPDatastore.

# Iterator-based Column Data Access To Existing Rows

In other tutorials we used e.g. `JKQTPDatastore::set()` to set values in data columns. Using this scheme, you can write code like shown below to draw a cose curve:

```.cpp
    size_t XCol=datastore->addLinearColumn(50, 0, 4.0*M_PI, "cos curve: x-data");
    size_t YCol=datastore->addColumn(datastore->getRows(XCol), "cos curve: y-data");
    for (size_t i=0; i<datastore->getRows(XCol); i++) {
        datastore->set(YCol, i, cos(datastore->get(XCol, i)));
    }
```
Here we added two columns with 50 entries. `XCol` contains linearly spaced values between 0 and 2*pi and `YCol` contains 50 uninitialized values. Then we iterate an index `i` over all these items (`datastore->getRows(XCol)` returns the rows in a column, i.e. 50 in the example above) and used `JKQTPDatastore::set()` to store the calculated values in the two columns. The current x-values is read from `XCol` using `JKQTPDatastore::get()`. The resulting plot looks like this:

![simpletest_datastore_iterators_cosine](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_iterators_cosine.png)

The same loop can be written using iterators:
```.cpp
    size_t XCol=datastore->addLinearColumn(50, 0, 4.0*M_PI, "cos curve: x-data");
    size_t YCol=datastore->addColumn(datastore->getRows(XCol), "cos curve: y-data");
    auto itY=datastore->begin(YCol);
    for (auto itX=datastore->begin(XCol); itX!=datastore->end(XCol); ++itX, ++itY) {
        *itY=cos(*itX);
    }
```

# Back-Inserter for Columns

Above we used two previously sized columns and accessed (read and writing) existing rows in them. But `JKQTPDatastore` also provides an iterator comparable to [`std::back_inserter`](https://de.cppreference.com/w/cpp/iterator/back_inserter), which allows to add rows at the end of an existing (here initially empty) column:
```.cpp
    size_t XCol=datastore->addColumn("cos curve: x-data");
    size_t YCol=datastore->addColumn("cos curve: y-data");
    auto biXCol=datastore->backInserter(XCol);
    auto biYCol=datastore->backInserter(YCol);
    for (double x=0; x<4.0*M_PI; x+=4.0*M_PI/50.0) {
        *++biXCol=x;
        *++biYCol=cos(x);
    }
```

# Using C++ STL algorithms

You can write this a bit more compact, if you use `JKQTPDatastore::addLinearColumn()` and the C++ STL-algorithm [`std::transform()`](https://de.cppreference.com/w/cpp/algorithm/transform):
```.cpp
    size_t XCol=datastore->addLinearColumn(50, 0, 4.0*M_PI, "cos curve: x-data");
    size_t YCol=datastore->addColumn("cos curve: y-data");
    std::transform(datastore->begin(XCol), datastore->end(XCol), datastore->backInserter(YCol), &cos);
```

Of course you can now also interface other algorithms, like e.g. [`std::sort()`](https://de.cppreference.com/w/cpp/algorithm/sort):
```.cpp
    std::sort(datastore->begin(colY), datastore->end(colY));
```

With this line of code, the `YCol` is sorted in ascending order and the plot becomes:

![simpletest_datastore_iterators_cosine_ysorted](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_iterators_cosine_ysorted.png)

Another example would be to replace all value `y<-0.5` with the value `1.0` using  [`std::replace_if()`](https://de.cppreference.com/w/cpp/algorithm/replace):
```.cpp
    std::replace_if(datastore->begin(YCol), datastore->end(YCol), [](double v) { return v<-0.5; }, 1.0);
```

![simpletest_datastore_iterators_cosine_yreplaced](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/simpletest_datastore_iterators_cosine_yreplaced.png)

Finally also the [erase-remove idiom](https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom) (e.g. known from `std::vector`) is supported:
```.cpp
    datastore->eraseFromColumn(std::remove_if(datastore->begin(YCol), datastore->end(YCol), [](double v) { return v<0; }), datastore->end(YCol));
```

# Special Properties of the JKQTPDatastore-Iterators

Note that the iterator classes of `JKQTPDatastore` (namely `JKQTPColumnIterator` and `JKQTPColumnConstIterator`) provide additional function to access the properties of the data-column row they point to:
  - `JKQTPColumnIterator::isValid()` checks whether the iterator points to a valid row in a column. it is `false` e.g. for an iterator returned by `JKQTPDatastore::end()`
  - `JKQTPColumnIterator::getPosition()` returns the row/position inside the column the iterator points to
  - `JKQTPColumnIterator::getImagePosition()` / `JKQTPColumnIterator::getImagePositionX()` / `JKQTPColumnIterator::getImagePositionY()` return the x-/y-location of the pointed-to pixel in an image column
  - `JKQTPColumnIterator::getImageColumns()` / `JKQTPColumnIterator::getImageRows()` return the width/height of the image represented by the image column (the pointed-to pixel is part of)
  