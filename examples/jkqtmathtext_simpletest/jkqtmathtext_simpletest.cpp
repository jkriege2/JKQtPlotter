/** \example jkqtmathtext_simpletest.cpp
 * A very basic example for the usage of JKQTMathText
 *
 * \ref JKQTMathTextSimpleExample
 */
 
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include "jkqtmathtext/jkqtmathtext.h"

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
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
    mathText.setFontSize(20);

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
