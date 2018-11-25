# -------------------------------------------------
# Project created by QtCreator 2010-06-13T12:40:50
# -------------------------------------------------
TARGET = JKQTFastPlotter_test

TEMPLATE = app

include(../../lib/jkqtfastplotter.pri)

SOURCES += jkqtfastplotter_test.cpp \
    testmain.cpp

HEADERS += testmain.h

RCC_DIR = .rccs

CONFIG += qt windows

QT += core gui opengl

UI_DIR = .uics

OBJECTS_DIR = .objs

MOC_DIR = .mocs

DESTDIR = ./

DEFINES += DEBUG_TIMING

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
