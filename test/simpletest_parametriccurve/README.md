[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Plotting Parametric Curves
This project (see `./test/simpletest_parametriccurve/`) demonstrates how to draw parametric curves, using [`JKQTPxyLineGraph`}(https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest) and [`JKQTPxyParametrizedScatterGraph`](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_paramscatterplot). 

The source code of the main application can be found in  [`jkqtplotter_simpletest_parametriccurve.cpp`](https://github.com/jkriege2/JKQtPlotter/blob/master/test/simpletest_parametriccurve/jkqtplotter_simpletest_parametriccurve.cpp). First, the parametric curve (here a [logarithic spiral](https://en.wikipedia.org/wiki/Logarithmic_spiral)) is sampled into two columns containing the x- and y-values along the curve. In addition the radial distance from x=y=0 is added into a third column:
```c++
    QVector<double> X, Y, R;
    const int Ndata=500; // number of plot points in each curve
    const double phiMax=4.0*M_PI;
    const double a=1;
    const double k=0.2;
    for (double phi=-phiMax; phi<=phiMax; phi+=phiMax/double(Ndata)) {
        const double x=a*exp(k*phi)*cos(phi);
        const double y=a*exp(k*phi)*sin(phi);
        X<<x;
        Y<<y;
        R<<sqrt(x*x+y*y);
    }
    // and copy it to the datastore
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY=ds->addCopiedColumn(Y, "y");
    size_t columnR=ds->addCopiedColumn(R, "r");
```


Then simples graph just uses the columns X and Y to plot the curve:
```c++
    JKQTPxyLineGraph* graph1=new JKQTPxyLineGraph(&plot);
    graph1->set_xColumn(columnX);
    graph1->set_yColumn(columnY);
    graph1->set_drawLine(true);
    graph1->set_symbol(JKQTPnoSymbol);
    graph1->set_title("one-colored spiral");
    plot.addGraph(graph1);
```
 
If you use `JKQTPxyParametrizedScatterGraph` instead of `JKQTPxyLineGraph`, you can also modify the color of the line-segments, connecting the datapoints:
```c++
    JKQTPxyParametrizedScatterGraph* graph2=new JKQTPxyParametrizedScatterGraph(&plot2);
    graph2->set_xColumn(columnX);
    graph2->set_yColumn(columnY);
    graph2->set_colorColumn(columnR);
    graph2->set_palette(JKQTPMathImageMATLAB);
    graph2->set_symbol(JKQTPnoSymbol);
    graph2->set_drawLine(true);
    graph2->set_title("colored spiral");
    graph2->get_colorBarRightAxis()->set_axisLabel("color scale radius $r(\\phi)$");
    plot2.addGraph(graph2);
```

The result looks like this:

![jkqtplotter_simpletest_parametriccurve](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parametriccurve1.png)

... and with the line-color set by the radius:

![jkqtplotter_simpletest_parametriccurve](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parametriccurve2.png)



[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)