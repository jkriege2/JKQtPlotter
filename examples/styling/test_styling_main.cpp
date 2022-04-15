#include <QApplication>
#include "test_styling.h"
#include "jkqtplotter/jkqtplotterstyle.h"


int main(int argc, char* argv[])
{
        
#if QT_VERSION >= 0x050600
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#endif
    QApplication app(argc, argv);


    // you can set the system-wide default style properties early on
    // all JKQTPlotter instance created after this, will use these
    // settings as their initial settings
    JKQTPGetSystemDefaultStyle().userActionFontSize=12;

    TestStyling win;
    win.show();

    return app.exec();
}
