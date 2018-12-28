#include <QApplication>
#include "testmain.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    TestMain w;
    w.show();
    return a.exec();
}
