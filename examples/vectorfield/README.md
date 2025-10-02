# Example (JKQTPlotter): Vector Field Plot Example                                      {#JKQTPlotterVectorFieldExample}


This project (see [`vectorfield`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/vectorfield) demonstrates the use of JKQTPVectorFieldGraph to visualize a vector field. 

The source code of the main application is (see [`vectorfield.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/vectorfield/vectorfield.cpp).

Here is a short summary of the important parts of the code:

```.cpp
    // 1. setup a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. make up some arbitrary data to be used for plotting
    //    this generates a 2D grid of x/y-coordinates and then calculates dx=cos(y)*sqrt(x/3.0) and dy=sin(x)*sqrt(x/3.0)
    const auto columnXY=ds->addLinearGridColumns(NX, 0, 6, NY, -3, 3,"x","y");
    const auto columnDX=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return sin(y)*sqrt(x/3.0); });
    const auto columnDY=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return cos(x)*sqrt(x/3.0); });

    // 3. create JKQTPVectorFieldGraph to display the data:
    JKQTPVectorFieldGraph* graph1=new JKQTPVectorFieldGraph(&plot);
    graph1->setXYColumns(columnXY);
    graph1->setDxColumn(columnDX);
    graph1->setDyColumn(columnDY);
        graph1->setTitle(QObject::tr("$\\vec{f}(x,y)=\\bigl[\\sin(y)\\cdot\\sqrt{x/3}, \\cos(x)\\cdot\\sqrt{x/3}\\bigr]^\\mathrm{T}$"));

    // 4. add the graphs to the plot, so it is actually displayed
    plot.addGraph(graph1);
```


The result looks like this:

![vectorfield](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/vectorfield.png)


By default, the length of the drawn vector is determined from the actual length in the data via an autoscaling algorithm that is supposed to prevent the vectors from overlapping.
But you can modify this behaviour by adding a line 

```.cpp
        graph1->setVectorLengthMode(JKQTPVectorFieldGraph::LengthFromData);
```

which will use the given lengths directly (only scaled by an optional factor defined by JKQTPVectorFieldGraph::setLengthScaleFactor() ). The result then looks like this:

![vectorfield](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/JKQTPVectorFieldGraphLengthFromData.png)

Alternatively you can also set 

```.cpp
        graph1->setVectorLengthMode(JKQTPVectorFieldGraph::IgnoreLength);
```

which will draw all vectors with the same length. You can scale this length by setting  JKQTPVectorFieldGraph::setAutoscaleLengthFactor() ). The result then looks like this:

![vectorfield](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/JKQTPVectorFieldGraphIgnoreLength.png)


Finally it is also possibly to scale the vector's line width with its length:

```.cpp
        graph1->setVectorLengthMode(JKQTPVectorFieldGraph::AutoscaleLength);
        graph1->setVectorLineWidthMode(JKQTPVectorFieldGraph::AutoscaleLineWidthFromLength);
```

This will result in a plot like this:

![vectorfield](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/JKQTPVectorFieldGraphAutoscaleLengthAutoscaleLineWidthFromLength.png)
