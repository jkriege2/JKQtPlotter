# Example (JKQTPlotter): Different Types of Errorindicators {#JKQTPlotterErrorBarStyles}
This project (see `./examples/simpletest_errorbarstyles/`) simply creates a JKQTPlotter widget (as a new window) and adds several curves show-casing different styles of error indicators. Data is initialized from two QVector<double> objects.

The source code of the main application can be found in  [`jkqtplotter_simpletest_errorbarstyles.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/simpletest_errorbarstyles/jkqtplotter_simpletest_errorbarstyles.cpp). 

First some data is added to the internal datastore (mostly, like explained in several other examples, like e.g. [Line Graph with Different Symbols and Line Styles](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/simpletest_symbols_and_styles)). The (in a loop) several graphs are added, each with a distinct style for its error indicators:

```.cpp
    // 3. now we make several plots with different error styles
    //    for that we iterate over every style from the vector errorStyles
    //    the array showXandYError indicates whether we want x- and y-error
    //    for a style for all stywhere this is false, only y-error-indicators
    //    are shown
	QVector<JKQTPErrorPlotstyle> errorStyles    {JKQTPNoError, JKQTPErrorBars, JKQTPErrorSimpleBars, JKQTPErrorLines, JKQTPErrorPolygons, JKQTPErrorBoxes, JKQTPErrorEllipses, JKQTPErrorBarsPolygons, JKQTPErrorBarsLines, JKQTPErrorSimpleBarsLines, JKQTPErrorSimpleBarsPolygons };
    QVector<bool>                showXandYError { false      , true          , true                , false          , false             , true           , true              , false                 , false              , false                    , false                        };

    for (int errorID=0; errorID<errorStyles.size(); errorID++) {
        // generate some plot data
        QVector<double> Y;
        for (auto& xx: X) {
            Y<<xx*0.5+static_cast<double>(errorID)*2.5;
        }

		// create a graph object
        JKQTPXYLineErrorGraph* graph=new JKQTPXYLineErrorGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->setXColumn(columnX);
        graph->setYColumn(ds->addCopiedColumn(Y, "y"+QString::number(errorID)));
        graph->setXErrorColumn(columnXError);
        graph->setYErrorColumn(columnYError);

        // set error style, for the y-axis
        graph->setYErrorStyle(errorStyles[errorID]);
        // no error indicators for the x-values
        graph->setXErrorStyle(JKQTPNoError);
        // ... unless: for some error styles we want error in both directions
        if (showXandYError[errorID]) {
            graph->setXErrorStyle(errorStyles[errorID]);
            graph->setDrawLine(false);
        }
        // make error indicator 30% transparent
        QColor c=graph->getErrorFillColor();
        c.setAlphaF(0.3);
        graph->setErrorFillColor(c);
        // set error indicator line width
        graph->setErrorWidth(1);
        // set length of small bars at the end of error bars
        graph->setErrorbarSize(15);


        // set symbol (cross/X) + pen style (and color)dashed)
        graph->setSymbol(JKQTPCross);
        graph->setStyle(Qt::DashLine);
        // set symbol size
        graph->setSymbolSize(5);
        // set width of symbol lines
        graph->setSymbolWidth(1);
        // set width of graph line
        graph->setLineWidth(1);

        // graph title is made from symbol+errorStylestyle, we use the LaTeX instruction \verb around the
        // result of JKQTPErrorPlotstyle2String(), because it contains underscores that would otherwise
        // lead to lower-case letter, which we don't want
        graph->setTitle("\\verb{"+JKQTPErrorPlotstyle2String(errorStyles[errorID])+"}");

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);
    }
```

The error styles are set in these lines:
```.cpp
	// set error style, for the y-axis
	graph->setYErrorStyle(errorStyles[errorID]);
	// no error indicators for the x-values
	graph->setXErrorStyle(errorStyles[errorID]);
```

There are several variables that can be used to further style the error indicator, like:
```.cpp
	// make error indicator 30% transparent
	QColor c=graph->getErrorFillColor();
	c.setAlphaF(0.3);
	graph->setErrorFillColor(c);
	// set error indicator line width
	graph->setErrorWidth(1);
	// set length of small bars at the end of error bars
	graph->setErrorbarSize(15);
```

There are more properties that you can find in the documentation of the mix-in classes `JKQTPXYGraphErrors`, `JKQTPXGraphErrors`, `JKQTPYGraphErrors`, `JKQTPGraphErrors`.

In addition the plot key is moved outside the pot and the grid in the plot is switched off:
```.cpp
    // 6. change locaion of key (outside top-right)
    plot.getPlotter()->setKeyPosition(JKQTPKeyOutsideRightTop);
    // ... and switch off the grid
    plot.getXAxis()->setDrawGrid(false);
    plot.getXAxis()->setDrawMinorGrid(false);
    plot.getYAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawMinorGrid(false);
```

The result looks like this:

![jkqtplotter_simpletest_errorbarstyles](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_errorbarstyles.png)

Error bars are implemented in the mixin-classes `JKQTPXYGraphErrors`, `JKQTPXGraphErrors` and `JKQTPYGraphErrors` that are all derived from `JKQTPGraphErrors`. With these it is simple to add error indicators to several different plot styles. Usually you can recognize these by looking at the class name, e.g. `JKQTPXYLineGraph` is a simple line+symbol graph, and `JKQTPXYLineErrorGraph` is the same with error indictaors (see above). There are also several other plots with error indicators:
  - `JKQTPBarVerticalErrorGraph` for barcharts with errors:<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_errorbarstyles_barcharts.png)
  - `JKQTPImpulsesVerticalGraph` for impulse/candle-stick charts with errors:<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_errorbarstyles_impulses.png)
  - `JKQTPFilledCurveXErrorGraph` for filled curves with errors:<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_errorbarstyles_filledcurves.png)
.


