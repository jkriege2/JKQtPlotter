[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Plotting Parsed Mathematical Functions as Line Graphs
### Plot Function f(x)
This project (see `./test/simpletest_parsedfunctionplot/`) demonstrates how to plot mathematical functions as line graphs. The functions are defined as strings that will be evaluated with the equation parser, integrated into JKQtPlotter. 

Note: See the example [Plotting Mathematical Functions as Line Graphs](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_functionplot) if you don't want to draw parsed functions, but want to provide a C function, or C++ functor!

Adding an evaluated funtion to a graph is very simple: 
```c++
    JKQTPxParsedFunctionLineGraph* parsedFunc=new JKQTPxParsedFunctionLineGraph(plot);
    parsedFunc->set_function("sin(x*8)*exp(-x/4)");
    parsedFunc->set_title("user function");
```
As you can see a graph of the type `JKQTPxParsedFunctionLineGraph` is used, which plots a function that depends on the variable `x`. The given function is parsed and evaluated (see [`lib/jkqtplottertools/jkqtpmathparser.h`](https://github.com/jkriege2/JKQtPlotter/blob/master/lib/jkqtplottertools/jkqtpmathparser.h) for details on the features of the math parser). An intelligent drawing algorithm chooses the number of control points for drawing a smooth graph, with sufficient amount of details, by evaluating locally the slope of the function.

In the example in [`test/simpletest_parsedfunctionplot/simpletest_parsedfunctionplot.cpp`](https://github.com/jkriege2/JKQtPlotter/blob/master/test/simpletest_parsedfunctionplot/simpletest_parsedfunctionplot.cpp) we do not simply set a fixed function, but add a `QLineEdit` which allows to edit the function and redraws it, once ENTER is pressed:
```c++
    JKQtPlotter* plot=new JKQtPlotter(&mainWin);
    QLineEdit* edit=new QLineEdit(&mainWin);
    edit->setToolTip("enter a function in dependence of the variable <tt>x</tt> and press ENTER to update the graph");
    
    // ...
    
    
    // 2. now we add a JKQTPxParsedFunctionLineGraph object, which will draw the function from
    //    the line edit
    JKQTPxParsedFunctionLineGraph* parsedFunc=new JKQTPxParsedFunctionLineGraph(plot);
    plot->addGraph(parsedFunc);
    //    finally we connect the line edit with the graph, whenever RETURN is pressed,
    //    the graph is updated:
    auto updateGraphFunctor=
       [=]() {
        parsedFunc->set_title("user function: \\verb{"+edit->text()+"}");
        parsedFunc->set_function(edit->text());
        plot->update_plot();
       };
    QObject::connect(edit, &QLineEdit::returnPressed, updateGraphFunctor);
    QObject::connect(edit, &QLineEdit::editingFinished, updateGraphFunctor);
    edit->setText("sin(x*8)*exp(-x/4)");
    updateGraphFunctor();
```


This code snippet results in a plot like this:

![jkqtplotter_simpletest_parsedfunctionplot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parsedfunctionplot.png)

### Plotting with parameters
As shown in [Plotting Mathematical Functions as Line Graphs](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_functionplot) you can also use externally set parameters in a plot function. These parameters can be double numbers and may be set with either as an internal parameter vector, or may be read from a parameter column (as shown in the [linked example](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_functionplot)). These parameters are available as variables `p1`, `p2`, ... in the function string. Here is a small example:

```c++
    JKQTPxParsedFunctionLineGraph* parsedFunc=new JKQTPxParsedFunctionLineGraph(plot);
    parsedFunc->set_function("sin(x*p1)*exp(-x/p2)");
    parsedFunc->set_paramV(/*p1=*/8, /*p2=*/4);
    parsedFunc->set_title("user function");
```

### Plot Function f(y)
If you use the graph class `JKQTPyParsedFunctionLineGraph` instead of `JKQTPxParsedFunctionLineGraph`, you can plot functions `x=f(y)` (instead of `y=f(x)`). The function from the example above will then ahve to be changed to `sin(y*8)*exp(-y/4)` and the result will look like this:

![jkqtplotter_simpletest_parsedfunctionplot_fy](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parsedfunctionplot_fy.png)


### Properties of the Adaptive Plotting Algorithm
The adaptive capabilities of the rendering algorithm can be seen, when plotting e.g. `2/x`, which is drawn smoothely, even around the undefined value at `x=0`:

![jkqtplotter_simpletest_parsedfunctionplot_2overx.png](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parsedfunctionplot_2overx.png)

With an additional checkbox in this example, you can switch drawing the actual sample points of the drawing algorithm on and off, by calling `parsedFunc->set_displaySamplePoints(...)`. This can be used to debug the drawing algorithm and explore its parameters (which you can set with `set_minSamples()`, `set_maxRefinementDegree()`, `set_slopeTolerance()`, `set_minPixelPerSample()`). Here is an example of a 2/x function with shown sample points:

![jkqtplotter_simpletest_parsedfunctionplot_2overx_samplepoints.png](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parsedfunctionplot_2overx_samplepoints.png)


[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)