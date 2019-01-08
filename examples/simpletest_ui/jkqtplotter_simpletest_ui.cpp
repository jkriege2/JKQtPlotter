#include <QApplication>
#include "formwithjkqtplotter.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    FormWithJKQtPlotter plot;
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
