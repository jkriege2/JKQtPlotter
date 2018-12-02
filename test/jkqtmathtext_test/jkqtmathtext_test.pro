include($$PWD/asana.pri)

LIBS +=


SOURCES += jkqtmathtext_test.cpp \
    testform.cpp

RCC_DIR = rccs

CONFIG += qt windows


TARGET = jkqtplot_test

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


DEPENDPATH += . ../../lib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release):LIBS += -L../../lib/debug -ljkqtplotterlib
CONFIG (release):LIBS += -L../../lib/release -ljkqtplotterlib