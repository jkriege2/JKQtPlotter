#include <QApplication>
#include "formwithjkqtplotter.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    FormWithJKQTPlotter plot;
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
