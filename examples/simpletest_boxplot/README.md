# Example (JKQTPlotter): Boxplots {#JKQTPlotterBoxplotsGraphs}
This project (see [`simpletest_boxplot`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/simpletest_boxplot) demonstrates how to use JKQTPlotter to draw <a href="https://en.wikipedia.org/wiki/Box_plot">box plots</a> using the classes `JKQTPBoxplotVerticalGraph` and `JKQTPBoxplotHorizontalGraph`.

The source code of the main application is (see [`jkqtplotter_simpletest_boxplot.cpp`](jkqtplotter_simpletest_boxplot.cpp).

After adding all necessary data to the JKQTDatastore:
```.cpp
    // 2. now we create data for the boxplots
    QVector<double> POS, POSY, MEDIAN, MEAN, Q25, Q75, MIN, MAX;
    POS    << 1   << 4   << 7   << 10;
    POSY   << -1  << -2  << -3  << -4;
    MIN    << 2   << 3   << 2.5 << 6;
    Q25    << 4   << 4.5 << 5   << 7;
    MEDIAN << 5   << 6   << 7   << 9;
    MEAN   << 5.5 << 5.2 << 8   << 8;
    Q75    << 6   << 7   << 9   << 11;
    MAX    << 8   << 7.5 << 11  << 12;

    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    size_t columnPOS=ds->addCopiedColumn(POS, "POS");
    size_t columnPOSY=ds->addCopiedColumn(POSY, "POSY");
    size_t columnMIN=ds->addCopiedColumn(MIN, "MIN");
    size_t columnQ25=ds->addCopiedColumn(Q25, "Q25");
    size_t columnMEDIAN=ds->addCopiedColumn(MEDIAN, "MEDIAN");
    size_t columnMEAN=ds->addCopiedColumn(MEAN, "MEAN");
    size_t columnQ75=ds->addCopiedColumn(Q75, "Q75");
    size_t columnMAX=ds->addCopiedColumn(MAX, "MAX");
```

... you can generate the JKQTPBoxplotVerticalGraph:

```.cpp
    // 4. create a graph of vertical boxplots:
    JKQTPBoxplotVerticalGraph* graph=new JKQTPBoxplotVerticalGraph(&plot);
    graph->setPosColumn(columnPOS);
    graph->setMinColumn(columnMIN);
    graph->setPercentile25Column(columnQ25);
    graph->setMedianColumn(columnMEDIAN);
    graph->setMeanColumn(columnMEAN);
    graph->setPercentile75Column(columnQ75);
    graph->setMaxColumn(columnMAX);
    graph->setTitle("vertical Boxplots");
```

You can further style the plot by e.g. setting:

```.cpp
    // 4.1 make fill collor a lighter shade of the outline color
    graph->setFillColor(graphh->getColor().lighter());
    // 4.2 make whiskers dashed
    graph->setWhiskerStyle(Qt::DashLine);
    // 4.3 change mean symbol
    graph->setMeanSymbol(JKQTPstar);
```

The result looks like this:

![jkqtplotter_simpletest_boxplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_boxplot.png)

In addition to the simple box plots, the image above also shows outliers as small circles. these need to be drawn with a separate `JKQTPXYLineGraph`:

```.cpp
    // add some outliers (as (x,y)-pairs)
    QVector<double> OUTLIERSX, OUTLIERSY;
    OUTLIERSX << 4 << 4   << 4   << 4 << 4      << 10 << 10 << 10   << 10 << 10   << 10   << 10;
    OUTLIERSY << 1 << 0.5 << 1.3 << 8 << 8.1    << 5  << 4  << 12.2 << 13 << 12.5 << 13.5 << 13.1;

    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    ...
    size_t columnOUTLIERSX=ds->addCopiedColumn(OUTLIERSX, "OUTLIERSX");
    size_t columnOUTLIERSY=ds->addCopiedColumn(OUTLIERSY, "OUTLIERSY");

    // 4. create a graph of vertical boxplots:
    ...

    // 5. outliers need to be drawn separately
    JKQTPXYLineGraph* graphOutliers=new JKQTPXYLineGraph(&plot);
    graphOutliers->setXColumn(columnOUTLIERSX);
    graphOutliers->setYColumn(columnOUTLIERSY);
    graphOutliers->setTitle("outliers");
    // make the color a darker shade of the color of graph
    graphOutliers->setColor(graph->getColor().darker());
    graphOutliers->setFillColor(QColor("white"));
    // draw outliers as small circles, without lines
    graphOutliers->setSymbol(JKQTPCircle);
    graphOutliers->setDrawLine(false);
    graphOutliers->setSymbolSize(7);
```
