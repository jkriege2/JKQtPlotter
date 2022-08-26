#include <QApplication>
#include "test_user_interaction.h"
#include "jkqtpexampleapplication.h"


int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    TestUserInteraction win;
    win.show();

    return app.exec();
}
