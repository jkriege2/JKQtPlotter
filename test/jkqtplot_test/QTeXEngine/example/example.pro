TARGET       = example
TEMPLATE     = app
CONFIG      += warn_on release thread

MOC_DIR      = ../tmp
OBJECTS_DIR  = ../tmp
DESTDIR      = ./

INCLUDEPATH += ../src
LIBS        += ../libQTeXEngine.a

HEADERS      = renderarea.h \
		   pixmaps.h
SOURCES      = main.cpp \
               renderarea.cpp
