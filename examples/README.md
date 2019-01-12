# JKQtPlotter: Examples: Examples & Tutorials
This section assembles some simple examples of usage. 
You can find more (complex) examples for the classes in this repository in the subfolder "test". 
All test-projects are Qt-projects that use qmake to build. You can load them into QtCreator easily.

## Different Plot Data Styles

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](../screenshots/jkqtplotter_simpletest1_small.png)](./simpletest) | [Very Basic Example (Line Graph)](./simpletest) | `JKQTPxyLineGraph` <br> C++-style QVector arrays of data |
| [![](../screenshots/jkqtplotter_simpletest_speed_small.png)](./simpletest_speed) | [Line Graph with Live Data / Speed Test](./simpletest_speed) | `JKQTPxyLineGraph` <br> external `std::array<double,N>` data, not owned by datastore <br> live-data, measure plotting speed <br> tipps to increas plotting speed |
| [![](../screenshots/jkqtplotter_simpletest_symbols_and_styles_small.png)](./simpletest_symbols_and_styles) | [Line Graph with Different Symbols and Line Styles](./simpletest_symbols_and_styles) | `JKQTPxyLineGraph` <br> C++ vector of data <br> setting line styles and symbol styles <br> automatic graph coloring |
| [![](../screenshots/jkqtplotter_simpletest_stepplots_small.png)](./simpletest_stepplots) | [Step Line Plots in Different Styles](./simpletest_stepplots) | `JKQTPstepHorizontalGraph` (and `JKQTPxyLineGraph`) <br> C++ vector of data <br> different step modes, filled and line-only |
| [![](../screenshots/jkqtplotter_simpletest_symbols_and_errors_small.png)](./simpletest_symbols_and_errors) | [Simple Line/Symbol Graph With Errorbars](./simpletest_symbols_and_errors) | `JKQTPxyLineErrorGraph` <br> C-style arrays of data |
| [![](../screenshots/jkqtplotter_simpletest_errorbarstyles_small.png)](./simpletest_errorbarstyles) | [Different Types of Error Indicators](./simpletest_errorbarstyles) | `JKQTPxyLineErrorGraph` <br> different styles of error indicators for x- and y-errors <br> C++-style QVector for data <br> styling error indicators <br> moving key and formatting plotter grid |
| [![](../screenshots/jkqtplotter_simpletest_barchart_small.png)](./simpletest_barchart) | [Simple Bar Charts](./simpletest_barchart) | `JKQTPbarVerticalGraph` <br> C-style arrays of data |
| [![](../screenshots/JKQTPbarVerticalGraphStacked_small.png) <br> ![](../screenshots/JKQTPbarHorizontalGraphStacked_small.png)](./simpletest_stackedbars) | [Stacked Bar Charts](./simpletest_stackedbars) | `JKQTPbarVerticalStackableGraph`, `JKQTPbarHorizontalStackableGraph` <br> C++-style vectors of data |
| [![](../screenshots/jkqtplotter_simpletest_filledgraphs_small.png)](./simpletest_filledgraphs) | [Filled Curve Plots](./simpletest_filledgraphs) | `JKQTPbarVerticalGraph` <br> setting/altering data in `JKQTPdatstore` directly <br>  transparent plots <br> calculating histograms |
| [![](../screenshots/jkqtplotter_simpletest_impulsesplot_small.png)](./simpletest_impulsesplot) | [Impulse Plots](./simpletest_impulsesplot) | `JKQTPimpulsesVerticalGraph` and `JKQTPimpulsesHorizontalGraph` <br> C++-style QVector as plot data |
| [![](../screenshots/jkqtplotter_simpletest_paramscatterplot_small.png)](./simpletest_paramscatterplot) | [Scatter Graph with Parametrized Symbols/Colors](./simpletest_paramscatterplot) | `JKQTPxyParametrizedScatterGraph` <br> C++-style QVector as plot data <br> modify scatter/points/line-graph properties by data |
| [![](../screenshots/jkqtplotter_simpletest_paramscatterplot_image_small.png)](./simpletest_paramscatterplot_image) | [Draw an Artistic Image with a Parametrized Scatter Graph](./simpletest_paramscatterplot_image) | `JKQTPxyParametrizedScatterGraph` <br> C++-style QVector as plot data <br> rectangular arrangement of scatters <br> generative computer graphics |
| [![](../screenshots/jkqtplotter_simpletest_parametriccurve_small.png)](./simpletest_parametriccurve) | [Plotting Parametric Curves](./simpletest_parametriccurve) | `JKQTPxyLineGraph` and `JKQTPxyParametrizedScatterGraph` <br> C++-style QVector as plot data <br> parametric curve plotting |
| [![](../screenshots/jkqtplotter_simpletest_functionplot_small.png)](./simpletest_functionplot) | [Plotting Mathematical Functions as Line Graphs](./simpletest_functionplot) | `JKQTPxFunctionLineGraph`  <br> diretly plotting C/C++-functions |
| [![](../screenshots/jkqtplotter_simpletest_parsedfunctionplot_small.png)](./simpletest_parsedfunctionplot) | [Plotting Parsed Mathematical Functions as Line Graphs](./simpletest_parsedfunctionplot) | `JKQTPxParsedFunctionLineGraph`  <br> plotting functions with the internal math equation parser/evaluator |
| [![](../screenshots/jkqtplotter_simpletest_geometric_small.png)](./simpletest_geometric) | [Plotting Geometric Objects](./simpletest_geometric) |  |

## Styling the Plot, Keys, Axes, ...

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](../screenshots/jkqtplotter_simpletest_logaxes_small.png)](./simpletest_logaxes) | [logarithmic axes](./simpletest_logaxes) | `JKQTPxyLineGraph` and `JKQTPgeoText` <br> C++ vector of data <br> logarithmic axes and styling <br> plot line styles <br> internal LaTeX parser <br> add commenting text to a graph |
| [![](../screenshots/jkqtplotter_simpletest_dateaxes_small.png) <br> ![](../screenshots/jkqtplotter_simpletest_dateaxes_dates_small.png) <br> ![](../screenshots/jkqtplotter_simpletest_dateaxes_timeaxis_small.png)](./simpletest_dateaxes) | [date/time axes](./simpletest_dateaxes) | `JKQTPxyLineGraph` and `JKQTPfilledVerticalRangeGraph` <br> C++ vector of data <br> date/time axes <br> plot min/max range graph <br> internal LaTeX parser <br> data from CSV files |


## Image data Plots

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](../screenshots/jkqtplotter_simpletest_rgbimageplot_qt_small.png)](./simpletest_rgbimageplot_qt) | [`QImage` as a Graph](./simpletest_rgbimageplot_qt) | `JKQTPImage` <br> `QImage` drawn onto a plot with arbitrary scaling) |
| [![](../screenshots/jkqtplotter_simpletest_imageplot_small.png)](./simpletest_imageplot) | [Basic 1-channel Raw C Image Plot](./simpletest_imageplot) | `JKQTPColumnMathImage` <br> image data copied from C-style row-major array into a single column of the internal datastore <br> Describes several options of the image plotting classes (different ways of color coding, what to do with data above/below the limits etc.) |
| [![](../screenshots/jkqtplotter_simpletest_imageplot_modifier_small.png)](./simpletest_imageplot_modifier) | [Modifier-Feature of Image Plots](./simpletest_imageplot_modifier) | `JKQTPColumnMathImage` <br> image data copied from C-style row-major array into a single column of the internal datastore <br> Image is modified by a second image to display two data dimensions at the same time |
| [![](../screenshots/jkqtplotter_simpletest_imageplot_nodatastore_small.png)](./simpletest_imageplot_nodatastore) | [Basic 1-channel Raw C Image Plot <br> without the internal datastore](./simpletest_imageplot_nodatastore) | `JKQTPMathImage` <br> image data in a C-style row-major array, not using internal datastore |
| [![](../screenshots/jkqtplotter_simpletest_imageplot_opencv_small.png)](./simpletest_imageplot_opencv) | [1-channel OpenCV cv::Mat Image Plot](./simpletest_imageplot_opencv) | `JKQTPColumnMathImage` <br> image data copied from OpenCV cv::Mat-structure into a single column of the internal datastore |
| [![](../screenshots/jkqtplotter_simpletest_rgbimageplot_opencv_small.png)](./simpletest_rgbimageplot_opencv) | [RGB OpenCV cv::Mat Image Plot](./simpletest_rgbimageplot_opencv) | `JKQTPColumnRGBMathImage` <br> image data copied from OpenCV cv::Mat-structure into three columns of the internal datastore |

## GUI Tools and Plot Layout

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](../screenshots/test_multiplot_small.png)](./test_multiplot) | [Layouting Several Plots](./test_multiplot) | Combining plots in Qt Layouts <br> linking plot axes <br> copy data from a `std::map` int the datastore <br> print plots/print preview |
| [![](../screenshots/jkqtplotter_simpletest_ui_small.png)](./simpletest_ui) | [Placing JKQtPlotter into a Qt User-Interface-XML-file (`*.ui`)](./simpletest_ui) | using Qt Form Designer <br> parsed function plots (`JKQTPxParsedFunctionLineGraph`) |

## Tools and Special Features

| Screenshot    | Description   | Notes         |
|:-------------:| ------------- | ------------- |
| [![](../screenshots/jkqtmathtext_simpletest_small.png)](./jkqtmathtext_simpletest) | [JKQTMathText: LaTeX Renderer](./jkqtmathtext_simpletest) |  |


