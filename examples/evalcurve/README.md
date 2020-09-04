# Example (JKQTPlotter): Plotting Parametric Mathematical Curves as Line Graphs {#JKQTPlotterEvalCurves}
## Basics
This project (see `./examples/evalcurve/`) demonstrates how to plot mathematical functions as line graphs. The functions may be defined as static C functions, C++ functors or c++ inline functions. 

[TOC]

# Simple C++ inline function
The example shows how to plot a simple C++ inline function: 

```.cpp
    JKQTPXYFunctionLineGraph* func1=new JKQTPXYFunctionLineGraph(plot);
    func1->setPlotFunctionFunctor([](double t) -> QPointF {
        const double a=5;
        const double b=4;
        const double delta=JKQTPSTATISTICS_PI/4.0;
        return QPointF(sin(a*t+delta), sin(b*t));
    });
    func1->setTRange(0, 2.0*JKQTPSTATISTICS_PI);
    func1->setTitle("C++-inline function $[ sin(5{\\cdot}t+\\pi/4), sin(4{\\cdot}t) ]$");
    plot->addGraph(func1);
```

Note that here a functor is defined, which calculates the points on a [Lissajous Curve](https://en.wikipedia.org/wiki/Lissajous_curve), i.e. a function mapping a parameter `t` to a two-dimensional point `QPointF` with `x=sin(a*t+delta)`and `y=sin(b*t)`. This function is evaluated on a range of values for `t`, set by 

```.cpp
    func1->setTRange(0, 2.0*JKQTPSTATISTICS_PI);
```

The class uses an adaptive algorithm, which determines by the local slope, at how many points (or how close points) the functor has to be evaluated. 

# Screenshot

This code snippets above result in a plot like this:

![evalcurve](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/evalcurve.png)

# Notes

Just as shown in [examples/functionplot](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/functionplot) for JKQTPXFunctionLineGraph and JKQTPYFunctionLineGraph, different types of functions can be used to plot. Either simple C++ inline functions, that take a `double t` and return a `QPointF`, or functions that additionally take a parameter vector `void* params`. In that case, the parameters may be provided from different sources, as described in [examples/functionplot](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/functionplot) . 



