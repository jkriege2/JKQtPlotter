#include <QApplication>
#include <QtGui>
#include <cmath>
#include "testmain.h"

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    TestMain tab;

    tab.show();

    return app.exec();
}
