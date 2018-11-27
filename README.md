# JKQtPlotter
This is an extensive library of function/data plotter classes for Qt (>= 4.7, tested with Qt up to 5.4).

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
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest1_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest) | [Very Basic Example (Line Graph)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest) | C-style arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_symbols_and_errors_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_symbols_and_errors) | [Simple Line/Symbol Graph With Errorbars](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_symbols_and_errors) | C-style arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_barchart_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_barchart) | [Simple Bar Charts](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_barchart) | C-style arrays of data |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/JKQTPbarVerticalGraphStacked_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_stackedbars) | [Stacked Bar Charts](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_stackedbars) | C++-style vectors of data |

### Image data Plots

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot) | [1-channel Raw C Image Plot](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot) | image data copied from C-style row-major array into a single column of the internal datastore |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_imageplot_opencv_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot_opencv) | [1-channel OpenCV cv::Mat Image Plot](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtplotter_simpletest_imageplot_opencv) | image data copied from OpenCV cv::Mat-structure into a single column of the internal datastore |

### Tools and Special Features

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtmathtext_simpletest_small.png)](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtmathtext_simpletest) | [JKQTMathText: LaTeX Renderer](https://github.com/jkriege2/JKQtPlotter/tree/master/test/jkqtmathtext_simpletest) |  |
