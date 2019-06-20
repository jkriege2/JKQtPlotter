include($$PWD/asana.pri)

LIBS +=


SOURCES += jkqtmathtext_test.cpp \
    testform.cpp

RCC_DIR = rccs

CONFIG += link_prl qt windows


TARGET = jkqtmathtext_test

QT += core gui svg xml

UI_DIR = .uics

HEADERS += testform.h 

OBJECTS_DIR = .objs

INCLUDEPATH +=

MOC_DIR = .mocs

FORMS += \
    testform.ui

DESTDIR = ./

DEFINES += AUTOLOAD_XITS_FONTS AUTOLOAD_Asana_FONTS

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
