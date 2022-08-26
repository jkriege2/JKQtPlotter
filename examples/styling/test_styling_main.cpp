#include <QApplication>
#include "test_styling.h"
#include "jkqtplotter/jkqtplotterstyle.h"
#include "jkqtpexampleapplication.h"


int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    // you can set the system-wide default style properties early on
    // all JKQTPlotter instance created after this, will use these
    // settings as their initial settings
    JKQTPGetSystemDefaultStyle().userActionFontSize=12;

    TestStyling win;
    win.show();

    return app.exec();
}
