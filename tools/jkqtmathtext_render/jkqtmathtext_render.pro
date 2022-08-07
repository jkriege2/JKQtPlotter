TARGET = jkqtmathtext_render
TEMPLATE = app

SOURCES += jkqtmathtext_render.cpp

CONFIG += link_prl qt
QT += core gui 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


DEPENDPATH += ../../lib  ../../qmake/staticlib/jkqtmathtextlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    DEPENDPATH += ../../qmake/staticlib/jkqtmathtextlib/debug
    LIBS += -L../../qmake/staticlib/jkqtmathtextlib/debug -L../../../qmake/staticlib/jkqtmathtextlib/debug -ljkqtmathtextlib_debug
} else {
    DEPENDPATH += ../../qmake/staticlib/jkqtmathtextlib/release
    LIBS += -L../../qmake/staticlib/jkqtmathtextlib/release -L../../../qmake/staticlib/jkqtmathtextlib/release -ljkqtmathtextlib
}
message("LIBS = $$LIBS")

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
