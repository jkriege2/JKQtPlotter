# Example (JKQTPlotter): Vector Field Plot Example {#JKQTPlotterVectorFieldExample}
This project (see [`vectorfield`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/vectorfield) demonstrates the use of JKQTPXYvectorfield to add labels to the datapoints of a graph. 

The source code of the main application is (see [`vectorfield.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/vectorfield/vectorfield.cpp).

Here is a short summary of the important parts of the code:

```.cpp
    // 1. setup a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. make up some arbitrary data to be used for plotting
    //    this generates a 2D grid of x/y-coordinates and then calculates dx=cos(y) and dy=sin(x)
    const auto columnXY=ds->addLinearGridColumns(NX, 0, 6, NY, -3, 3,"x","y");
    const auto columnDX=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return sin(y); });
    const auto columnDY=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return cos(x); });

    // 3. create JKQTPVectorFieldGraph to display the data:
    JKQTPVectorFieldGraph* graph1=new JKQTPVectorFieldGraph(&plot);
    graph1->setXYColumns(columnXY);
    graph1->setDxColumn(columnDX);
    graph1->setDyColumn(columnDY);
    graph1->setTitle(QObject::tr("$\\vec{f}(x,y)=\\bigl[\\sin(y), \\cos(x)\\bigr]^\\mathrm{T}$"));

    // 4. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph1);
```


The result looks like this:

![vectorfield](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/vectorfield.png)


