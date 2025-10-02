# Example (JKQTPlotter): Barchart With Functor Fill-Mode                                      {#JKQTPlotterBarchartsFunctorFilling}


This project (see [`barchart_functorfill`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/barchart_functorfill) shows how to draw barcharts, where the bars are filled differently, as defined by a custom functor.

The source code of the main application is (see [`barchart_functorfill.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/barchart_functorfill/barchart_functorfill.cpp):
```.cpp
        // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create two columns for key and value
    size_t columnK=ds->addLinearColumn(11, 0, 10, "k");
    size_t columnV=ds->addColumnCalculatedFromColumn(columnK, [](double x) { return 5.0+x; }, "v");

    // 3. create graph in the plot, which plots the dataset:
    JKQTPBarVerticalGraph* graph=new JKQTPBarVerticalGraph(&plot);
    graph->setKeyColumn(columnK);
    graph->setValueColumn(columnV);
    // set FunctorFilling fill Mode
    graph->setFillMode(JKQTPBarGraphBase::FillMode::FunctorFilling);
    // define filling functor
    graph->setFillBrushFunctor(
      [](double key, double value) {
        return QBrush(QColor::fromHsvF(key/12.0, 1.0, 1.0)); 
      }
    );
    plot.addGraph(graph);

    // 4 autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.setWindowTitle(title);
    plot.show();
    plot.resize(400,400);
```


The result looks like this:

![barchart_functorfill](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/barchart_functorfill.png)



In order to draw horizontal error bars, you have to use `JKQTPBarHorizontalGraph` instead of `JKQTPBarVerticalGraph`:

![barchart_functorfill_hor](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/barchart_functorfill_hor.png)


