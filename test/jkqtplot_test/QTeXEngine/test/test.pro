TARGET       = test
TEMPLATE     = app
CONFIG      += warn_on release thread

MOC_DIR      = ../tmp
OBJECTS_DIR  = ../tmp
DESTDIR      = ./

INCLUDEPATH += ../src
LIBS        += ../libQTeXEngine.a

SOURCES += test.cpp
