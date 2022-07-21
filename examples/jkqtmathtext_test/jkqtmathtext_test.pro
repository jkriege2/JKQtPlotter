TARGET = jkqtmathtext_test
TEMPLATE

SOURCES += jkqtmathtext_test.cpp \
    testform.cpp

HEADERS += testform.h 

FORMS += testform.ui

include($$PWD/asana.pri)

DEFINES += AUTOLOAD_XITS_FONTS AUTOLOAD_Asana_FONTS

CONFIG += link_prl qt windows
QT += core gui svg xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


DEPENDPATH += ../../lib  ../../qmake/staticlib/jkqtmathtextlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    DEPENDPATH += ../../qmake/staticlib/jkqtmathtextlib/debug
    LIBS += -L../../qmake/staticlib/jkqtmathtextlib/debug -ljkqtmathtextlib_debug
} else {
    DEPENDPATH += ../../qmake/staticlib/jkqtmathtextlib/release
    LIBS += -L../../qmake/staticlib/jkqtmathtextlib/release -ljkqtmathtextlib
}
message("LIBS = $$LIBS")

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
