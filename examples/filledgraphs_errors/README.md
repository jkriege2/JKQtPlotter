# Example (JKQTPlotter): Filled Graphs with Error Indicators {#JKQTPlotterFilledGraphsErrorBars}
This project (see [`filledgraphs_errors`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/filledgraphs_errors) shows how to draw filled graphs with different styles of error indicators.

The source code of the main application is (see [`filledgraphs_errors.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/filledgraphs_errors/filledgraphs_errors.cpp):
```.cpp
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create two columns for key and value
    size_t columnK=ds->addLinearColumn(10, 0, 2.0*JKQTPSTATISTICS_PI,"k");
    size_t columnV=ds->addColumnCalculatedFromColumn(columnK, &cos, "v");

    // 3. create graph in the plot, which plots the dataset:
    JKQTPBarGraphBase* graph=new JKQTPBarVerticalGraph(&plot);
    graph->setKeyColumn(columnK);
    graph->setValueColumn(columnV);
    // set TwoColor fill Mode
    graph->setFillMode(JKQTPBarGraphBase::FillMode::TwoColorFilling);
    graph->setFillColor(QColor("green"));
    graph->fillStyleBelow().setFillColor(QColor("red"));
    plot.addGraph(graph);

    // 4 autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 5. show plotter and make it a decent size
    plot.setWindowTitle(title);
    plot.show();
    plot.resize(400,400);
```


The result looks like this:

![filledgraphs_errors](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/filledgraphs_errors.png)



In order to draw horizontal error bars, you have to use `JKQTPBarHorizontalGraph` instead of `JKQTPBarVerticalGraph`:

![filledgraphs_errors_hor](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/filledgraphs_errors_hor.png)


