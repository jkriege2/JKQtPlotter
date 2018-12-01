# JKQtPlotter
This is an extensive library of function/data plotter classes for Qt (>= 4.7, tested with Qt up to 5.11).

This software is licensed under the term of the GNU Lesser General Public License 2.1 
(LGPL 2.1) or above. 

## Screenshots
[Screenshots directory](https://github.com/jkriege2/JKQtPlotter/tree/master/screenshots)


## Examples
This section assembles some simple examples of usage. 
You can find more (complex) examples for the classes in this repository in the subfolder "test". 
All test-projects are Qt-projects that use qmake to build. You can load them into QtCreator easily.

### Basic Plotting

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest1_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest) | [Very Basic Example (Line Graph)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest) | `JKQTPxyLineGraph`<br/>C-style arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_symbols_and_styles_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_symbols_and_styles) | [Line Graph with Different Symbols and Line Styles](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_symbols_and_styles) | `JKQTPxyLineGraph`<br/>C++ vector of data<br/>setting line styles and symbol styles<br/>automatic graph coloring |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_logaxes_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_logaxes) | [logarithmic axes](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_logaxes) | `JKQTPxyLineGraph` and `JKQTPgeoText`<br/>C++ vector of data<br/>logarithmic axes<br/>plot line styles<br/>internal LaTeX parser<br/>add commenting text to a graph |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_dateaxes_small.png)<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_dateaxes_dates_small.png)<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_dateaxes_timeaxis_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_dateaxes) | [date/time axes](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_dateaxes) | `JKQTPxyLineGraph` and `JKQTPfilledVerticalRangeGraph`<br/>C++ vector of data<br/>date/time axes<br/>plot min/max range graph<br/>internal LaTeX parser<br/>data from CSV files |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_symbols_and_errors_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_symbols_and_errors) | [Simple Line/Symbol Graph With Errorbars](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_symbols_and_errors) | `JKQTPxyLineErrorGraph`<br/>C-style arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_barchart_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_barchart) | [Simple Bar Charts](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_barchart) | `JKQTPbarVerticalGraph`<br/>C-style arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/JKQTPbarVerticalGraphStacked_small.png)<br>![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/JKQTPbarHorizontalGraphStacked_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_stackedbars) | [Stacked Bar Charts](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_stackedbars) | `JKQTPbarVerticalStackableGraph`, `JKQTPbarHorizontalStackableGraph`<br/>C++-style vectors of data |

### Image data Plots

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot) | [Basic 1-channel Raw C Image Plot](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot) | `JKQTPColumnMathImage`<br/>image data copied from C-style row-major array into a single column of the internal datastore<br>Describes several options of the image plotting classes (different ways of color coding, what to do with data above/below the limits etc.) |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_modifier_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot_modifier) | [Modifier-Feature of Image Plots](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot_modifier) | `JKQTPColumnMathImage`<br/>image data copied from C-style row-major array into a single column of the internal datastore<br>Image is modified by a second image to display two data dimensions at the same time |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_nodatastore_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot_nodatastore) | [Basic 1-channel Raw C Image Plot<br>without the internal datastore](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot_nodatastore) | `JKQTPMathImage`<br/>image data in a C-style row-major array, not using internal datastore |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_opencv_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot_opencv) | [1-channel OpenCV cv::Mat Image Plot](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot_opencv) | `JKQTPColumnMathImage`<br/>image data copied from OpenCV cv::Mat-structure into a single column of the internal datastore |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_rgbimageplot_opencv_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_rgbimageplot_opencv) | [RGB OpenCV cv::Mat Image Plot](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_rgbimageplot_opencv) | `JKQTPColumnRGBMathImage`<br/>image data copied from OpenCV cv::Mat-structure into three columns of the internal datastore |

### Tools and Special Features

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtmathtext_simpletest_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtmathtext_simpletest) | [JKQTMathText: LaTeX Renderer](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtmathtext_simpletest) |  |
