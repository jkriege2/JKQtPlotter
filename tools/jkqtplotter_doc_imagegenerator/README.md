# Tool (JKQTMathText): Command-Line Utility jkqtplotter_doc_imagegenerator           {#JKQTPlotterDocImageRenderCmdLineTool}
This command-line tool is used to generate images for the documentation.

For some of its options, it uses a JKQTBasePlotter to directly render an image without creating a visible window. The code  then looks like this:


First we generate the JKQTBasePlotter object and add some data to the internal JKQTPDatastore
```.cpp
  JKQTBasePlotter plot(true);
  JKQTPDatastore* ds=plot.getDatastore();
  size_t cx=ds->addCopiedColumn(QVector<double>{-1.5,-0.5,0.5,1.5,2.5},"x");
  size_t cy=ds->addCopiedColumn(QVector<double>{-0.75,-0.3,-0.05,0.2,0.65},"y");
```

Now we set the range of x/y plot coordinates ...
```.cpp
  plot.setXY(-0.8,2.2,-0.5,0.7);
```
and the size of the widget, i.e. the size of the plot in the windowing system.
```.cpp
  plot.setWidgetSize(150,50);
```
Now we can add graphs to the plotter, e.g. 
```.cpp
  JKQTPXYLineGraph* g=new JKQTPXYLineGraph(&plot);
  g->setXColumn(cx);
  g->setYColumn(cy);
  plot.addGraph(g);
```
Finally we store an image of the plot as PNG-file:
```.cpp
  plot.saveAsPixelImage("output.png", false, "png");
```
