[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Simple impulse plots
This project (see `./test/simpletest_impulsesplot/`) simply creates a JKQtPlotter widget (as a new window) and adds a single impulse graph. 
The soruce code of the main application is (see [`jkqtplotter_simpletest_impulsesplot.cpp`](https://github.com/jkriege2/JKQtPlotter/blob/master/test/simpletest_impulsesplot/jkqtplotter_simpletest_impulsesplot.cpp).



First the data columns for three x-y-curves are generated. One column of x-values with entries 0,1,2,...,254,255 (256 entries). 
```c++
    size_t columnX=ds->addLinearColumn(256, 0, 255, "x");
```

And three columns with 256 entries each, which will be filled with the R-, G- and B-histograms of an image `example.bmp`:
```c++
    size_t columnR=ds->addColumn(256, "historam_R");
    size_t columnG=ds->addColumn(256, "historam_G");
    size_t columnB=ds->addColumn(256, "historam_B");
```
	
In this example we will access the data in the internal datastore directly. This access is possible through objects of type JKQTPcolumn, which is a proxy to the data in one of the columns in a `JKQTdatastore`:

```c++
    JKQTPcolumn cG=ds->getColumn(columnG);
    JKQTPcolumn cR=ds->getColumn(columnR);
    JKQTPcolumn cB=ds->getColumn(columnB);
```

In order to calculate the histograms, first all entries in the columns are set to 0:

```c++
    cR.setAll(0);
    cG.setAll(0);
    cB.setAll(0);
```

Finally the histogram is calculated:

```c++
    QImage image(":/example.bmp");
    for (int y=0; y<image.height(); y++) {
        for (int x=0; x<image.width(); x++) {
            QRgb pix=image.pixel(x,y);
            cR.incValue(qRed(pix), 1);
            cG.incValue(qGreen(pix), 1);
            cB.incValue(qBlue(pix), 1);
        }
    }
    cR.scale(100.0/static_cast<double>(image.width()*image.height()));
    cG.scale(100.0/static_cast<double>(image.width()*image.height()));
    cB.scale(100.0/static_cast<double>(image.width()*image.height()));
```

Finally three `JKQTPfilledCurveXGraph` objects are generated and added to the plot (here we show the code for the R-channel only):

```c++
    JKQTPfilledCurveXGraph* graphR=new JKQTPfilledCurveXGraph(&plot);

    // set graph titles
    graphR->set_title("R-channel");

    // set graph colors (lines: non-transparent, fill: semi-transparent
    QColor col;
    col=QColor("red"); graphR->set_color(col);
    col.setAlphaF(0.25); graphR->set_fillColor(col);

    // set data
    graphR->set_xColumn(columnX); graphR->set_yColumn(columnR);


    // add the graphs to the plot, so they are actually displayed
    plot.addGraph(graphR);
```





The result looks like this:

![jkqtplotter_simpletest_impulsesplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_impulsesplot.png)


[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)