TEMPLATE = app

# output executable name
TARGET = jkqtplotter_simpletest_imageplot_nodatastore

# source code for this simple demo
SOURCES = jkqtplotter_simpletest_imageplot_nodatastore.cpp

# configure Qt
CONFIG += link_prl qt
QT += core gui xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# include JKQTPlotter source code
DEPENDPATH += ../../lib  ../../staticlib/jkqtplotterlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/jkqtplotterlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../staticlib/jkqtplotterlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")

!win32-msvc* {
    QMAKE_CXXFLAGS += -fexceptions
} else {
    QMAKE_CXXFLAGS += /EHsc 
    # /std:c++14
    # To enable M_PI, M_E,...
    DEFINES += _USE_MATH_DEFINES
    # To fix error: C2338: va_start argument must not
    # have reference type and must not be parenthesized
    DEFINES += _CRT_NO_VA_START_VALIDATION
}



