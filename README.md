# JKQtPlotter
This is an extensive library of function/data plotter classes for Qt (>= 4.7, tested with Qt up to 5.11).

This software is licensed under the term of the GNU Lesser General Public License 2.1 
(LGPL 2.1) or above. 

## Continuous Integration Status
[![Build status](https://ci.appveyor.com/api/projects/status/vq2o9pfi97isxm2a/branch/master?svg=true)](https://ci.appveyor.com/project/jkriege2/jkqtplotter/branch/master)


## Screenshots
[Screenshots directory](https://github.com/jkriege2/JKQtPlotter/tree/master/screenshots)


## Examples
This section assembles some simple examples of usage. 
You can find more (complex) examples for the classes in this repository in the subfolder "test". 
All test-projects are Qt-projects that use qmake to build. You can load them into QtCreator easily.

### Different Plot Data Styles

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest1_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest) | [Very Basic Example (Line Graph)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest) | `JKQTPxyLineGraph`<br/>C++-style QVector arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_speed_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_speed) | [Line Graph with Live Data / Speed Test](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_speed) | `JKQTPxyLineGraph`<br/>external `std::array<double,N>` data, not owned by datastore<br/>live-data, measure plotting speed<br/>tipps to increas plotting speed |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_symbols_and_styles_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_symbols_and_styles) | [Line Graph with Different Symbols and Line Styles](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_symbols_and_styles) | `JKQTPxyLineGraph`<br/>C++ vector of data<br/>setting line styles and symbol styles<br/>automatic graph coloring |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_stepplots_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_stepplots) | [Step Line Plots in Different Styles](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_stepplots) | `JKQTPstepHorizontalGraph` (and `JKQTPxyLineGraph`)<br/>C++ vector of data<br/>different step modes, filled and line-only |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_symbols_and_errors_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_symbols_and_errors) | [Simple Line/Symbol Graph With Errorbars](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_symbols_and_errors) | `JKQTPxyLineErrorGraph`<br/>C-style arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_errorbarstyles_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_errorbarstyles) | [Different Types of Error Indicators](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_errorbarstyles) | `JKQTPxyLineErrorGraph`<br/>different styles of error indicators for x- and y-errors<br>C++-style QVector for data<br/>styling error indicators<br/>moving key and formatting plotter grid |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_barchart_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_barchart) | [Simple Bar Charts](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_barchart) | `JKQTPbarVerticalGraph`<br/>C-style arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/JKQTPbarVerticalGraphStacked_small.png)<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/JKQTPbarHorizontalGraphStacked_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_stackedbars) | [Stacked Bar Charts](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_stackedbars) | `JKQTPbarVerticalStackableGraph`, `JKQTPbarHorizontalStackableGraph`<br/>C++-style vectors of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_filledgraphs_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_filledgraphs) | [Filled Curve Plots](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_filledgraphs) | `JKQTPbarVerticalGraph`<br/>setting/altering data in `JKQTPdatstore` directly<br/> transparent plots<br/>calculating histograms |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_impulsesplot_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_impulsesplot) | [Impulse Plots](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_impulsesplot) | `JKQTPimpulsesVerticalGraph` and `JKQTPimpulsesHorizontalGraph`<br/>C++-style QVector as plot data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_paramscatterplot_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_paramscatterplot) | [Scatter Graph with Parametrized Symbols/Colors](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_paramscatterplot) | `JKQTPxyParametrizedScatterGraph`<br/>C++-style QVector as plot data<br/>modify scatter/points/line-graph properties by data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parametriccurve_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_parametriccurve) | [Plotting Parametric Curves](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_parametriccurve) | `JKQTPxyLineGraph` and `JKQTPxyParametrizedScatterGraph`<br/>C++-style QVector as plot data<br/>parametric curve plotting |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_functionplot_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_functionplot) | [Plotting Mathematical Functions as Line Graphs](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_functionplot) | `JKQTPxFunctionLineGraph` <br/>diretly plotting C/C++-functions |
[![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_parsedfunctionplot_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_parsedfunctionplot) | [Plotting Parsed Mathematical Functions as Line Graphs](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_parsedfunctionplot) | `JKQTPxParsedFunctionLineGraph` <br/>plotting functions with the internal math equation parser/evaluator |

### Styling the Plot, Keys, Axes, ...

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_logaxes_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_logaxes) | [logarithmic axes](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_logaxes) | `JKQTPxyLineGraph` and `JKQTPgeoText`<br/>C++ vector of data<br/>logarithmic axes and styling<br/>plot line styles<br/>internal LaTeX parser<br/>add commenting text to a graph |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_dateaxes_small.png)<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_dateaxes_dates_small.png)<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_dateaxes_timeaxis_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_dateaxes) | [date/time axes](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_dateaxes) | `JKQTPxyLineGraph` and `JKQTPfilledVerticalRangeGraph`<br/>C++ vector of data<br/>date/time axes<br/>plot min/max range graph<br/>internal LaTeX parser<br/>data from CSV files |


### Image data Plots

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_rgbimageplot_qt_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_rgbimageplot_qt) | [`QImage` as a Graph](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_rgbimageplot_qt) | `JKQTPImage`<br/>`QImage` drawn onto a plot with arbitrary scaling) |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_imageplot) | [Basic 1-channel Raw C Image Plot](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_imageplot) | `JKQTPColumnMathImage`<br/>image data copied from C-style row-major array into a single column of the internal datastore<br>Describes several options of the image plotting classes (different ways of color coding, what to do with data above/below the limits etc.) |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_modifier_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_imageplot_modifier) | [Modifier-Feature of Image Plots](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_imageplot_modifier) | `JKQTPColumnMathImage`<br/>image data copied from C-style row-major array into a single column of the internal datastore<br>Image is modified by a second image to display two data dimensions at the same time |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_nodatastore_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_imageplot_nodatastore) | [Basic 1-channel Raw C Image Plot<br>without the internal datastore](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_imageplot_nodatastore) | `JKQTPMathImage`<br/>image data in a C-style row-major array, not using internal datastore |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_opencv_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_imageplot_opencv) | [1-channel OpenCV cv::Mat Image Plot](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_imageplot_opencv) | `JKQTPColumnMathImage`<br/>image data copied from OpenCV cv::Mat-structure into a single column of the internal datastore |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_rgbimageplot_opencv_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_rgbimageplot_opencv) | [RGB OpenCV cv::Mat Image Plot](https://github.com/jkriege2/JKQtPlotter/tree/master/test/simpletest_rgbimageplot_opencv) | `JKQTPColumnRGBMathImage`<br/>image data copied from OpenCV cv::Mat-structure into three columns of the internal datastore |

### GUI Tools and Plot Layout

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/test_multiplot_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/test_multiplot) | [Layouting Several Plots](https://github.com/jkriege2/JKQtPlotter/tree/master/test/test_multiplot) | Combining plots in Qt Layouts<br/>linking plot axes<br>copy data from a `std::map` int the datastore<br>print plots/print preview |

### Tools and Special Features

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtmathtext_simpletest_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtmathtext_simpletest) | [JKQTMathText: LaTeX Renderer](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtmathtext_simpletest) |  |
