[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Line Graph with Logarithmic y-axis
This project (see `./test/jkqtplotter_simpletest_logaxes/`) simply creates a JKQtPlotter widget (as a new window) and several line-graphs of different resonance curves. 

The source code of the main application can be found in  [`jkqtplotter_simpletest_logaxes.cpp`](https://github.com/jkriege2/JKQtPlotter/blob/master/test/jkqtplotter_simpletest_logaxes/jkqtplotter_simpletest_logaxes.cpp). Mainly several graphs are generated in a loop and then different line styles are applied to the graphs (set by ``graph->set_style()`). The colors are set automtically from an internal default palette. The main loop looks like this:

```c++
	QVector<Qt::PenStyle> pens {Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine };
    for (int id=0; id<D.size(); id++) {
        // generate some plot data
        QVector<double> Y;
        for (auto& xx: X) {
            Y<<1.0/sqrt(sqr(1-sqr(xx))+sqr(2*xx*D[id]));
        }

        JKQTPxyLineGraph* graph=new JKQTPxyLineGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->set_xColumn(columnX);
        graph->set_yColumn(ds->addCopiedColumn(Y, "y"+QString::number(id)));

        // don't use symbols
        graph->set_symbol(JKQTPnoSymbol);
        // use one of different pens
        graph->set_style(pens[id%pens.size()]);
        // set width of graph line
        graph->set_lineWidth(1.5);

        // graph title is made from symbol+penstyle
        graph->set_title(QString("D=\\delta/\\omega_0=%1").arg(D[id]));

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);
    }
```

Then a `JKQTPgeoText` is added to the graph, which shows the function plotted in the plot:
```c++
    // 4. Also we add a text-element in the plot to show the plotted function
    //    This element (JKQTPgeoText) is taken from the set of geometric elements
    //    and is simply parametrized by a position (1.8/10) and the text to display.
    //    In addition you can also set the font size (here to 15)
    plot.addGraph(new JKQTPgeoText(&plot, 1.8, 10, "\\frac{A}{A_{stat}}=\\frac{1}{\\sqrt{\\left(1-\\eta^2\\right)^2+\\left(2{\\eta}D\\right)^2}}", 15));
    // for nicer rendering we set the fonts used by the internal LaTeX parser instance to XITS
    plot.get_plotter()->get_mathText()->useXITS();
```

Finally the y-axis is switched to logarithmic scaling and the axis labels are set:
```c++
    // 4. set y-axis to logarithmic (x-axis would be analogous)
    //    and set axis labels (using LaTeX notation)
    plot.get_yAxis()->set_logAxis(true);
    plot.get_yAxis()->set_axisLabel("Amplitude A/A_{stat}");
    plot.get_xAxis()->set_axisLabel("relative driving frequency \\eta=\\omega/\\omega_0");
```

The result looks like this:

![jkqtplotter_simpletest_symbols_and_styles](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_logaxes.png)

Without the logarithmic scaling we would have:

![jkqtplotter_simpletest_symbols_and_styles](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_logaxes_nolog.png)


[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)