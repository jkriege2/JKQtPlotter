#include <QApplication>
#include <QtGui>
#include <cmath>
#include "testmain.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    TestMain tab;

    tab.show();

    return app.exec();
}
