# source code for this simple demo
SOURCES = test_multiplot.cpp \
    test_multiplot_ui.cpp

# configure Qt
CONFIG += link_prl qt
CONFIG += link_prl c++11
QT += core gui xml  svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = multiplot


# include JKQTPlotter source code
DEPENDPATH += ../../lib  ../../staticlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../qmake/staticlib/jkqtplotterlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../qmake/staticlib/jkqtplotterlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")

win32-msvc* {
    QMAKE_CXXFLAGS += /EHsc /std:c++14
    # To enable M_PI, M_E,...
    DEFINES += _USE_MATH_DEFINES
    # To fix error: C2338: va_start argument must not
    # have reference type and must not be parenthesized
    DEFINES += _CRT_NO_VA_START_VALIDATION
    # fix errors with min()/max() macros from windows
    DEFINES += NOMINMAX
}






FORMS += \
    test_multiplot_ui.ui

HEADERS += \
    test_multiplot_ui.h

RESOURCES += \
    test_multiplot.qrc
