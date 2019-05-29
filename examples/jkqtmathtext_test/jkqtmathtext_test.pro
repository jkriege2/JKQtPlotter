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


DEPENDPATH += ../../lib  ../../staticlib/jkqtmathtextlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    DEPENDPATH += ../../staticlib/jkqtmathtextlib/debug
    LIBS += -L../../staticlib/jkqtmathtextlib/debug -ljkqtmathtextlib_debug
} else {
    DEPENDPATH += ../../staticlib/jkqtmathtextlib/release
    LIBS += -L../../staticlib/jkqtmathtextlib/release -ljkqtmathtextlib
}
message("LIBS = $$LIBS")
