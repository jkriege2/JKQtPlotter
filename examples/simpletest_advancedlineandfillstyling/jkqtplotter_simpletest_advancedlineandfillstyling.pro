# source code for this simple demo
SOURCES = jkqtplotter_simpletest_advancedlineandfillstyling.cpp 

RESOURCES += jkqtplotter_simpletest_advancedlineandfillstyling.qrc

# configure Qt
CONFIG += link_prl qt
QT += core gui xml  svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = jkqtplotter_simpletest_advancedlineandfillstyling


# include JKQTPlotter source code
DEPENDPATH += ../../lib  ../../staticlib/jkqtplotterlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/jkqtplotterlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../staticlib/jkqtplotterlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")


win32-msvc* {
    # To enable M_PI, M_E,...
    DEFINES += _USE_MATH_DEFINES
    # To fix error: C2338: va_start argument must not
    # have reference type and must not be parenthesized
    DEFINES += _CRT_NO_VA_START_VALIDATION
    # fix errors with min()/max() macros from windows
    DEFINES += NOMINMAX
}



