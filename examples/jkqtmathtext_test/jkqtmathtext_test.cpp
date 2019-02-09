#include <QApplication>
#include "testform.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    TestForm w;
    w.show();
    w.updateMath();
    w.showMaximized();

    return app.exec();
}
