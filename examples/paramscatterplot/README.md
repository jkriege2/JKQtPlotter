# Example (JKQTPlotter): Scatter Graph with Parametrized Symbols/Colors {#JKQTPlotterParamScatter}
This project (see `./examples/paramscatterplot/`) demonstrates the capabilities of `JKQTPXYParametrizedScatterGraph`. This graph class plots symbol&line-graphs, juts like [`JKQTPXYLineGraph`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/symbols_and_styles/) and in addition modifies several properties of each plot point by data from an additional column. These properties can be modified:
- symbol size
- symbol type
- symbol/line color
- line width

The source code of the main application can be found in  [`paramscatterplot.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/paramscatterplot/paramscatterplot.cpp). First, several datasets are generated and added to the internal datastore. the resulting datatable looks like this:

![paramscatterplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/paramscatterplot_datatable.png)

Then several plots are added that modify different properties.

The simplest case is to modify the symbol type. Simply set the property `symbolColumn` with `graph1->setSymbolColumn(columnP)` to a data column. The values in the data column will be cast to an integer and then will be translated to `JKQTPGraphSymbols`. If the numbers are larger than the available symbol types in `JKQTPGraphSymbols`, the graph will cycle through the available symbols (via a modulo-operation with the max. symbol count!).
```.cpp
	JKQTPXYParametrizedScatterGraph* graph1=new JKQTPXYParametrizedScatterGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY1);
    graph1->setSymbolColumn(columnP);
    graph1->setDrawLine(true);
    graph1->setColor(QColor("blueviolet"));
    graph1->setTitle("1: symbol type");
    plot.addGraph(graph1);
```


The next two code snippets show how to modify the size of the symbols and the line width of the lines, connecting the symbols (ensure to set `graph6->setDrawLine(true)`, because otherwise no line will be drawn). The principle is the same as above, but here you need to set the properties `sizeColumn` for the symbol size and `linewidthColumn` for the line width. All values in the line width or symbol size columns are interpreted as sizes in dtp points (pt)!
```.cpp
    // symbol size
    JKQTPXYParametrizedScatterGraph* graph3=new JKQTPXYParametrizedScatterGraph(&plot);
    graph3->setXColumn(columnX);
    graph3->setYColumn(columnY3);
    graph3->setSizeColumn(columnS);
    graph3->setSymbolType(JKQTPFilledCircle);
    graph3->setDrawLine(true);
    graph3->setTitle("3: symbol size");
    plot.addGraph(graph3);

	// line width
    JKQTPXYParametrizedScatterGraph* graph6=new JKQTPXYParametrizedScatterGraph(&plot);
    graph6->setXColumn(columnX);
    graph6->setYColumn(columnY6);
    graph6->setLinewidthColumn(columnLW);
    graph6->setDrawLine(true);
    graph6->setSymbolType(JKQTPNoSymbol);
    graph6->setTitle("6: line width");
    plot.addGraph(graph6);
```


Finally you can set the color of each symbol, based on data in the column `colorColumn`. Here two possibilities exist: First you can store the RGB(A) value for each datapoint explicitly. For this, you first need to create the data in the column, using the Qt-function [`qRgb()`](http://doc.qt.io/qt-5/qcolor.html#qRgb) or [`qRgba()`}(http://doc.qt.io/qt-5/qcolor.html#qRgba):
```.cpp
    QVector<double> RGB;
    const int Ndata=10; // number of plot points in each curve
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*2.0*M_PI;
        RGB<<double(qRgb(double(i)/double(Ndata)*255,0,255-double(i)/double(Ndata)*255));
    }
    size_t columnRGB=ds->addCopiedColumn(RGB, "rgb");
```.cpp
Basically the data points in a RGB(A)-column will be interpreted by castig them to [`QRgb`](http://doc.qt.io/qt-5/qcolor.html#QRgb-typedef).

Now you can add the graph. In order to interpret the color column as RGB(A)-values, ensure to set `graph4->setColorColumnContainsRGB(true)`:
```.cpp
    JKQTPXYParametrizedScatterGraph* graph4=new JKQTPXYParametrizedScatterGraph(&plot);
    graph4->setXColumn(columnX);
    graph4->setYColumn(columnY4);
    graph4->setColorColumn(columnRGB);
    graph4->setColorColumnContainsRGB(true);
    graph4->setDrawLine(true);
    graph4->setSymbolType(JKQTPFilledDownTriangle);
    graph4->setTitle("4: RGB-color");
    plot.addGraph(graph4);
```

The second variant for setting the color of each datapoint is by mapping the values in the column to a color palette (`JKQTPMathImageRYGB` in this example). For this you simply need to define the color coumn and the palette to use. By default, the color palette spans the full range of values in `colorColumn`:
```.cpp
    JKQTPXYParametrizedScatterGraph* graph2=new JKQTPXYParametrizedScatterGraph(&plot);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setColorColumn(columnC);
    graph2->setPalette(JKQTPMathImageRYGB);
    graph2->setSymbolType(JKQTPFilledRect);
    graph2->setDrawLine(true);
    graph2->setTitle("2: color");
    graph2->getColorBarRightAxis()->setAxisLabel("color scale for graph2");
    plot.addGraph(graph2);
```
Note: If you want to set the range manually, use `ste_imageMin()` and `setImageMax()` after setting `setAutoImageRange(false)`.


Note also that it is possible to combine any of parametrizations above in a single graph, by setting two or more columns:
```.cpp
    JKQTPXYParametrizedScatterGraph* graph5=new JKQTPXYParametrizedScatterGraph(&plot);
    graph5->setXColumn(columnX);
    graph5->setYColumn(columnY5);
    graph5->setColorColumn(columnC);
    graph5->setSizeColumn(columnS);
    graph5->setPalette(JKQTPMathImageBLUEYELLOW);
    graph5->setDrawLine(true);
    graph5->setTitle("5: color+size");
    graph5->getColorBarRightAxis()->setAxisLabel("color scale for graph5");
    plot.addGraph(graph5);
```


The full test appication combines all these variants and the result looks like this:

![paramscatterplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/paramscatterplot.png)


In `` you can also set special functors that transform the values from the data columns (symbol type+size, datapoint), before using them for the plot, which gives you even more control. As an example you can set a special functor to `graph6`:

```.cpp
    graph6->setLinewidthColumnFunctor([](double x, double y, double w) {
        return fabs(sin(w/3.0)*25.0);
    });
```

This will result in modulated linewidths as shown below:

![JKQTPXYParametrizedScatterGraph_LinewidthFunctor.png](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/JKQTPXYParametrizedScatterGraph_LinewidthFunctor.png)


The same can be done for symbol type, e.g. with code like this:

```.cpp
    graph1->setSymbolColumnFunctor([](double x, double y, double sym) -> JKQTPGraphSymbols {
        if (sym<Ndata/2) {
            return JKQTPGraphSymbols::JKQTPCircle;
        } else if (sym>Ndata/2) {
            return JKQTPGraphSymbols::JKQTPFilledCircle;
        } else {
            return JKQTPGraphSymbols::JKQTPPlus;
        }
    });
```


This will result in symbols as shown below:

![JKQTPXYParametrizedScatterGraph_SymbolFunctor.png](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/JKQTPXYParametrizedScatterGraph_SymbolFunctor.png)

As an alternaitve for symbols, you can define the functor also in terms of a QMap:

```.cpp
    graph1->setSymbolColumnFunctor([](double x, double y, double sym) -> JKQTPGraphSymbols {
        if (sym<Ndata/2) {
            return JKQTPGraphSymbols::JKQTPCircle;
        } else if (sym>Ndata/2) {
            return JKQTPGraphSymbols::JKQTPFilledCircle;
        } else {
            return JKQTPGraphSymbols::JKQTPPlus;
        }
    });
```

This will result in symbols as shown below:

![JKQTPXYParametrizedScatterGraph_MappedSymbolFunctor.png](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/JKQTPXYParametrizedScatterGraph_MappedSymbolFunctor.png)

