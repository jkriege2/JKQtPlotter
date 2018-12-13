[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Layouting Several Plots
This project (see `./test/test_multiplot/`) shows how several JKQtPlotter widgets can be combined to in a layout (based on the [Qt layouting system](http://doc.qt.io/qt-5/layout.html)). It also shows how axes in such a layout can be linked to improve user experience.

The source code of the main application can be found in  [`test_multiplot.cpp`](https://github.com/jkriege2/JKQtPlotter/blob/master/test/test_multiplot/test_multiplot.cpp). 

First three plots are generated and put into a [QGridLayout](http://doc.qt.io/qt-5/qgridlayout.html):

```c++
// 1. create a widget
    QWidget mainWidget;
    mainWidget.setWindowTitle("JKQtPlotter(s) in a QGridLayout");

    // 2. Create a QGridLayout for the plots and add it to the widget.
    QGridLayout* layout=new QGridLayout();
    mainWidget.setLayout(layout);

    // 3.1 create a main plotter widget and add it to the layout
    JKQtPlotter* plotMain=new JKQtPlotter(&mainWidget);
    layout->addWidget(plotMain, 0,0);
    JKQTPdatastore* ds=plotMain->getDatastore();

    // 3.2 create a second and third plotter widget and add them to the
    //     layout below and at the bottom right of the plotMain.
    //     Also configure it to use the same datastore as plotMain
    JKQtPlotter* plotResid=new JKQtPlotter(false, &mainWidget, ds);
    layout->addWidget(plotResid, 1,0);
    JKQtPlotter* plotResidHist=new JKQtPlotter(false, &mainWidget, ds);
    layout->addWidget(plotResidHist, 1,1);
	
    // 3.3 set relative sizes of the plots via the layout (small plots have 1/3 the width and height of the large plot
    layout->setRowStretch(0,3);
    layout->setRowStretch(1,1);
    layout->setColumnStretch(0,3);
    layout->setColumnStretch(1,1);
```

With this simple setup, all three plots would be arranged by the QLayout, but they were all independent. This example could be part of a data fitting application, where the main plot shows data and a fit curve. A plot below that will display the residulas (errors) of the fit. Now if a user zooms one of the plots, he would expect that athe x-axes of the two plots are synchronized. The same for a third plot on the rhs of the residuals, which will show a residual histogram. This linking of the axes can be achieved by the following code:

```c++
    // 3.4 synchronize width/x-axis of plotResid to width/x-axis of plotMain
    plotResid->get_plotter()->synchronizeToMaster(plotMain->get_plotter(), true, false, true, true);

    // 3.5 synchronize y-axis of width/plotResidHist to y-axis of width/plotResid
    plotResidHist->get_plotter()->synchronizeToMaster(plotResid->get_plotter(), false, true, true, true);
```

Finally: When printing or saving an image of the plots, the plotter will no know anything about the arrangement of the plots and the plots cannot be printed/drawn in the same arrangement as in the window. If you want to arrange the plots in the same layout in a printout, as in the window, you will have to tell the main plot, in which arrangement to print the plots:

```c++
    // 3.6 ensure that the plot are printed/exported in whole, when printing in plotMain
    plotMain->get_plotter()->set_gridPrinting(true);
    plotMain->get_plotter()->addGridPrintingPlotter(0,1,plotResid->get_plotter());
    plotMain->get_plotter()->addGridPrintingPlotter(1,1,plotResidHist->get_plotter());
```

In the first line, grid-printing (i.e. the layouted printing of several graphs) is activated. Then the arrangement of the two slave plots `plotResid` and `plotResidHist` is defined as (`x,y`)-shifts with respect to the master plot `plotMain`.

Now some data is generated and several curves are added to the graphs. See [`test_multiplot.cpp`](https://github.com/jkriege2/JKQtPlotter/blob/master/test/test_multiplot/test_multiplot.cpp) for the full source code.

Finally the axes and plots need a bit of formatting to make them look nicer:

```c++
    // 6.1 axis labels, distributed over the several plots
    plotMain->get_yAxis()->set_axisLabel("y axis");
    plotResid->get_xAxis()->set_axisLabel("x axis");
    plotResid->get_yAxis()->set_axisLabel("residuals");
    plotResidHist->get_xAxis()->set_axisLabel("frequency");
    // 6.2 switch off the tick labels on the axes that directly face another plot
    plotMain->get_xAxis()->set_drawMode1(JKQTPCADMticks);
    plotResidHist->get_yAxis()->set_drawMode1(JKQTPCADMticks);
    // 6.3 show tick labels on the rhs y-axis of the residual histogram plot
    plotResidHist->get_yAxis()->set_drawMode2(JKQTPCADMticksAndLabels);
    // 6.4 hide keys in all plots but the main plot
    plotResid->get_plotter()->set_showKey(false);
    plotResidHist->get_plotter()->set_showKey(false);
    // 6.5 hide position label and toolbars in the plots except main plot
    plotResid->set_displayToolbar(false);
    plotResid->set_displayMousePosition(false);
    plotResidHist->set_displayToolbar(false);
    plotResidHist->set_displayMousePosition(false);
    plotMain->set_toolbarAlwaysOn(true);
```

As a last step, the axes are scaled automatically, so the data fills the plots:

```c++
    // 7. scale plots automatically to data
    plotResid->zoomToFit();
    plotResidHist->zoomToFit();
    plotMain->zoomToFit();
```

The result looks like this:

![test_multiplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/test_multiplot.png)

You push the print button (![test_multiplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/lib/jkqtplotterressources/images/jkqtp_24_print.png)) to open a print preview dialog, which will give an impression of how the three plots will be arranged in a printout:

![test_multiplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/test_multiplot_printpreview.png)




[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)
