# Example (JKQTMathText): A simple usage example            {#JKQTMathTextSimpleExample}
JKQTMathText is a hand-written LaTeX-renderer for Qt (implemented in native C++, using Qt). It supports a large set of standard LaTeX markup and can render it to a QPainter.

This project (see `./examples/jkqtmathtext_simpletest/`) simply creates a QLabel (as a new window) that displays a rendered LaTeX equation (here the time-dependent Schrödinger equation).
The QMake project looks like this (see [`jkqtmathtext_simpletest.pro`](../jkqtmathtext_simpletest/jkqtmathtext_simpletest.pro):
```.qmake
# include JKQTMathText source-code, including the open-source XITS fonts
include(../../lib/jkqtmathtext_with_xits.pri)
SOURCES += jkqtmathtext_simpletest.cpp

# if you don't want to use the XITS fonts, use this line (and uncomment the 
# last two line!):
#include(../../lib/jkqtmathtext.pri)

CONFIG += qt
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = jkqtmathtext_simpletest
```
And the source code of the main application is (see [`jkqtmathtext_simpletest.cpp`](../jkqtmathtext_simpletest/jkqtmathtext_simpletest.cpp):
```.cpp
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include "jkqtmathtext/jkqtmathtext.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // we use a simple label to display the math text
    QLabel lab;

    // 1. we will paint into a QPixmap
    QPixmap pix(600,400);
    pix.fill(QColor("white"));
    QPainter painter;

    // 2. now we create a JKQTMathText object.
    //    Also we configure the JKQTMathText to use the XITS fonts that
    //    were included in the *.pro-file
    JKQTMathText mathText;
    mathText.useXITS();
    mathText.set_fontSize(20);

    // 3. now we parse some LaTeX code (the Schroedinger's equation), so
    //    we can draw it onto the QPixmap in the next step
    mathText.parse("$\\left[-\\frac{\\hbar^2}{2m}\\frac{\\partial^2}{\\partial x^2}+V(x)\\right]\\Psi(x)=\\mathrm{i}\\hbar\\frac{\\partial}{\\partial t}\\Psi(x)$");

    // 3. here we do the painting
    painter.begin(&pix);
    mathText.draw(painter, Qt::AlignCenter, QRectF(0,0,pix.width(), pix.height()), false);
    painter.end();

    // now we display and resize the label as a window
    lab.setPixmap(pix);
    lab.show();
    lab.resize(600,400);

    return app.exec();
}
```
The result looks like this:

![jkqtmathtext_simpletest](../../screenshots/jkqtmathtext_simpletest.png)

