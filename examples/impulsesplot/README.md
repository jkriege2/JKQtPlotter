# Example (JKQTPlotter): Simple impulse plots                                      {#JKQTPlotterImpulsePlots}


This project (see `./examples/impulsesplot/`) simply creates a JKQTPlotter widget (as a new window) and adds a single impulse graph. 
The source code of the main application is (see [`impulsesplot.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/impulsesplot/impulsesplot.cpp).



First data for a curve is calculated and stored in `QVector<double>`: 
```.cpp
    QVector<double> X, Y;
    for (int i=0; i<Ndata; i++) {
        const double xx=double(i)/double(Ndata)*6.0*M_PI;
        X << xx;
        Y << cos(xx)*exp(-xx/10.0);
    }
```

... and finally the data is copied into the datastore
```.cpp
    size_t columnX=ds->addCopiedColumn(X,  "x");
    size_t columnY=ds->addCopiedColumn(Y,  "y");
```
	
Now an impulse graph object is generated and added to the plot:
```.cpp
    JKQTPImpulsesVerticalGraph* graph=new JKQTPImpulsesVerticalGraph(&plot);
    graph->setXColumn(columnX);
    graph->setYColumn(columnY);
	graph->setLineWidth(2);
	graph->setColor(QColor("red"));
    graph->setTitle(QObject::tr("$\\cos(x)\\cdot\\exp(-x/10)$"));

    plot.addGraph(graph);
```

The result looks like this:

![impulsesplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/impulsesplot.png)

There is an alternative class `JKQTPImpulsesHorizontalGraph` which draws horizontal impulse plots:
```.cpp
    JKQTPImpulsesHorizontalGraph* graph=new JKQTPImpulsesHorizontalGraph(&plot);
    graph->setYColumn(columnX);
    graph->setXColumn(columnY);
    graph->setLineWidth(2);
    graph->setColor(QColor("blue"));
    graph->setTitle(QObject::tr("$\\cos(x)\\cdot\\exp(-x/10)$"));
```

This code snippet results in a plot like this:

![impulsesplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/impulsesplot_horizontal.png)

The classes `JKQTPImpulsesVerticalGraph` and `JKQTPImpulsesHorizontalGraph` also provide the ability to draw a symbol at the end of the impulse, e.g. using this code:

```.cpp
    graph->setDrawSymbols(true);
    graph->setSymbolType(JKQTPGraphSymbols::JKQTPFilledStar);
```

This code snippet results in a plot like this:

![impulsesplot_symbols](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/impulsesplot_symbols.png)

Finally you can move the baseline (i.e. the level, where the impulses start, which is typically x=0 or y=0) in the classes `JKQTPImpulsesVerticalGraph` and `JKQTPImpulsesHorizontalGraph`:

```.cpp
    graph->setBaseline(0.25);
```

This code snippet results in a plot like this:

![impulsesplot_baseline](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/impulsesplot_baseline.png)

