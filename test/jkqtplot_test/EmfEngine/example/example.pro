include( ../config.pri )

TARGET         = example
TEMPLATE       = app
CONFIG        += warn_on release thread 
win32: CONFIG += console

MOC_DIR      = ../tmp
OBJECTS_DIR  = ../tmp
DESTDIR      = ./

INCLUDEPATH   += ../src
unix: INCLUDEPATH += /usr/local/include/libEMF

LIBS   += ../libEmfEngine.a

win32: LIBS   += -lgdi32
unix:  LIBS += /usr/local/lib/libEMF.a

HEADERS       = renderarea.h \
                pixmaps.h
SOURCES       = main.cpp \
                renderarea.cpp
