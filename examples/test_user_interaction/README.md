# Tutorial (JKQTPlotter): User-Interactions in JKQtPlotter {#JKQTPlotterUserInteraction}

## Basic Description
This project (see `./examples/test_user_interaction/`) demonstrates different types of user-interactions in JKQTPlotter.

It contains a simple plot with two graphs and provides several widgets that allow to bind different user interactions to different events:

![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/test_user_interaction.png)


## Inner Workings

The source code of the main application can be found in  [`test_user_interaction.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/test_user_interaction/test_user_interaction.cpp). 

The menu at the top of the window contains several of the QActions provided for user-interaction by JKQTBasePlotter:
```.cpp
    // add some of the default QActions from the JKQTPlotter to the window menu
    // Some of the are also available in the context menu and toolbar of the JKQTPlotter
    // others are not
    QMenu* menuPlot=menuBar()->addMenu("Plot-Menu");
    menuPlot->addAction(plot->getPlotter()->getActionPrint());
    QMenu* menuPlotS=menuPlot->addMenu("Save ...");
    menuPlotS->addAction(plot->getPlotter()->getActionSaveData());
    menuPlotS->addAction(plot->getPlotter()->getActionSavePDF()); // not available from JKQTPlotter by default
    menuPlotS->addAction(plot->getPlotter()->getActionSavePlot());
    QMenu* menuPlotZ=menuPlot->addMenu("Zoom ...");
    menuPlotZ->addAction(plot->getPlotter()->getActionZoomAll());
    menuPlotZ->addAction(plot->getPlotter()->getActionZoomIn());
    menuPlotZ->addAction(plot->getPlotter()->getActionZoomOut());
```

The rest of the form contains several Qt widgets which switch diverse aspects of the plot style (log-axes, grid on/off ...) and allow to bind different actions to several defined events, e.g.
```.cpp
    // add a QComboBox that allows to set the left mouse button action for the JKQTPlotter
    cmbLeftNoModMouseAction=new QComboBox(this);
    layForm->addRow("mouse action: left-click, no modifiers", cmbLeftNoModMouseAction);
    cmbLeftNoModMouseAction->addItem("jkqtpmdaPanPlotOnMove");
    cmbLeftNoModMouseAction->addItem("jkqtpmdaPanPlotOnRelease");
    cmbLeftNoModMouseAction->addItem("jkqtpmdaZoomByRectangle");
    cmbLeftNoModMouseAction->addItem("jkqtpmdaDrawRectangleForEvent");
    cmbLeftNoModMouseAction->addItem("jkqtpmdaDrawCircleForEvent");
    cmbLeftNoModMouseAction->addItem("jkqtpmdaDrawEllipseForEvent");
    cmbLeftNoModMouseAction->addItem("jkqtpmdaDrawLineForEvent");
    cmbLeftNoModMouseAction->addItem("jkqtpmdaScribbleForEvents");
    cmbLeftNoModMouseAction->addItem("NoMouseAction");
    cmbLeftNoModMouseAction->setCurrentIndex(2);
    connect(cmbLeftNoModMouseAction, SIGNAL(currentIndexChanged(int)), this, SLOT(setLeftMouseAction(int)));
    setLeftMouseAction(cmbLeftNoModMouseAction->currentIndex());

    // ......
    
    void TestUserInteraction::setLeftMouseAction(int index)
    {
        if (index==cmbLeftNoModMouseAction->count()-1) plot->deregisterMouseDragAction(Qt::LeftButton, Qt::NoModifier);
        else plot->registerMouseDragAction(Qt::LeftButton, Qt::NoModifier, static_cast<JKQTPMouseDragActions>(index));
    }
```

As you can see, this QComboBox registers one of the available actions to the event of a left-button single mouse click without having modifiers pressed at the same time. The slot that actually reconfigures the JKQTPlotter uses the methods JKQTPlotter::deregisterMouseDragAction() and JKQTPlotter::registerMouseDragAction() to achieve this effect.



## Available User-Interactions

You can play around with this example to find out about different types of user interactions. SOm of them are illustrated with animations below.

### Switching Graph Visibility via Context Menu

the default context menu contains a sub-menu that allows to switch every graph and and off:

![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/contextmenu_graphvisibility.gif)


### Toolbar at the top of the Plot
Each JKQTPlotter contains a toolbar that is by default invisible and appears, when the mouse moves over a designated area at the top of the plot:

![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/jkqtvanishtoolbar.gif)

You can also use the checkboxes "enable toolbar" to disable this toolbar alltogether and "toolbar 'always on'" to switch the vanishing feature off and make it visible all the time.

### Mouse Position Display

The JKQTPlotter contains a small text display for the current mouse position (in plot coordinates) at the top of the graph:

![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/mousepositiondisplay.gif)

### Drag the Plot Viewport
You can move the viewport of the graph using the mouse ("jkqtpmdaPanPlotOnMove"-action). If you drag inside the plot window, you can move in both directions, if you drag over one of the coordinate axes, you can change the range of this axis only:

![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/drag_viewport.gif)

### Zooming with the Mouse

There are several options to zoom, using the mouse:
* using the mouse wheel
* with double-click actions jkqtpdcaClickZoomsOut, jkqtpdcaClickZoomsIn
* using the context menu (or the toolbar)<br>
  ![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/zoomin_mouse_contextmenu.gif)

Again these actions are limited to a single axis, if the mouse is above that axis (and not inside the actual plot rectangle).
  
### Drawing Geometrical Forms

You can also use the mouse to draw various geometricals forms. When you finish drawing, a single event is emitted for that form, which contains its size and position:

#### Lines
![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/draw_line.gif)

#### Rectangles
![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/draw_rectangle.gif)

#### Circles  
![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/draw_circle.gif)

#### Ellipses
![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/draw_ellipse.gif)


