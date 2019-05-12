# Example (JKQTPlotter): Line Graph with Different Symbols and Line Styles {#JKQTPlotterSymbolsAndStyles}
This project (see `./examples/simpletest_symbols_and_styles/`) simply creates a JKQTPlotter widget (as a new window) and adds a single line-graph (a sine-wave). Data is initialized from two QVector<double> objects.

The source code of the main application can be found in  [`jkqtplotter_simpletest_symbols_and_styles.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/simpletest_symbols_and_styles/jkqtplotter_simpletest_symbols_and_styles.cpp). Mainly several graphs are generated in a loop and then different symbol and line styles are applied to the graph (set by `graph->setSymbolType()` for the symbol and `graph->setLineStyle()` for the line style). The colors are set automtically from an internal default palette. The main loop looks like this:

```.cpp
    QVector<Qt::PenStyle> pens {Qt::NoPen, Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine };
    int ipen=0;
    for (int symbolID=0; symbolID<=JKQTPMaxSymbolID; symbolID++) {
        // generate some plot data
        QVector<double> Y;
        for (auto& xx: X) {
            Y<<sin(xx)+static_cast<double>(symbolID)*1.5;
        }

        JKQTPXYLineGraph* graph=new JKQTPXYLineGraph(&plot);

        // copy data into datastore and immediately set the yColumn
        graph->setXColumn(columnX);
        graph->setYColumn(ds->addCopiedColumn(Y, "y"+QString::number(symbolID)));

        // set symbol + pen style and color
        graph->setSymbolType(static_cast<JKQTPGraphSymbols>(symbolID));
        graph->setLineStyle(pens[ipen]);
        // set symbol size
        graph->setSymbolSize(14);
        // set width of symbol lines
        graph->setSymbolLineWidth(1.5);
        // set width of graph line
        graph->setLineWidth(1);
        
        // graph title is made from symbol+penstyle
        graph->setTitle(JKQTPGraphSymbols2NameString(static_cast<JKQTPGraphSymbols>(graph->getSymbolType()))+QString(", ")+jkqtp_QPenStyle2String(graph->getLineStyle()));

        // add the graph to the plot, so it is actually displayed
        plot.addGraph(graph);

        ipen++;
        if (ipen>=pens.size()) ipen=0;
    }
```

In addition to the symbol type and line style, you can also alter the size of the symbols (`graph->setSymbolSize(14)`), the line-width used to draw them (`graph->setSymbolLineWidth(1.5)`) and the line width of the graph line (`graph->setLineWidth(1)`). If you want to switch off the line altogether, use `graph->setDrawLine(false)`.

Note: There are additional, more advanced options for styling the graphs. See the example [Advanced Line and Fill Styling](@ref JKQTPlotterAdvancedLineAndFillStyling) for details.

The result looks like this:

![jkqtplotter_simpletest_symbols_and_styles](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_symbols_and_styles.png)



