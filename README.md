# JKQTPlotter - A Qt Plotting Library
This is an extensive C++ library for data visualization, plotting and charting for Qt (>= 5.0, tested with Qt up to 6.3). It is feature-rich but self-contained and only depends on the [Qt framework](https://qt.io).

This software is licensed under the term of the [GNU Lesser General Public License 2.1 
(LGPL 2.1)](./LICENSE) or above. 

[![Lates Release](https://img.shields.io/github/v/release/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/releases)

![Language](https://img.shields.io/github/languages/top/jkriege2/JKQtPlotter)
[![Qt5](https://img.shields.io/badge/Qt-5-brightgreen)](https://doc.qt.io/qt-5/)
[![Qt6](https://img.shields.io/badge/Qt-6-brightgreen)](https://doc.qt.io/qt-6/)

[![Documentation](https://img.shields.io/badge/documentation-online-blue)](http://jkriege2.github.io/JKQtPlotter/index.html)

[![Build status](https://ci.appveyor.com/api/projects/status/vq2o9pfi97isxm2a?svg=true)](https://ci.appveyor.com/project/jkriege2/jkqtplotter)

[![Commit Activity](https://img.shields.io/github/commit-activity/m/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/pulse)
[![Last Commit](https://img.shields.io/github/last-commit/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/pulse)
[![Contributors](https://img.shields.io/github/contributors/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/graphs/contributors)

[![Open Issues](https://img.shields.io/github/issues/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/issues)
[![Closed Issues](https://img.shields.io/github/issues-closed/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/issues?q=is%3Aissue+is%3Aclosed)

[![Open PRs](https://img.shields.io/github/issues-pr/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/pulls)
[![Closed PRs](https://img.shields.io/github/issues-pr-closed/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/pulls?q=is%3Apr+is%3Aclosed)

[![CodeQL](https://github.com/jkriege2/JKQtPlotter/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/jkriege2/JKQtPlotter/actions/workflows/codeql-analysis.yml)
[![MSVC-CodeAnalysis](https://github.com/jkriege2/JKQtPlotter/actions/workflows/msvc-codeanalysis.yml/badge.svg)](https://github.com/jkriege2/JKQtPlotter/actions/workflows/msvc-codeanalysis.yml)

[![EXAMPLES-Page](./screenshots/examplesbanner.png)](https://jkriege2.github.io/JKQtPlotter/example_tutorial_projects.html)

## Main Features
- 2D Plotter widget class [JKQTPlotter](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter.html):
  - high-quality plotting
  - no other dependencies than Qt >= 5.0 ([CImg](https://cimg.eu/) and [OpenCV](https://opencv.org/) are optional dependencies)
  - highly customizable axes/grids (linear/log, date/time, custom ticks ...)
  - [JKQTMathText:](http://jkriege2.github.io/JKQtPlotter/group__jkqtmathtext.html) integrated LaTeX parser (pure C++, no dependencies) to render mathematical equations in axis labels, ticks, ...
  - extensive user-interactions pre-programmed (several zooming modes, selecting regions, custom context menus, switch graph visibility, ...)
  - full print and export (PDF,PNG,...) support with preview and parametrization out-the-box
  - [highly customizable look and feel](http://jkriege2.github.io/JKQtPlotter/group__jkqtpplotter__styling.html)
  - supports the Qt layout system for graphs and allows to symchronize several graphs with each other
- [centralized data management](http://jkriege2.github.io/JKQtPlotter/group__jkqtpdatastorage.html) in an internal datastore [JKQTPDatastore](http://jkriege2.github.io/JKQtPlotter/class_j_k_q_t_p_datastore.html):
  - data organized by columns, can also represent image data (ropw-major)
  - allows to reuse a column in several graphs
  - access via Qt's model view framework
  - external or internal datasets
  - complete with GUI (table view)
  - export capabilities (e.g. to CSV, SYLK, ...)
  - C++ standard iterator interface
  - [statistics library](http://jkriege2.github.io/JKQtPlotter/group__jkqtcommon__statistics__and__math.html) (basic statistics, boxplots, histograms, kernel density estimates, regression analysis, polynomial fitting)
- large variety of [graphs](https://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__concretegraphs.html) that can be added to a plot, e.g.:
  - [scatter plot](https://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__linesymbolgraphs__scatter.html) and [parametrized scatter plots (e.g. bubble charts)](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__linesymbolgraphs__param.html) 
  - [line graphs](https://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__linesymbolgraphs__line.html), [step graphs](http://jkriege2.github.io/JKQtPlotter/class_j_k_q_t_p_special_line_horizontal_graph.html), [impulses](https://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__sticks.html)
  - [filled curves](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__filledgraphs.html)
  - [barcharts](https://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__barcharts.html) (also stacked)
  - extensive support for different [styles of error indicators](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__basegraphserrors.html)
  - [integrated mathematical function parser](http://jkriege2.github.io/JKQtPlotter/group__jkqtptools__math__parser.html) for [parsed function plots](https://jkriege2.github.io/JKQtPlotter/class_j_k_q_t_p_x_parsed_function_line_graph.html) (with intelligent rendering algorithm)
  - line/scatter graphs can also be [based on C/C++ functions](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__functiongraphs.html) instead of data series (C++11 support!)
  - [statistical plots)](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__statgraphs.html) (e.g. boxplots, violinplots, ...)
  - large variety of [image plots](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__imagelots.html) (inclusing different color-scale modes, RGBA-plots, overlays/masks)
  - [contour plots](https://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__imagelots__contour.html)
  - [vector field graphs/quiver plots](https://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__vectorfieldgraphs.html)
  - [geometric forms](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__geoplots.html) / [annotations](http://jkriege2.github.io/JKQtPlotter/group__jkqtplotter__annotations.html)
  - can be easily extended by deriving a new graph from [JKQTPPlotElement](http://jkriege2.github.io/JKQtPlotter/class_j_k_q_t_p_plot_element.html), [JKQTPPlotAnnotationElement](http://jkriege2.github.io/JKQtPlotter/class_j_k_q_t_p_plot_annotation_element.html), [JKQTPGeometricPlotElement](http://jkriege2.github.io/JKQtPlotter/class_j_k_q_t_p_geometric_plot_element.html), [JKQTPGraph](http://jkriege2.github.io/JKQtPlotter/class_j_k_q_t_p_graph.html)
- optional: [OpenCV interface](http://jkriege2.github.io/JKQtPlotter/group__jkqtpinterfaceopencv.html), [CImg interfaces](http://jkriege2.github.io/JKQtPlotter/group__jkqtpinterfacecimg.html)
- CMake-based build system
- extensive set of [Examples/Tutorials](https://jkriege2.github.io/JKQtPlotter/example_tutorial_projects.html)
- extensive doxygen-generated [Documentation](http://jkriege2.github.io/JKQtPlotter/index.html)

## [Documentation](http://jkriege2.github.io/JKQtPlotter/index.html)
A Documentation (auto-)generated with [doxygen](http://www.doxygen.nl/) from the trunk source code can be found here: 
**[http://jkriege2.github.io/JKQTPlotter/index.html](http://jkriege2.github.io/JKQtPlotter/index.html)**

There are also some subpage of general intetest:
- [TODO List](http://jkriege2.github.io/JKQtPlotter/page_todo.html)
- [Release Notes & Version Overview](http://jkriege2.github.io/JKQtPlotter/page_whatsnew.html)

## [Examples](https://jkriege2.github.io/JKQtPlotter/example_tutorial_projects.html)

[![EXAMPLES-Page](./screenshots/examplesbanner.png)](https://jkriege2.github.io/JKQtPlotter/example_tutorial_projects.html)

There is a [large set of usage examples (with explanations for each) and tutorials](https://jkriege2.github.io/JKQtPlotter/example_tutorial_projects.html) in the folder [`./examples/`](./examples).
All test-projects are Qt-projects that use tcmake to build. Some of them are also available with additional qmake build-files.

In addition: The [Screenshots-folder](./screenshots/) contains several screenshots, partly taken from the provided examples, but also from other software using this libarary (e.g. [QuickFit 3.0](https://github.com/jkriege2/QuickFit3))

## Building Using CMake

[![Lates Release](https://img.shields.io/github/v/release/jkriege2/JKQtPlotter)](https://github.com/jkriege2/JKQtPlotter/releases)

JKQTPlotter contains two different build systems: A modern [CMake](https://cmake.org/)-based build and an older (and deprecated!) QMake-based build (which works out of the box with Qt 5.x and QT 6.x). Both systems are explained in detail in http://jkriege2.github.io/JKQtPlotter/page_buildinstructions.html.


With [CMake](https://cmake.org/) you can easily build JKQTPlotter and all its examples, by calling something like:
```
    $ mkdir build; cd build
    $ cmake .. -G "<cmake_generator>" "-DCMAKE_PREFIX_PATH=<path_to_your_qt_sources>" "-DCMAKE_INSTALL_PREFIX=<where_to_install>"
    $ cmake --build . --config "Debug"
    $ cmake --install . --config "Debug"
```

This will create CMake targets, which you can easily link against. For the main plotter library, the target's name is \c JKQTPlotter5::JKQTPlotter5 or  \c JKQTPlotter6::JKQTPlotter6 depending on the Qt-Version you use. You can then simmply link against this via:
```
  find_package(JKQTPlotter6 REQUIRED)
  target_link_libraries(${PROJECT_NAME} JKQTPlotter6::JKQTPlotter6)
```
or on a Qt-version agnostic way via:
```
  find_package(JKQTPlotter${QT_VERSION_MAJOR} REQUIRED)
  target_link_libraries(${PROJECT_NAME} JKQTPlotter${QT_VERSION_MAJOR}::JKQTPlotter${QT_VERSION_MAJOR})
```
See https://jkriege2.github.io/JKQtPlotter/page_buildinstructions__c_m_a_k_e.html for details.

## Usage via CMake's FetchConten-API

In addition to the method described above (i.e. build and install the library and then use it), you can also use JKQTPlotter via CMake's [FetchContent-API](https://cmake.org/cmake/help/latest/module/FetchContent.html). 

For this method, you need to add these lines to your CMake project:
```
include(FetchContent) # once in the project to include the module
# ... now declare JKQTPlotter5/6
FetchContent_Declare(JKQTPlotter${QT_VERSION_MAJOR}
                     GIT_REPOSITORY https://github.com/jkriege2/JKQtPlotter.git
                     # GIT_TAG        v5.0.0)
# ... finally make JKQTPlotter5/6 available
FetchContent_MakeAvailable(JKQTPlotter${QT_VERSION_MAJOR})
```

These declare JKQTPlotter and make it available in your project. Afterwards you should be able to link against it, using
```
target_link_libraries(${PROJECT_NAME} JKQTPlotter${QT_VERSION_MAJOR}::JKQTPlotter${QT_VERSION_MAJOR})
```


## Stargazers over time

[![Stargazers over time](https://starchart.cc/jkriege2/JKQtPlotter.svg)](https://starchart.cc/jkriege2/JKQtPlotter)
