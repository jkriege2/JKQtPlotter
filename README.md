# JKQtPlotter
This is an extensive library of function/data plotter classes for Qt (>= 4.7, tested with Qt up to 5.4).

This software is licensed under the term of the GNU Lesser General Public License 2.1 
(LGPL 2.1) or above. 

## Examples
This section assembles some simple examples of usage. You can find more (complex) examples for the classes in this repository in the subfolder "test". All test-projects are Qt-projects that use qmake to build. You can load them into QtCreator easily.

###Very simple line-graph
This project (see `./test/jkqtplotter_simpletest/`) simply creates a JKQtPlotter widget (as a new window) and adds a single line-graph (a sine-wave). Data is initialized from two QVector<double> objects.
The QMake project looks like this (see `./test/jkqtplotter_simpletest/jkqtplotter_simpletest.pro`):
```qmake
# source code for this simple demo
SOURCES = jkqtplotter_simpletest.cpp 

# configure Qt
CONFIG += qt
QT += core gui svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = jkqtplotter_simpletest

# include JKQtPlotter source code
include(../../jkqtplotter.pri)
```
And the soruce code of the main application is (see `./test/jkqtplotter_simpletest/jkqtplotter_simpletest.cpp`):
```c++
#include <QApplication>
#include "jkqtplotter.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore 
	//    (for convenience)
    JKQtPlotter plot;
    JKQTPdatastore* ds=plot.getDatastore();

    // 2. now we create data for a simple plot (a sine curve)
    QVector<double> X, Y;
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*M_PI;
        X<<x;
        Y<<sin(x);
    }

    // 3. make data available to JKQtPlotter by adding it to the internal 
	//    datastore.
    //    Note: In this step the data is copied (of not specified otherwise), so
    //          you can reuse X and Y afterwards!
    //    The variables columnX and columnY will contain the internal column ID 
    //    of the newlycreated columns with names "x" and "y" and the (copied) 
	//    data from X and Y.
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY=ds->addCopiedColumn(Y, "y");

    // 4. create a graph in the plot, which plots the dataset X/Y:
    JKQTPxyLineGraph* graph1=new JKQTPxyLineGraph(&plot);
    graph1->set_xColumn(columnX);
    graph1->set_yColumn(columnY);
    graph1->set_title(QObject::tr("sine graph"));

    // 5. add the graph to the plot, so it is actually displayed
    plot.addGraph(graph1);

    // 6. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
```
The result looks like this:
![jkqtplotter_simpletest1](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest1.png)

###Very simple line-graph
This project (see `./test/jkqtplotter_simpletest_symbols_and_errors/`) simply creates a JKQtPlotter widget (as a new window) and adds a single line-graph (a sine-wave) that has y-errorbars. In addition, this example shows how to change some of the axis properties and how to use LaTeX markup to format axis labels (can actually be used for all labels in JKQtPlotter). Also, in comparison to the last example, here we initialize the data from C-type arrays (double*), instead of QVector<double> objects.

The soruce code of the main application is (see `./test/jkqtplotter_simpletest_symbols_and_errors/jkqtplotter_simpletest_symbols_and_errors.cpp`):
```c++
#include <QApplication>
#include "jkqtplotter.h"

// number of datapoints:
#define Ndata 10

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQtPlotter plot;
    JKQTPdatastore* ds=plot.getDatastore();

    // 2. now we create data for a simple plot (a sine curve with lin. increasing errors)
    double X[Ndata], Y[Ndata], YERROR[Ndata];
    for (int i=0; i<Ndata; i++) {
        X[i]=double(i)/double(Ndata)*2.0*M_PI;
        Y[i]=sin(X[i]);
        YERROR[i]=0.2+double(i)/double(Ndata)*0.25;
    }

    // 3. make data available to JKQtPlotter by adding it to the internal datastore.
    //    Note: In this step the data is copied (of not specified otherwise), so you can
    //          reuse X and Y afterwards!
    //    the variables columnX and columnY will contain the internal column ID of the newly
    //    created columns with names "x" and "y" and the (copied) data from X and Y.
    size_t columnX=ds->addCopiedColumn(X, Ndata, "x");
    size_t columnY=ds->addCopiedColumn(Y, Ndata, "y");
    size_t columnYE=ds->addCopiedColumn(YERROR, Ndata, "y-error");

    // 4. create a graph in the plot, which plots the dataset X/Y:
    JKQTPxyLineErrorGraph* graph1=new JKQTPxyLineErrorGraph(&plot);
    graph1->set_xColumn(columnX);
    graph1->set_yColumn(columnY);
    graph1->set_yErrorColumn(columnYE);
    graph1->set_symbol(JKQTPfilledStar); // set symbol style
    graph1->set_yErrorStyle(JKQTPerrorBars); // set error indicator type
    graph1->set_drawLine(false); // don't draw a line
    graph1->set_title(QObject::tr("sine graph"));

    // 5. add the graph to the plot, so it is actually displayed
    plot.addGraph(graph1);

    // 6. hide 0-lines
    plot.getXAxis()->set_showZeroAxis(false);
    plot.getYAxis()->set_showZeroAxis(false);

    // 7. set some axis properties (we use LaTeX for nice equation rendering)
    plot.getXAxis()->set_axisLabel(QObject::tr("x-axis $x$ [mm]"));
    plot.getYAxis()->set_axisLabel(QObject::tr("\\textbf{\\color{red}{y-axis} $\\left(y=\\sin(x)\\pm(0.2+0.25\\cdot x)\\right)$ [A.U.]}"));
    plot.getXAxis()->set_labelFont("Arial");
    plot.getYAxis()->set_labelFont("Times New Roman");
    plot.getYAxis()->set_labelFontSize(12); // large x-axis label
    plot.getYAxis()->set_tickLabelFontSize(10); // and larger y-axis tick labels

    // 8. autoscale the plot so the graph is contained
    plot.zoomToFit();

    // show plotter and make it a decent size
    plot.show();
    plot.resize(600,400);

    return app.exec();
}

```
The result looks like this:
![jkqtplotter_simpletest_symbols_and_errors](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_symbols_and_errors.png)


##Screenshots
###Scatter Plots and Boxplots
![Scatterplots](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/screen_scatter.png)

###Different Types of Barcharts
![Barcharts](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/screen_barcharts.png)

###Image Plots
You can plot C-arrays as images in different color-coding styles. Diferent Overlays/masks are also available. Finally you can use LaTeX markup to format any axis/plot/tick/... label. there is an internal LaTeX parser in this package.
![Scatterplots](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/screen_images_latex.png)


###Plotting a user-defined (parsed) function
Yes, a complete math expression parser is contained!
![Function Plot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/screen_functionplot.png)

###Axis-Label styles in LogLog-Plot
![LogLog Plot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/screen_loglog.png)


###Parametrized Scatter Plots and Data Viewer
Scatter Plots can have symbols where the shape/color/size is parametrized by a data column. Also the plotter is built around an internal datastore, which you can access (readonly!!!) by a data-viewer that is accessible from the contextmenu in any plot.
![LogLog Plot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/screen_parmetrizedplots_datatable.png)


#JKQTmathText
JKQTmathText is a hand-written LaTeX-renderer for Qt (implemented in native C++, using Qt). It supports a large set of standard LaTeX markup and can render it to a QPainter.
![LatexRender](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/mscreen_schroedinger.png)
![LatexRender](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/mscreen_rottaion.png)
![LatexRender](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/mscreen_maxwell.png)
![LatexRender](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/mscreen_cauchy.png)
![LatexRender](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/mscreen_sd.png)

