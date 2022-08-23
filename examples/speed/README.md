# Example (JKQTPlotter): Simple line-graph with live-data (speed test) {#JKQTPlotterSpeedTest}
This project (see `./examples/speed/`) creates a JKQTPlotter widget (as a new window) withs two line-graph (a sine and a cosine wave with a random disturbance). It constantly changes the  data (a bit) and redraws the plot. The redraw-rate is measured and displayed at the top of the window. Also the example offers several context-menu entries to alter the plot and thus experiment with different factors that affect the actuak plot speed.

Here follows a basic description of the eample's code.  Note however that the actual code is more involved and this description is mostly to give you a first clue, what is going on.

Data is stored in two [`std::array<double, NDATA>`](https://en.cppreference.com/w/cpp/container/array) objects (`X`, `Y`, and `Y2`) and the data is added as external pointer to the datastore:
```.cpp
    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    //    Here the data from the std::array's is not copied, but only the pointer to
    //    the array is added to the datastore. therefore the datastore does not manage
    //    the memory, oly uses the data stored in it!
    JKQTPDatastore* ds=getDatastore();
    size_t columnX=ds->addColumn(X.data(), NDATA, "x");
    size_t columnY=ds->addColumn(Y.data(), NDATA, "y");
    size_t columnY2=ds->addColumn(Y2.data(), NDATA, "y2");
```
The datastore then uses the data from the `std::array` instances, i.e. just references the memory managed by the `std::arrays`s.

For this example we also don't use axis autoscaling, but set the axes explicitly:
```.cpp
    // 6. scale the plot so the graph is contained
    setX(X[0], X[NDATA-1]);
    setY(-2,2);
```

Finally a slot is started with a one-shot timer. In that slot, the data is shifted one place to the left and the graph is updated. The slot also calculated the current framerate and displays it in the window title. Finally a single-shot timer with 1ms delay is used to call the slot again (i.e. continuously):
```.cpp
void SpeedTestPlot::plotNewData()
{
    // move old data to the left
    for (size_t i=0; i<NDATA-1; i++) {
        X[i]=X[i+1];
        Y[i]=Y[i+1];
        Y2[i]=Y2[i+1];
    }
    // add one new data point
    X[NDATA-1]=X[NDATA-2]+dx;
    Y[NDATA-1]=sin(X[NDATA-1])+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;
    Y2[NDATA-1]=cos(X[NDATA-1])+static_cast<double>(std::rand())/static_cast<double>(RAND_MAX + 1u)-0.5;

    // set new x-range and replot
    setX(X[0], X[NDATA-1]);
    redrawPlot();

    // calculate and update FPS-rate in window title
    auto tlastalst=t_lastplot;
    t_lastplot=std::chrono::system_clock::now();
    const double delta_secs=static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(t_lastplot-tlastalst).count())/1000.0;
    setWindowTitle(QString("Live Data Speed Test: %2 datapoint, %1 fps").arg(1.0/delta_secs).arg(NDATA));
    // enqueue call for next data value
    QTimer::singleShot(1, this, SLOT(plotNewData()));
}
``` 

The result looks like this:

![speed1](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/speed.png)

There are different factors affecting the replot speed:
1. *Anti-Aliasing:* If `JKQTPlotter` uses Anti-Aliasing for plotting, the plots are much nicer, but also about a factor of 3-4 slower. This is due to the increased amount of calculations, necessary in the drawing sub-system of Qt.
   You can configrue anti-aliasing with these calls:
   ```.cpp
      plot.getPlotter()->setUseAntiAliasingForGraphs(false);
      plot.getPlotter()->setUseAntiAliasingForSystem(false);
      plot.getPlotter()->setUseAntiAliasingForText(false);
	```
2. *Number of Graphs:* The number of plots (and also ther type and complexity) is a major imapct factor in the plotting speed. You can switch off a plot with the context menu:<br>![contextmenu_graph_visibility](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/contextmenu_graph_visibility.png)
3. *Number of Datapoints:* Of course, the more data has to be plotted, the slower the algorithm.
4. *Graph Type:* the speed also depends on how many elements have to be drawn. So drawing lines and symbols is slower than drawing lines-only or symbols-only.
5. *Axis Scales and Plot Appearance:* Replotting is done in two steps: First the plot with the axes, labels etc. is drawn. Then the graphs are draw on top. Therefore a replot is faster, if only the graphs change, because the background (plot) does not have to be replotted.

You can change all these properties by additional context-menu actions in this test-program.

The next table summarizes some results for plotting speed under different conditions, obatined with the test program in this directory (conditions: Qt 5.11, 32-bit, MinGW, Release, Win7, Phenom II X4 765, 500 data points):

| Anti-Aliasing    | X-Axis Redraw   | # Graphs      | frame rate [fps] |
| ---------------- | --------------- | ------------- | ---------------- |
| yes              | yes             | 2             |  4               |
| yes              | yes             | 1             |  7               |
| yes              | no              | 2             |  9               |
| yes              | no              | 1             | 16               |
| no               | yes             | 2             | 18               |
| no               | yes             | 1             | 24               |
| no               | no              | 2             | 32               |
| no               | no              | 1             | 48               |



Rerunning this test on a modern AMD Ryzen 3600 (conditions: Qt 5.13.1, 64-bit, MinGW, Release, AMD Ryzen 3600, Win10, 500 data points):

| Anti-Aliasing    | X-Axis Redraw   | # Graphs      | frame rate [fps] |
| ---------------- | --------------- | ------------- | ---------------- |
| yes              | yes             | 2             |  7               |
| yes              | yes             | 1             | 14               |
| yes              | no              | 2             | 14               |
| yes              | no              | 1             | 28               |
| no               | yes             | 2             | 30               |
| no               | yes             | 1             | 52               |
| no               | no              | 2             | 59               |
| no               | no              | 1             | 100              |



