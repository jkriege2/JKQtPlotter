# Example (JKQTPlotter): Plotting Geometric Objects {#JKQTPlotterGeometricGraphs}
This project (see `./test/simpletest_geometric/`) shows the capabilities of JKQTPlotter to also draw geometric elements, like circles, ellipses, rectangles etc.

The source code of the main application can be found in  [`jkqtplotter_simpletest_geometric.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/simpletest_geometric/jkqtplotter_simpletest_geometric.cpp). First a plot is generated and the axis aspect ratio is set to 1, so an accurate plot is generated. Then several geometric graphs are added to the plot. Here are some examples, you can find more more examples in the source code of the example:

```.cpp
	// a text element
    plot.addGraph(new JKQTPGeoText(&plot, 0.1,0.6, "$x_{1/2}=\\frac{\\sqrt{b^2-4ac}}{2a}$", 10, QColor("red")));

	
	// a single symbol
    plot.addGraph(new JKQTPGeoSymbol(&plot, 0.1,0.6, JKQTPCircle, 5, QColor("grey")));

	
	// a line
    plot.addGraph(new JKQTPGeoLine(&plot, 1,  0.05, 1.9, 0.9, QColor("red"), 2));

	
	// a one-sided infinite line with slope dy/dx=0.25/0.2
    JKQTPGeoInfiniteLine* infLine=new JKQTPGeoInfiniteLine(&plot, 1.7,  0.2, 0.2, 0.25, QColor("green"), 1.5, Qt::PenStyle::DashLine);
    infLine->setTwoSided(false);
    infLine->setAlpha(0.5);
    plot.addGraph(infLine);

	
	// a polyline
    QVector<QPointF> p;
    p<<QPointF(1, 0.1)<<QPointF(1.3, 0.9)<<QPointF(1.6, 0.3)<<QPointF(1.9, 0.8);
    plot.addGraph(new JKQTPGeoPolyLines(&plot, p, QColor("darkgreen"), 4, Qt::PenStyle::DashDotLine));

	
	// rectangle:
    plot.addGraph(new JKQTPGeoRectangle(&plot, QPointF(0.4,1.3), QPointF(0.9,1.6), QColor("blue"), 1, Qt::SolidLine, rfill));
	// a rotated rectangle (rotated by 35 degrees):
    plot.addGraph(new JKQTPGeoRectangle(&plot, 0.5,1.5,0.8,0.5, 35, QColor("darkblue"), 2, Qt::DashLine));

	
	// ellipse:
    plot.addGraph(new JKQTPGeoEllipse(&plot, QPointF(0.4,1.3), QPointF(0.9,1.6), QColor("blue"), 1, Qt::SolidLine, rfill));
	// a rotated ellipse (rotated by 35 degrees):
    plot.addGraph(new JKQTPGeoEllipse(&plot, 0.5,1.5,0.8,0.5, 35, QColor("darkblue"), 2, Qt::DashLine));

	
	// a polygon
    JKQTPGeoPolygon* polygraph=new JKQTPGeoPolygon(&plot, QColor("red"), 2, Qt::PenStyle::DashLine, QColor("salmon"));
    polygraph->appendPoint(2.1, 0.5);
    polygraph->appendPoint(2.9, 0.9);
    polygraph->appendPoint(2.2, 0.8);
    polygraph->appendPoint(2.8, 0.25);
    polygraph->appendPoint(2.6, 0.6);
    polygraph->setAlpha(0.75);
    plot.addGraph(polygraph);

	
    // an arc from an ellipse from -10 degrees to 117 degrees, centered at 2.5,1.5 and full axes of 0.5 and 0.5
    plot.addGraph(new JKQTPGeoArc(&plot,2.5,1.5,0.5,0.5, -10, 117 , QColor("orange"), 4, Qt::PenStyle::DashLine));

	
    // a pie centered at 2.5,2.5 with ellipse axes 0.9 and 0.9 and from angle 0 degrees to 90 degrees
    plot.addGraph(new JKQTPGeoPie(&plot,2.5,2.5,0.9,0.9, 0, 90 , QColor("blue"), 4, Qt::PenStyle::SolidLine,QColor("lightblue")));

	
    // a chord centered at 2.5,2.5 with ellipse axes 0.9 and 0.9 and from angle 0 degrees to 90 degrees
    plot.addGraph(new JKQTPGeoChord(&plot,2.5,2.5,0.9,0.9, 0, 90 , QColor("blue"), 4, Qt::PenStyle::SolidLine,QColor("lightblue")));

```

The result of the example combines all these elements and looks like this:

![jkqtplotter_simpletest_geometric](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_geometric.png)



