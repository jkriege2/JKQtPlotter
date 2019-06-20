#include <QApplication>
#include "test_user_interaction.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    TestUserInteraction win;
    win.show();

    return app.exec();
}
