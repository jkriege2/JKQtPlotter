#include <QApplication>
#include "testform.h"

int main(int argc, char* argv[])
{
        
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    TestForm w;
    w.show();
    w.updateMath();
    w.showMaximized();

    return app.exec();
}
