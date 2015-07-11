# -------------------------------------------------
# Project created by QtCreator 2010-06-13T12:40:50
# -------------------------------------------------
TARGET = JKQTFastPlotter_test

TEMPLATE = app

SOURCES += jkqtfastplotter_test.cpp \
    ../../jkqtfastplotter.cpp \
    ../../jkqtptools.cpp \
    ../../jkqtphighrestimer.cpp \
    testmain.cpp

HEADERS += ../../jkqtfastplotter.h \
           ../../jkqtptools.h \
           ../../jkqtphighrestimer.h \
    testmain.h

RCC_DIR = .rccs

CONFIG += qt windows

QT += core gui opengl

UI_DIR = .uics

OBJECTS_DIR = .objs

MOC_DIR = .mocs

DESTDIR = ./

DEFINES += DEBUG_TIMING

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
