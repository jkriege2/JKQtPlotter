# source code for this simple demo
SOURCES = test_distributionplot.cpp 

# configure Qt
CONFIG += link_prl qt
QT += core gui xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = test_distributionplot

# include JKQTPlotter source code
DEPENDPATH += ../../lib  ../../staticlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../staticlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")





