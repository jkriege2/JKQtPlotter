# -------------------------------------------------
# Project created by QtCreator 2010-06-13T12:40:50
# -------------------------------------------------
TARGET = JKQTFastPlotter_test

TEMPLATE = app

SOURCES += jkqtfastplotter_test.cpp \
    jkqtfastplotter_test_testmain.cpp

HEADERS += jkqtfastplotter_test_testmain.h

RCC_DIR = .rccs

CONFIG += link_prl qt windows

QT += core gui opengl

UI_DIR = .uics

OBJECTS_DIR = .objs

MOC_DIR = .mocs

DESTDIR = ./

DEFINES += DEBUG_TIMING

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# include JKQTPlotter library
DEPENDPATH += ../../lib  ../../staticlib/jkqtfastplotterlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/jkqtfastplotterlib/debug -ljkqtfastplotterlib_debug
} else {
    LIBS += -L../../staticlib/jkqtfastplotterlib/release -ljkqtfastplotterlib
}
message("LIBS = $$LIBS")
