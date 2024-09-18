# Example (JKQTPlotter): Using a QScrollbar together with JKQtPlotter             {#JKQTPlotterUIScrollbar}
This project (see `./examples/ui_bind_scrollbar/`) shows how to use JKQTPlotter together with a <a href="https://doc.qt.io/qt-6/qscrollbar.html">QScrollBar</a> for panning.

The source code of the main application can be found in [`ui_bind_scrollbar.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/ui_bind_scrollbar/ui_bind_scrollbar.cpp).

First we create a QWidget, a plot and a QScrollBar in a layout:

```.cpp
    QWidget win;
    QVBoxLayout* lay=new QVBoxLayout();
    win.setLayout(lay);

    JKQTPlotter* plot=new JKQTPlotter(&win);
    lay->addWidget(plot);
    // add a QScrollBar below the plot
    QScrollBar* scroll=new QScrollBar(Qt::Horizontal, &win);
    scroll->setMinimum(0);
    scroll->setMaximum(100);
    scroll->setPageStep(10);
    lay->addWidget(scroll);
```

Then we add a plot ranging from x=0 to x=100, with 10000 datapoints:

```.cpp
    // 4. create a graph
    JKQTPDatastore* ds=plot->getDatastore();
    const int NPOINTS=10000;
    JKQTPXYLineGraph* graph=new JKQTPXYLineGraph(plot);
    const size_t colX=ds->addLinearColumn(NPOINTS, 0, 100, "x");
    graph->setXColumn(colX);
    graph->setYColumn(ds->addCalculatedColumnFromColumn(colX, [](double x) { return 10.0*sin(x*3.0)*fabs(cos((x/8.0))); }, "f(x)"));
    graph->setDrawLine(true);
    graph->setSymbolType(JKQTPNoSymbol);
    // ... add the graphs to the plot, so it is actually displayed
    plot->addGraph(graph);
```

The plot axis range is limited to the plot range and zooming in y-direction is disabled

```.cpp
    plot->setAbsoluteXY(0,100,-10,10);
    // show everything in y-direction
    plot->setY(-10,10);
    // fix y-range, so no zoming occurs in y
    plot->getYAxis()->setRangeFixed(true);
```

Now we need a slot for the QScrollBar (here implemented as a functor), which  calculates the desired view in the graph and sets it. Note the blockSignals()-calls that stop the plot from sending signals back to the scrollbar, which would cause a strange loop due to the fact that the scrollbar only accepts integers for range and value!

```.cpp
    QObject::connect(scroll, &QScrollBar::valueChanged, [plot,scroll](int value) {
        const double scrollRange=scroll->maximum()-scroll->minimum()+scroll->pageStep();
        const double scrollPos=scroll->value();
        const double scrollPageSize=scroll->pageStep();
        const double scrollRelative=scrollPos/scrollRange;
        const double plotFullRange=plot->getAbsoluteXMax()-plot->getAbsoluteXMin();
        const double plotViewRange=scrollPageSize/scrollRange*plotFullRange;
        const double plotViewStart=plot->getAbsoluteXMin()+scrollRelative*plotFullRange;
        plot->blockSignals(true);
        plot->setX(plotViewStart, plotViewStart+plotViewRange);
        plot->blockSignals(false);
    });
    scroll->setValue(1); // ensure to call slot once!
```

A second functor catches whenever the user zooms or pans (or otherwise changes the axis ranges) of the plot by catching the signal JKQTPlotter::zoomChangedLocally() and from that calculates the value (and pageStep) for the QScrollBar:

```.cpp
    QObject::connect(plot, &JKQTPlotter::zoomChangedLocally, [scroll](double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* plot) {
        const double plotFullRange=plot->getAbsoluteXMax()-plot->getAbsoluteXMin();
        const double plotViewRange=newxmax-newxmin;
        const double plotRelViewRange=(plotViewRange>=plotFullRange)?1.0:(plotViewRange/(plotFullRange-plotViewRange));
        const double plotRelViewStart=(newxmin-plot->getAbsoluteXMin())/plotFullRange;
        const double scrollRange=scroll->maximum()-scroll->minimum();
        scroll->blockSignals(true);
        scroll->setPageStep(plotRelViewRange*scrollRange);
        scroll->setValue(plotRelViewStart*scrollRange);
        scroll->blockSignals(false);
    });
```

The window from this example looks like this:

![ui_bind_scrollbar](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/ui_bind_scrollbar.png)


With the same method, it is also possible to add x- and y-scrollbars:

![ui_bind_scrollbar](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/ui_bind_scrollbar_win01.png)
