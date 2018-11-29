[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Line Graph with Different Symbols and Line Styles
This project (see `./test/jkqtplotter_simpletest_symbols_and_styles/`) simply creates a JKQtPlotter widget (as a new window) and adds a single line-graph (a sine-wave). Data is initialized from two QVector<double> objects.

The source code of the main application can be found in  [`jkqtplotter_simpletest_symbols_and_styles.cpp`](https://github.com/jkriege2/JKQtPlotter/blob/master/jkqtplotter_simpletest_symbols_and_styles/jkqtplotter_simpletest_symbols_and_styles.cpp). Mainly several graphs are generated in a loop and then different symbol and line styles are applied to the graph (set by `graph->set_symbol()` for the symbol and `graph->set_style()` for the line style). The colors are set automtically from an internal default palette. The main loop looks like this:

```c++
	QVector<Qt::PenStyle> pens {Qt::NoPen, Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine };
    int ipen=0;
    for (int symbolID=0; symbolID<=JKQTPmaxSymbolID; symbolID++) {
        // generate some plot data
        QVector<double> Y;
        for (auto& xx: X) {
            Y<<sin(xx)+static_cast<double>(symbolID)*1.5;
        }

        JKQTPxyLineGraph* graph=new JKQTPxyLineGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->set_xColumn(columnX);
        graph->set_yColumn(ds->addCopiedColumn(Y, "y"+QString::number(symbolID)));

        // set symbol + pen style and color
        graph->set_symbol(static_cast<JKQTPgraphSymbols>(symbolID));
        graph->set_style(pens[ipen]);

        // graph title is made from symbol+penstyle
        graph->set_title(JKQTPgraphSymbols2NameString(static_cast<JKQTPgraphSymbols>(graph->get_symbol()))+QString(", ")+JKQTP_QPenStyle2String(graph->get_style()));

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);

        ipen++;
        if (ipen>=pens.size()) ipen=0;
    }
```


The result looks like this:

![jkqtplotter_simpletest_symbols_and_styles](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_symbols_and_styles.png)



[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)