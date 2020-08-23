# Example (JKQTPlotter): Plotting Arrows {#JKQTPlotterGeometricArrows}

This project shows the capabilities of JKQTPlotter to also draw arrows as geometric elements, using JKQTPGeoArrow. The arrow head/tail are defined by the enum values in JKQTPLineDecoratorStyle.

The source code of the main application can be found in  [`geo_arrows.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/geo_arrows/geo_arrows.cpp). First a plot is generated. Then several types of arrows are plotted onto the JKQtPlotter.

## Different types of arrows

A first table shows all available arrow tips in different sizes.
```.cpp
     for (size_t i=0; i<static_cast<size_t>(JKQTPLineDecoratorCount); i++) {
        auto const decor=static_cast<JKQTPLineDecoratorStyle>(i);
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 0.1,  arr_y, 0.3, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 0.2));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 0.4,  arr_y, 0.6, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 0.5));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 0.7,  arr_y, 0.9, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 1));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 1.0,  arr_y, 1.3, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 2));
        plot.addGraph(a=new JKQTPGeoArrow(&plot, 1.4,  arr_y, 1.7, arr_y+0.05, QColor("red"), decor, JKQTPNoDecorator, 3));
        plot.addGraph(new JKQTPGeoText(&plot, a->getX2()+0.05, a->getY2(), "\\verb{"+JKQTPLineDecoratorStyle2String(decor)+"}", 12, a->getLineColor()));
        arr_y+=arr_deltay;
    }
```

Here is the resulting table:

![geo_arrow_tips](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/geo_arrow_tips.png)

Note how the head-size scales with the line-width, but not linearly, but rather sub-linearly, so the tips do not grow too strongly.

Also note that all arrows end at the designated line-end (here indicated by dashed grey lines), even circles and rectangle:

![geo_arrow_tipsatlineend](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/geo_arrow_tipsatlineend.png)

## Classes with support for arrows

You can use JKQTPGeoArrow and JKQTPGeoLine to draw arrows (JKQTPGeoArrow is just a convenience class that enables arrows by default, otherwise it is equal to JKQTPGeoLine).

In addition, also other classes can show line-decorators:
  - JKQTPGeoLine
  - JKQTPGeoPolyLines
  - JKQTPGeoInfiniteLine
.

Here is an example of how to actiavate them for a JKQTPGeoPolyLines:

```.cpp
    QVector<QPointF> points; points<<QPointF(3,  0.6)<<QPointF(4,  0.5)<<QPointF(3,  1.2)<<QPointF(4,  1.0);
    JKQTPGeoPolyLines* polyLine=new JKQTPGeoPolyLines(&plot, points, QColor("blue"));
    polyLine->setHeadDecoratorStyle(JKQTPFilledDoubleArrow);
    polyLine->setTailDecoratorStyle(JKQTPCircleDecorator);
    plot.addGraph(polyLine);
```

Here is the result:

![geo_arrow_polylines](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/geo_arrow_polylines.png)

For the class JKQTPGeoInfiniteLine the start can be decorated with an arrow (only if two_sided==false!):

```.cpp
    JKQTPGeoInfiniteLine* infLine=new JKQTPGeoInfiniteLine(&plot, 1.5, 0.2, 1, 0.25, QColor("blue"), 2);
    infLine->setHeadDecoratorStyle(JKQTPFilledDoubleArrow);
    plot.addGraph(infLine);
```

Here is the result:

![geo_arrow_polylines](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/geo_arrow_inflines.png)

## Screenshot

The result of the complete example looks like this:

![geo_arrows](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/geo_arrows.png)



