# source code for this simple demo
SOURCES += jkqtplotter_simpletest_rgbimageplot_qt.cpp

RESOURCES += jkqtplotter_simpletest_rgbimageplot_qt.qrc


# configure Qt
CONFIG += link_prl qt
QT += core gui xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = jkqtplotter_simpletest_rgbimageplot_qt

# include JKQTPlotter source code
DEPENDPATH += . ../../lib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../staticlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")

# here you can activate some debug options
#DEFINES += SHOW_JKQTPLOTTER_DEBUG
#DEFINES += JKQTBP_AUTOTIMER
