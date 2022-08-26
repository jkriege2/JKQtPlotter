#include <QApplication>
#include "formwithjkqtplotter.h"
#include "jkqtpexampleapplication.h"


int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    FormWithJKQTPlotter plot;
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
