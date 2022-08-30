#include "jkqtpappsettingcontroller.h"
#include <QApplication>
#include <iostream>


JKQTPAppSettingController::JKQTPAppSettingController(int &argc, char **argv, bool enableHighDPIIfAVailable)
{
    // seed RNG
    std::srand(123456);
    bool deactivated=false;
    for (int i=0; i<argc; i++) {
        if (QString(argv[i])=="--disablehighdpi") {
            //std::cout<<"disabling High-DPI\n";
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false); // disable DPI support
            QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, false); // disable HiDPI pixmaps
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            QApplication::setAttribute(Qt::AA_Use96Dpi, true); // disable DPI support
#endif
            deactivated=true;
        }
    }
    if (enableHighDPIIfAVailable && !deactivated) {
        //std::cout<<"enabling High-DPI\n";
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true); // enable DPI support
        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true); // enable HiDPI pixmaps
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        QApplication::setAttribute(Qt::AA_Use96Dpi, false); // disable revert to 96DPI support
#endif
    }
}

JKQTPAppSettingController::~JKQTPAppSettingController()
{

}
