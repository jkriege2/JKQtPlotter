# Example (JKQTPlotter): Advanced Line and Fill Styling {#JKQTPlotterAdvancedLineAndFillStyling}
This project (see `./examples/advplotstyling/`) demonstrates how to use advanced line styling and filling options (e.g. custom dash-styles, gradient, image fills, transparencies, ...) with JKQtPlotter.

The source code of the main application can be found in  [`advplotstyling.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/advplotstyling/advplotstyling.cpp). 

Initially several data columns are generated and added to the internal datastore. Then several line graphs are added that all use different custom dashes. In addition the third line does not use a single color, but a gradient for coloring:
```.cpp
    JKQTPGeoLine* graphL1=new JKQTPGeoLine(&plot, 0, 3, 12, 3.5, Qt::red);
    JKQTPGeoLine* graphL2=new JKQTPGeoLine(&plot, 0, 3.8, 12, 4.5, Qt::red);
    JKQTPGeoLine* graphL3=new JKQTPGeoLine(&plot, 0, 4.6, 12, 5.5, Qt::red);

    // 5.2 set advanced line style (see also https://doc.qt.io/qt-5/qpen.html#setDashPattern)
    //     and use a gradient-brush to fill the last (thicker) line
    QVector<qreal> dashes1;
    dashes1 << 2 << 2 << 2 << 2 <<  4 << 4 <<  4 << 4 << 8 << 8 << 8 << 8 ;
    graphL1->setLineDashPattern(dashes1);
    graphL1->setLineWidth(2);
    QVector<qreal> dashes2;
    dashes2 << 1 << 2 << 2 << 2 << 3 << 2 << 4 << 2 << 5 << 2 << 6 << 2 ;
    graphL2->setLineDashPattern(dashes2);
    graphL2->setLineWidth(2);
    QVector<qreal> dashes3;
    dashes3 << 4 << 4 << 4 << 4 << 12 << 4 ;
    graphL3->setLineDashPattern(dashes3);
    graphL3->setLineWidth(4);
    QLinearGradient lineGrad(QPointF(0, 0), QPointF(1, 0));
    lineGrad.setColorAt(0, Qt::red);
    lineGrad.setColorAt(1, Qt::green);
    lineGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    graphL3->setLineBrush(lineGrad);
```

In addition, a `JKQTPSpecialLineHorizontalGraph` is filled using an image `example.bmp` as a texture:
```.cpp
    JKQTPSpecialLineHorizontalGraph* graphF=new JKQTPSpecialLineHorizontalGraph(&plot);
    
    // 5.2 set fill style, using an image, also rotate the images
    graphF->setSpecialLineType(JKQTPDirectLine);
    graphF->setDrawLine(true);
    graphF->setFillTexture(QPixmap(":/example.bmp"));
    graphF->setFillTransform(QMatrix(0.5,0,0,0.5,0,0).rotate(45));
    graphF->setFillCurve(true);
    graphF->setLineWidth(0.5);    
```
	
This texture-filled graph is overlayn by a graph filled with a color gradient that is semi-transparent in some colors:

```.cpp
    JKQTPSpecialLineHorizontalGraph* graphF2=new JKQTPSpecialLineHorizontalGraph(&plot);
    
    // 5.3 set fill style, using a custom linear gradient, also with changing transparency (alpha) values
    graphF2->setSpecialLineType(JKQTPStepCenter);
    graphF2->setDrawLine(false);
    graphF2->setFillCurve(true);
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(1, 1));
    QColor c1(Qt::yellow);
    c1.setAlphaF(0.5);
    QColor c2(Qt::red);
    QColor c3(Qt::blue);
    QColor c4(Qt::green);
    c4.setAlphaF(0.5);
    linearGrad.setColorAt(0, c1);
    linearGrad.setColorAt(0.3, c2);
    linearGrad.setColorAt(0.7, c3);
    linearGrad.setColorAt(1, c4);
    linearGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    // use this CoordinateMode, so the gradient fills the whole graph area
    graphF2->setFillGradient(linearGrad);
```

Finally two vertical barcharts with different filling options are added:

```.cpp
    JKQTPBarVerticalGraph* graphBE=new JKQTPBarVerticalGraph(&plot);
    JKQTPBarVerticalGraph* graphBF=new JKQTPBarVerticalGraph(&plot);

    // 5.4 fill barcharts with transparencies and make the surrounding line invisible (colored transparent)
    QColor c0=QColor("darkgreen").darker();
    c0.setAlphaF(.75);
    graphBE->setFillColor(c0);
    graphBE->setLineColor(Qt::transparent);
    QLinearGradient linearG(QPointF(0, 0), QPointF(0, 1));
    QColor cl11(QColor("darkgreen"));
    QColor cl12(Qt::green);
    cl12.setAlphaF(0.5);
    linearG.setColorAt(0, cl11);
    linearG.setColorAt(1, cl12);
    linearG.setCoordinateMode(QGradient::ObjectBoundingMode);
    graphBF->setFillGradient(linearG);
    graphBF->setLineColor(Qt::transparent);
```

In addition to the graph background properties, also the plot/widget/key backgrounds can be defined using a QBrush:

```.cpp
    // 7. set plot background with a gradient
    QLinearGradient backGrad(QPointF(0, 0), QPointF(1, 1));
    backGrad.setColorAt(0, QColor("salmon"));
    backGrad.setColorAt(1, QColor("white"));
    backGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    plot.getPlotter()->setPlotBackgroundGradient(backGrad);
```


The result looks like this:

![advplotstyling](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/advplotstyling.png)



