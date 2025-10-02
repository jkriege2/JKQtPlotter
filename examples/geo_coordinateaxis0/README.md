# Example (JKQTPlotter): Plot With Axes through Origin                                      {#JKQTPlotterGeometricCoordinateAxis0}



This example draws a coordinate system with several geomtric shapes, where the coordinate axes pass through the origin of the plot, not at the borders. Note that if you move the displayed coordinate ranges, so 0 is not in them, the axes still stick to the borders.

The source code of the main application can be found in  [`geo_coordinateaxis0.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/geo_coordinateaxis0/geo_coordinateaxis0.cpp).

Apart from the usual code to set up the graphs, the following fragment switches the "normal" axes at the borders off and the zero axis on:
```.cpp
    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.getXAxis()->setDrawMode1(JKQTPCADMnone);
    plot.getXAxis()->setDrawMode2(JKQTPCADMnone);
    plot.getXAxis()->setDrawMode0(JKQTPCADMcomplete|JKQTPCADMMaxFilledArrow);
    plot.getXAxis()->setLabelPosition(JKQTPLabelMax);
    plot.getXAxis()->setAxisLabel("$X$ Coordinate");
    plot.getYAxis()->setDrawMode1(JKQTPCADMnone);
    plot.getYAxis()->setDrawMode2(JKQTPCADMnone);
    plot.getYAxis()->setDrawMode0(JKQTPCADMcomplete|JKQTPCADMMaxFilledArrow);
    plot.getYAxis()->setLabelPosition(JKQTPLabelMax);
    plot.getYAxis()->setAxisLabel("$Y$ Coordinate");
```
The axes are configured to have an arrow at their max end (`|JKQTPCADMMaxFilledArrow`) and the axis label is also positioned close to the axis max.

The resulting plot looks like this:

![geo_coordinateaxis0](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/geo_coordinateaxis0.png)
