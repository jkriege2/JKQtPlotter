#include <QApplication>
#include "test_styling.h"
#include "jkqtplotter/jkqtplotterstyle.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // you can set the system-wide default style properties early on
    // all JKQTPlotter instance created after this, will use these
    // settings as their initial settings
    JKQTPGetSystemDefaultStyle().userActionColor=QColor("red");

    TestStyling win;
    win.show();

    return app.exec();
}
