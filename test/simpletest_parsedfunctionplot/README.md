[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Plotting Parsed Mathematical Functions as Line Graphs
This project (see `./test/simpletest_parsedfunctionplot/`) demonstrates how to plot mathematical functions as line graphs. The functions are defined as strings that will be evaluated with the equation parser, integrated into JKQtPlotter.

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

the adaptive capabilities of the rendering algorithm can be seen, when plotting e.g. `2/x`, which is drawn smoothely, even around the undefined value at `x=0`:

![jkqtplotter_simpletest_parsedfunctionplot_2overx.png](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parsedfunctionplot_2overx.png.png)


[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)