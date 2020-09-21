# JKQTPlotter - A Qt Plotting Library
This is an extensive Qt5 Plotter framework (including a feature-richt plotter widget, a speed-optimized, but limited variant and a LaTeX equation renderer!), written fully in C/C++ and without external dependencies.

This software is licensed under the term of the [GNU Lesser General Public License 2.1 
(LGPL 2.1)](./LICENSE) or above. 

* SourceRepositpry: https://github.com/jkriege2/JKQtPlotter
* Latest Release: https://github.com/jkriege2/JKQtPlotter/releases
* Documentation: https://jkriege2.github.io/JKQtPlotter/index.html
* Examples: [https://jkriege2.github.io/JKQtPlotter/example\_tutorial\_projects.html](https://jkriege2.github.io/JKQtPlotter/example_tutorial_projects.html)
* Screenshots: https://github.com/jkriege2/JKQtPlotter/tree/master/screenshots

![EXAMPLES-Page](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/examplesbanner.png)

## Main Features
- 2D Plotter widget class (JKQTPlotter)
  - high-quality plotting
  - no other dependencies than Qt >= 5.0 (tested up until 5.15)
  - highly customizable axes/grids (linear/log, date/time, custom ticks ...)
  - integrated LaTeX parser (pure C++, no dependencies) to render mathematical equations in axis labels, ticks, ...
  - extensive user-interactions pre-programmed (several zooming modes, selecting regions, custom context menus, switch graph visibility, ...)
  - full print and export (PDF,PNG,...) support with preview and parametrization out-the-box
  - highly customizable look and feel
  - supports the Qt layout system for graphs and allows to symchronize several graphs with each other
- centralized data management in an internal datastore:
  - data organized by columns, can also represent image data (ropw-major)
  - allows to reuse a column in several graphs
  - access via Qt's model view framework
  - external or internal datasets
  - complete with GUI (table view)
  - export capabilities (e.g. to CSV, SYLK, ...)
  - C++ standard iterator interface
  - statistics library (basic statistics, boxplots, histograms, kernel density estimates, regression analysis, polynomial fitting)
- large variety of graphs that can be added to a plot, e.g.:
  - scatter-plots (also parametrized color/size/symbol by a third data-column)
  - line graphs, step graphs, impulses
  - filled curves
  - barcharts (also stacked)
  - extensive support for different styles of error indicators
  - integrated mathematical function parser for function plots (with intelligent rendering algorithm)
  - line/scatter graphs can also be based on C/C++ functions instead of data series (C++11 support!)
  - statistical plots (e.g. boxplots)
  - extensive library of image plots (inclusing different color-scale modes, RGBA-plots, overlays/masks)
  - contour plots
  - geometric forms/annotations
  - can be easily extended by deriving a new graph from JKQTPPlotElement or JKQTPGeometricPlotElement
- optional: OpenCV, CImg interfaces
- CMake-based build system
- extensive set of Examples/Tutorials: [https://jkriege2.github.io/JKQtPlotter/example\_tutorial\_projects.html](https://jkriege2.github.io/JKQtPlotter/example_tutorial_projects.html)
- extensive doxygen-generated Documentation: http://jkriege2.github.io/JKQtPlotter/index.html

## Building

* Latest Release: [![Lates Release](https://img.shields.io/github/v/release/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/releases)

JKQTPlotter contains two different build systems: A modern [CMake](https://cmake.org/)-based build and an older (and deprecated!) QMake-based build (which works out of the box with Qt 5.x). Both systems are explained in detail in [http://jkriege2.github.io/JKQtPlotter/page\_buildinstructions.html](http://jkriege2.github.io/JKQtPlotter/page_buildinstructions.html).


With [CMake](https://cmake.org/) you can easily build JKQTPlotter and all its examples, by calling something like:
```
    $ mkdir build; cd build
    $ cmake .. -G "<cmake_generator>" "-DCMAKE_PREFIX_PATH=<path_to_your_qt_sources>"
    $ cmake --build . --config "Debug"
```

## Screenhhots

### Plotting

![Diverse Plots](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/datastore_groupedstat.png)
![Function Plotting](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/functionplot.png)
![Log-Axes](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/logaxes.png)
![Plot Symbols](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/symbols_and_styles.png)
![Error Indicators](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/errorbarstyles.png)
![Plot Styling](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/stylesbanner.png)
![Parametrized Plots](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/paramscatterplot.png)
![Boxplots](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/test_styledboxplot.png)
![Violin Plots](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/violinplot_vert.png)
![Statistics Library](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/datastore_regression.png)
![Scatter Plot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/paramscatterplot_image.png)
![Image Plot](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/imageplot_modifier.png)
![Multi-Plots, 2D Histograms](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/datastore_statistics_2d_histcontour.png)
![Contour Plot Animation](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/contourplot_animated.gif)
![Geometric Plots](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/geometric.png)

### LaTeX-Rendering
![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/mscreen_schroedinger.png)

## Build-In User-Interaction
![Zooming](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/zoomin_mouse_contextmenu.gif)
![Tooltips](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/tooltiptool.gif)
![Measure-Tool](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/rulertool.gif)
