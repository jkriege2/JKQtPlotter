# JKQtPlotter
This is an extensive library of function/data plotter classes for Qt (>= 4.7, tested with Qt up to 5.4).

This software is licensed under the term of the GNU Lesser General Public License 2.1 
(LGPL 2.1) or above. 

## Examples
You can find usage examples for the classes in this repository in the subfolder "test". All test-projects are Qt-projects that use qmake to build. You can load them into QtCreator easily.

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

