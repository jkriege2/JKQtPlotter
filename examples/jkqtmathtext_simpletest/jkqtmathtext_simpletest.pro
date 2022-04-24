TARGET = jkqtmathtext_simpletest
TEMPLATE = app

SOURCES += jkqtmathtext_simpletest.cpp

CONFIG += link_prl qt
QT += core gui xml  svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


DEPENDPATH += ../../lib  ../../qmake/staticlib/jkqtmathtextlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../qmake/staticlib/jkqtmathtextlib/debug -ljkqtmathtextlib_debug
} else {
    LIBS += -L../../qmake/staticlib/jkqtmathtextlib/release -ljkqtmathtextlib
}
message("LIBS = $$LIBS")

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
