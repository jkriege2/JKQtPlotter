include(xits.pri)
include(asana.pri)

LIBS +=


SOURCES = jkqtmathtext_test.cpp \
          ../../jkqtmathtext.cpp \
          ../../jkqtptools.cpp \
          ../../jkqtphighrestimer.cpp \
    testform.cpp

RCC_DIR = rccs

CONFIG += qt windows


TARGET = jkqtplot_test

QT += core gui svg

UI_DIR = .uics

HEADERS = ../../jkqtptools.h \
          ../../jkqtmathtext.h \
          ../../jkqtphighrestimer.h \
    testform.h

OBJECTS_DIR = .objs

INCLUDEPATH +=

MOC_DIR = .mocs

FORMS = \
    testform.ui

DESTDIR = ./

DEFINES += AUTOLOAD_XITS_FONTS AUTOLOAD_Asana_FONTS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
