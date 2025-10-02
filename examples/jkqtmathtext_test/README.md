# Example (JKQTMathText): A test application                                                 {#JKQTMathTextTestApp}


JKQTMathText is a hand-written LaTeX-renderer for Qt (implemented in native C++, using Qt). It supports a large set of standard LaTeX markup and can render it to a QPainter.

This test program demonstrates its capabilites. There are several pre-formulated LaTeX expressions that can be drawn, or you can enter a user-defined expression.
You can also set several properties of the rendering (fonts, font sizes, anti-aliasing, ...). You can also draw boxes around each part of the expression (this is mostly meant for development/debugging). In the lower-right you will see the LaTeX expression as a tree after parsing and you will get timing-values for the parsing and rendering and error output.

The application looks like this:

![jkqtmathtext_simpletest](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtmathtext_testapp.png)

