#include <QApplication>
#include <QtGui>
#include <cmath>
#include "testmain.h"
#include "jkqtpexampleapplication.h"

int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    TestMain tab;

    tab.show();

    return app.exec();
}
