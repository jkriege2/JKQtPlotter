# Example (JKQTPlotter): Filled Graphs with Error Indicators {#JKQTPlotterFilledGraphsErrorBars}
This project (see [`filledgraphs_errors`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/filledgraphs_errors) shows how to draw filled graphs with different styles of error indicators.

The source code of the main application is (see [`filledgraphs_errors.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/filledgraphs_errors/filledgraphs_errors.cpp):
```.cpp
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. now we create three columns for key and value
    size_t columnK=ds->addLinearColumn(9, 0.1*JKQTPSTATISTICS_PI, 1.0*JKQTPSTATISTICS_PI,"k");
    size_t columnK2=ds->addLinearColumn(9, 1.1*JKQTPSTATISTICS_PI, 2.0*JKQTPSTATISTICS_PI,"k2");
    size_t columnK3=ds->addLinearColumn(9, 2.1*JKQTPSTATISTICS_PI, 3.0*JKQTPSTATISTICS_PI,"k2");
    size_t columnV=ds->addColumnCalculatedFromColumn(columnK, [](double x) { return sin(x); }, "v");
    size_t columnV2=ds->addColumnCalculatedFromColumn(columnK2, [](double x) { return -sin(x); }, "v2");
    size_t columnV3=ds->addColumnCalculatedFromColumn(columnK3, [](double x) { return sin(x); }, "v3");
    size_t columnE=ds->addColumnCalculatedFromColumn(columnV, [](double x) { return 0.2*x; }, "error");
    size_t columnE2=ds->addColumnCalculatedFromColumn(columnV2, [](double x) { return 0.2*x; }, "error");
    size_t columnE3=ds->addColumnCalculatedFromColumn(columnV3, [](double x) { return 0.2*x; }, "error");

    // 3. create a graph in the plot, which plots the dataset with symmetric:
    JKQTPFilledCurveXErrorGraph* graph1=new JKQTPFilledCurveXErrorGraph(&plot);
    graph1->setKeyColumn(columnK);
    graph1->setValueColumn(columnV);
    graph1->setValueErrorColumn(columnE);
    // set error indicator style
    graph1->setValueErrorStyle(JKQTPErrorBars);
    graph1->setTitle(QObject::tr("JKQTPErrorBars"));
    plot.addGraph(graph1);

    // 4. create a second graph in the plot, which plots the second dataset with outer error bars only:
    JKQTPFilledCurveXErrorGraph* graph2=new JKQTPFilledCurveXErrorGraph(&plot);
    graph2->setKeyColumn(columnK2);
    graph2->setValueColumn(columnV2);
    graph2->setValueErrorColumn(columnE2);
    // set error indicator style
    graph2->setValueErrorStyle(JKQTPErrorPolygons);
    graph2->setTitle(QObject::tr("JKQTPErrorPolygons"));
    plot.addGraph(graph2);

    // 5. create a third graph in the plot, which plots the second dataset with outer error bars only:
    JKQTPFilledCurveXErrorGraph* graph3=new JKQTPFilledCurveXErrorGraph(&plot);
    graph3->setKeyColumn(columnK3);
    graph3->setValueColumn(columnV3);
    graph3->setValueErrorColumn(columnE3);
    // set error indicator style
    graph3->setValueErrorStyle(JKQTPErrorLines);
    graph3->setTitle(QObject::tr("JKQTPErrorLines"));
    plot.addGraph(graph3);


    // 6 autoscale the plot so the graph is contained
    plot.zoomToFit();

    // 7. show plotter and make it a decent size
    plot.getPlotter()->setKeyPosition(JKQTPKeyInsideBottomLeft);
    plot.setWindowTitle(title);
    plot.show();
    plot.resize(400,400);
```


The result looks like this:

![filledgraphs_errors](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/filledgraphs_errors.png)



In order to draw horizontal error bars, you have to use `JKQTPFilledCurveYErrorGraph` instead of `JKQTPFilledCurveXErrorGraph`:

![filledgraphs_errors_hor](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/filledgraphs_errors_y.png)


