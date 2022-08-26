# Example (JKQTPlotter): Very simple scatter-graph             {#JKQTPlotterscatter}

This project (see `./examples/scatter/`) simply creates a JKQTPlotter widget (as a new window) and adds a single scatter graph of type JKQTPXYScatterGraph (a sine-wave with noise). 



The source code of the example can be found in [`jkqtplotter_scatter.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/scatter/scatter.cpp).

First we create a plotter window and get a pointer to the internal datastore (for convenience):
```.cpp
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();
```
Now we add two columns to the JKQTPDatastore and obtain back-inserter iterators for these:
```.cpp
    size_t columnX=ds->addColumn("x");
    auto colXInserter=ds->backInserter(columnX);
    size_t columnY=ds->addColumn("y");
    auto colYInserter=ds->backInserter(columnY);
```
Now we create data for a simple plot (a sine curve with random noise):
```.cpp
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0,0.5);
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        // put data
        const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
        *colXInserter=x;
        *colYInserter=sin(x)+distribution(generator);
        // advance back-inserters
        colXInserter++;
        colYInserter++;
    }

```
Here we create a graph in the plot, which plots the dataset X/Y:
```.cpp
    JKQTPXYScatterGraph* graph1=new JKQTPXYScatterGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY);
    graph1->setTitle(QObject::tr("sine graph"));
```
Now we add the graph to the plot, so it is actually displayed:
```.cpp
    plot.addGraph(graph1);
```
Finally we autoscale the plot so the graph is contained:
```.cpp
    plot.zoomToFit();
```
The result looks like this:

![jkqtplotter_scatter](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/scatter.png)



