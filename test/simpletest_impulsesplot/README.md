[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Simple impulse plots
This project (see `./test/simpletest_impulsesplot/`) simply creates a JKQtPlotter widget (as a new window) and adds a single impulse graph. 
The soruce code of the main application is (see [`jkqtplotter_simpletest_impulsesplot.cpp`](https://github.com/jkriege2/JKQtPlotter/blob/master/test/simpletest_impulsesplot/jkqtplotter_simpletest_impulsesplot.cpp).



First data for a curve is calculated and stored in `QVector<double`: 
```c++
    QVector<double> X, Y;
    for (int i=0; i<Ndata; i++) {
        const double xx=double(i)/double(Ndata)*6.0*M_PI;
        X << xx;
        Y << cos(xx)*exp(-xx/10.0);
    }
```

... and finally the data is copied into the datastore
```c++
    size_t columnX=ds->addCopiedColumn(X,  "x");
    size_t columnY=ds->addCopiedColumn(Y,  "y");
```
	
Now an impulse graph object is generated and added to the plot:
```c++
    JKQTPimpulsesVerticalGraph* graph=new JKQTPimpulsesVerticalGraph(&plot);
    graph->set_xColumn(columnX);
    graph->set_yColumn(columnY);
	graph->set_lineWidth(2);
	graph->set_color(QColor("red"));
    graph->set_title(QObject::tr("$\\cos(x)\\cdot\\exp(-x/10)$"));

    plot.addGraph(graph);
```

The result looks like this:

![jkqtplotter_simpletest_impulsesplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_impulsesplot.png)

There is an alternative class `JKQTPimpulsesHorizontalGraph` which draws horizontal impulse plots:
```c++
    JKQTPimpulsesHorizontalGraph* graph=new JKQTPimpulsesHorizontalGraph(&plot);
    graph->set_yColumn(columnX);
    graph->set_xColumn(columnY);
    graph->set_lineWidth(2);
    graph->set_color(QColor("blue"));
    graph->set_title(QObject::tr("$\\cos(x)\\cdot\\exp(-x/10)$"));
```

This code snippet results in a plot like this:

![jkqtplotter_simpletest_impulsesplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_impulsesplot_horizontal.png)


[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)